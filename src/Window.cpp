#include "Window.h"
#include "Camera.h"
#include "InputProcessing.h"
#include "Shadow.h"
#include "Light.h"
#include "ShaderStore.h"
#include "objects/base/Renderable.h"
#include "objects/primitives/IcoSphere.h"
#include "World.h"
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#define CAMERA_SPEED 2.5f

constexpr int width = 1280, height = 720;
bool wireframe = false;
bool mouseActive = false;
double deltaTime = 0.0;
std::chrono::time_point<std::chrono::high_resolution_clock> lastFrame;

Camera camera;
InputProcessing input;
ShadowProcessor shadowProcessor;
LightManager lightManager;

World *world;
GLFWframebuffersizefun prev_framebuffer_size_callback;
GLFWcursorposfun prev_cursor_position_callback;
GLFWmousebuttonfun prev_mouse_button_callback;
GLFWscrollfun prev_scroll_callback;

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

int Window::init()
{
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
    auto sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(4, 4, -4));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(-4, 4, 4));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(4, 4, 4));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(-4, 4, -4));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(-6, 6, -6));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(6, 6, 6));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(6, 6, -6));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(-6, 6, 6));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
    sphere = new IcoSphere();
    sphere->create(3);
    sphere->set_position(glm::vec3(-8, 8, 2));
    sphere->set_shader(ShaderStore::get_shader("noLight"));
    sphere->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(sphere->get_material())->color = glm::vec3(0.5f, 0.5f, 0.0f);
    world->insert(sphere);
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
    deltaTime = std::chrono::duration<double>(currentFrame - lastFrame).count();
    lastFrame = currentFrame;
    glfwPollEvents();
    input.process_keyboard(window, deltaTime);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
}

void Window::render() const
{
    ImGui::Render();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world->draw_debug();

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