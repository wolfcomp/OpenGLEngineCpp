#include "Window.h"
#include "Camera.h"
#include "InputProcessing.h"
#include "Shadow.h"
#include "Light.h"
#include "ShaderStore.h"
#include "objects/base/Renderable.h"
#include "objects/primitives/IcoSphere.h"
#include "objects/primitives/Cube.h"
#include "objects/debug/Line.h"
#include "objects/debug/Arrow.h"
#include "colliders/SphereCollider.h"
#include "World.h"
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "HSL.h"

#define CAMERA_SPEED 2.5f

constexpr int width = 1280, height = 720;
bool wireframe = false;
bool mouseActive = false;
bool drawDebug = false;
double deltaTime = 0.0;
double lastWorldUpdate = 0.0;
constexpr double minDeltaTime = 1 / 60.0;
float fps = 0.0f;
float fpsAvg[100] = {0};
std::chrono::time_point<std::chrono::high_resolution_clock> lastFrame = std::chrono::high_resolution_clock::now();
int spawnCount = 50;

Camera camera = Camera(glm::vec3(1.5f, 3.0f, 11.5f), glm::vec3(0.0f, 1.0f, 0.0f), -101.0f, -14.5f);
InputProcessing input;
ShadowProcessor shadowProcessor;
LightManager lightManager;

World *world;
Line *debugLine;
Arrow *debugArrow;
IcoSphere *debugSphere;
GLFWframebuffersizefun prev_framebuffer_size_callback;
GLFWcursorposfun prev_cursor_position_callback;
GLFWmousebuttonfun prev_mouse_button_callback;
GLFWscrollfun prev_scroll_callback;
glm::vec3 a = glm::normalize(glm::vec3(-.5, 0, -1));
glm::vec3 b = {0, 0, -1};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    // prev_framebuffer_size_callback(window, width, height);
}

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void move_character(const glm::vec3 &direction)
{
    if (!mouseActive)
        return;
    auto pos = camera.get_pos();
    pos += camera.get_movement(direction) * (float)(CAMERA_SPEED * deltaTime);
    camera.set_position(pos);
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    input.process_mouse_move(window, xpos, ypos);
    prev_cursor_position_callback(window, xpos, ypos);
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    input.process_mouse_button(window, button, action, mods);
    prev_mouse_button_callback(window, button, action, mods);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    input.process_mouse_scroll(window, xoffset, yoffset);
    prev_scroll_callback(window, xoffset, yoffset);
}

IcoSphere *create_new(glm::vec3 position, glm::vec3 velocity, glm::vec3 color, float mass = 10)
{
    IcoSphere *sphere;
    world->insert(sphere = new IcoSphere(position));
    sphere->create(3);
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    sphere->set_scale(glm::vec3(mass / 100));
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec4(color, 1);
    sphere->set_velocity(velocity);
    sphere->set_mass(mass);
    return sphere;
}

Cube *create_new_cube(glm::vec3 postion, glm::vec3 velocity, glm::vec3 color)
{
    Cube *cube;
    world->insert(cube = new Cube(postion));
    cube->set_shader(ShaderStore::get_shader("noLight"));
    cube->set_material(new ColorMaterial());
    cube->set_scale(glm::vec3(0.1f));
    dynamic_cast<ColorMaterial *>(cube->get_material())->color = glm::vec4(color, 1);
    cube->set_velocity(velocity);
    cube->get_collider()->set_channel(CollisionChannel::STATIC);
    return cube;
}

void spawn_random()
{
    auto pos = glm::vec3(0, 0, 0);
    auto vel = glm::vec3(0, 0, 0);
    auto color = glm::vec3(0, 0, 0);
    auto mass = 1.0f;
    for (int i = 0; i < spawnCount; i++)
    {
        pos = glm::vec3(rand() % 2000, rand() % 2000, rand() % 2000) / 100.0f - glm::vec3(10, 10, 10);
        vel = glm::vec3(rand() % 1000, rand() % 1000, rand() % 1000) / 100.0f;
        color = hsl(rand() % 360, (rand() % 255) / 255.0f, (rand() % 128 + 127) / 255.0f).get_rgb_vec3();
        mass = (rand() % 25);
        create_new(pos, vel, color, mass);
    }
}

int Window::init()
{
    srand(time(NULL));
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "OpenGLEngineCpp", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // disable vsync
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    init_listeners();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    prev_framebuffer_size_callback = glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    prev_cursor_position_callback = glfwSetCursorPosCallback(window, cursor_position_callback);
    prev_mouse_button_callback = glfwSetMouseButtonCallback(window, mouse_button_callback);
    prev_scroll_callback = glfwSetScrollCallback(window, scroll_callback);
    ShaderStore::add_params_callback([](const Shader *shader)
                                     { camera.set_shader(shader);
                                       lightManager.set_shader(shader);
                                       shadowProcessor.set_shader(shader);
                                       input.set_shader(shader); });
    ShaderStore::load_shaders();
    Renderable::setup();
    world = new World();
    world->set_bounds(glm::vec3(0, 0, 0), glm::vec3(10, 10, 10));
    debugLine = new Line();
    debugLine->set_shader(ShaderStore::get_shader("noLight"));
    debugLine->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(debugLine->get_material())->color = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
    debugArrow = new Arrow();
    debugArrow->set_shader(ShaderStore::get_shader("noLight"));
    debugArrow->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(debugArrow->get_material())->color = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
    debugSphere = new IcoSphere(glm::vec3(0, 0, 0));
    debugSphere->create(3);
    debugSphere->set_shader(ShaderStore::get_shader("noLight"));
    debugSphere->set_material(new ColorMaterial());
    debugSphere->set_scale(glm::vec3(0.1f));
    dynamic_cast<ColorMaterial *>(debugSphere->get_material())->color = glm::vec4(0.0f, 0.0f, 1.0f, 0.5f);
    return 0;
}

void Window::init_listeners()
{
    input.attach_keyboard_listener(
        GLFW_KEY_F, []()
        {
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_FILL : GL_LINE); 
            wireframe = !wireframe; },
        false);
    input.attach_keyboard_listener(
        GLFW_KEY_Q, []()
        { drawDebug = !drawDebug; },
        false);
    input.attach_keyboard_listener(
        GLFW_KEY_W, []()
        { move_character(glm::vec3(0, 0, -1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_S, []()
        { move_character(glm::vec3(0, 0, 1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_A, []()
        { move_character(glm::vec3(-1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_D, []()
        { move_character(glm::vec3(1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_SPACE, []()
        { move_character(glm::vec3(0, 1, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_LEFT_CONTROL, []()
        { move_character(glm::vec3(0, -1, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_ESCAPE, [&]()
        { 
            if(mouseActive)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            input.process_mouse_move(window, xpos, ypos, true);
            mouseActive = !mouseActive; },
        false);
    input.attach_mouse_listener([](MouseInput input)
                                { if(mouseActive) camera.process_mouse(input.x_pos_offset, input.y_pos_offset); });
}

void Window::update() const
{
    auto currentFrame = std::chrono::high_resolution_clock::now();
    // calculate the time it took to render the last frame and update the deltaTime variable
    // has to do microseconds due to fps being over 1000
    deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentFrame - lastFrame).count() / 1000000.0f;
    lastFrame = currentFrame;
    fps = 1.0f / deltaTime;
    // calculate the average fps
    for (int i = 0; i < 99; i++)
    {
        fpsAvg[i] = fpsAvg[i + 1];
    }
    fpsAvg[99] = fps;
    float sum = 0;
    for (int i = 0; i < 100; i++)
    {
        sum += fpsAvg[i];
    }
    fps = sum / 100;
    glfwPollEvents();
    input.process_keyboard(window, deltaTime);
    if (lastWorldUpdate >= minDeltaTime)
    {
        world->update(minDeltaTime);
        lastWorldUpdate = 0;
    }
    else
    {
        lastWorldUpdate += deltaTime;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (drawDebug)
        ImGui::ShowDemoWindow();

    ImGui::Begin("Debug");
    ImGui::SetWindowSize(ImVec2(311, 235), ImGuiCond_FirstUseEver);
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Separator();
    auto spawnCountRef = &spawnCount;
    ImGui::SetNextItemWidth(100);
    if (ImGui::InputInt("Number of objects to spawn", spawnCountRef))
    {
        spawnCount = std::max(std::min(*spawnCountRef, 999), 1);
    }
    if (ImGui::Button("Spawn balls in scene"))
    {
        spawn_random();
    }
    if (ImGui::Button("Clear scene"))
    {
        world->clear();
    }
    ImGui::Separator();
    ImGui::TextUnformatted("w, a, s, d, space, ctrl - move camera");
    ImGui::TextUnformatted("  * forward, left, back, right, up, down");
    ImGui::TextUnformatted("f - toggle wireframe");
    ImGui::TextUnformatted("q - toggle debug");
    ImGui::TextUnformatted("esc - toggle camera control");
    ImGui::TextUnformatted("Mouse - rotate camera");
    ImGui::End();
}

void Window::render() const
{
    ImGui::Render();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (drawDebug)
    {
        world->draw_debug(debugLine, debugArrow);
        debugSphere->draw();
    }

    world->draw();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(window);
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}