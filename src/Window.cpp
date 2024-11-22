#include "Window.h"
#include "input/Camera.h"
#include "input/InputProcessing.h"
#include "shaders/Shadow.h"
#include "Light.h"
#include "shaders/ShaderStore.h"
#include "objects/base/GameObjectBase.h"
#include "objects/primitives/IcoSphere.h"
#include "objects/primitives/Cube.h"
#include "objects/debug/Line.h"
#include "objects/debug/Arrow.h"
#include "colliders/SphereCollider.h"
#include "World.h"
#include <iostream>
#include <algorithm>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <chrono>
#include "HSL.h"
#include "culling/Frustum.h"
#include "objects/curves/Bezier.h"
#include "objects/curves/BSpline.h"
#include "objects/curves/BSplineSurface.h"
#include "objects/surface/PointCloud.h"
#include "objects/combined/TrackedBall.h"
#include "ecs/components/physics.h"
#include "ecs/systems/physics.h"

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
Frustum frustum;
DrawCounts drawCounts;

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

void move_camera(const glm::vec3 &direction)
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

void spawn_on_camera()
{
    auto pos = camera.get_pos();
    auto ball = new TrackedBall();
    ball->set_shader(ShaderStore::get_shader("default"));
    ball->set_material(new ColorMaterial(glm::vec4(1)));
    world->insert(ball);
    world->get_ecs()->insert<PhysicsComponent>(ball->get_uuid(), new PhysicsComponent{});
    ball->get_component<TransformComponent>()->set_position(pos);
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

    glfWindow = glfwCreateWindow(width, height, "OpenGLEngineCpp", nullptr, nullptr);
    if (glfWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(glfWindow);
    // disable vsync
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowTitle(glfWindow, "Starting up ImGui context");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glfWindow, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    glfwSetWindowTitle(glfWindow, "Initializing listeners");
    init_listeners();

    glfwSetWindowTitle(glfWindow, "Setting up OpenGL");
    glViewport(0, 0, width, height);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glfwSetInputMode(glfWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetWindowTitle(glfWindow, "Setting up internal callbacks");
    prev_framebuffer_size_callback = glfwSetFramebufferSizeCallback(glfWindow, framebuffer_size_callback);
    prev_cursor_position_callback = glfwSetCursorPosCallback(glfWindow, cursor_position_callback);
    prev_mouse_button_callback = glfwSetMouseButtonCallback(glfWindow, mouse_button_callback);
    prev_scroll_callback = glfwSetScrollCallback(glfWindow, scroll_callback);

    glfwSetWindowTitle(glfWindow, "Loading shaders");
    ShaderStore::add_params_callback([](const Shader *shader)
                                     { camera.set_shader(shader);
                                       shadowProcessor.set_shader(shader);
                                       input.set_shader(shader); });
    ShaderStore::load_shaders();

    glfwSetWindowTitle(glfWindow, "Setting up buffers");
    GameObjectBase::setup();

    glfwSetWindowTitle(glfWindow, "Setting up world and debug objects");
    world = new World();
    ShaderStore::add_params_callback([](const Shader *shader)
                                     { world->set_shader(shader); });
    world->set_directional_light(new DirectionalLight());
    world->update_directional_light([](DirectionalLight *light)
                                    {
                                        light->direction = glm::normalize(glm::vec3(0.2f, -1.0f, -0.3f));
                                        light->ambient = hsl(0, 0, 0.2f);
                                        light->diffuse = hsl(0, 0, 0.8f);
                                        light->specular = hsl(0, 0, 0.5f); });
    world->register_system(new PhysicsSystem(world->get_ecs(), world));
    debugLine = new Line();
    debugLine->set_shader(ShaderStore::get_shader("noLight"));
    debugLine->set_material(new ColorMaterial());
    debugLine->attatch_to_world(world);
    debugLine->register_ecs(world->get_ecs());
    dynamic_cast<ColorMaterial *>(debugLine->get_material())->color = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
    debugArrow = new Arrow();
    debugArrow->set_shader(ShaderStore::get_shader("noLight"));
    debugArrow->set_material(new ColorMaterial());
    debugArrow->attatch_to_world(world);
    debugArrow->register_ecs(world->get_ecs());
    dynamic_cast<ColorMaterial *>(debugArrow->get_material())->color = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
    debugSphere = new IcoSphere();
    debugSphere->create(3);
    debugSphere->set_shader(ShaderStore::get_shader("noLight"));
    debugSphere->set_material(new ColorMaterial());
    debugSphere->attatch_to_world(world);
    debugSphere->register_ecs(world->get_ecs());
    debugSphere->get_component<TransformComponent>()->set_scale(glm::vec3(.1f));
    dynamic_cast<ColorMaterial *>(debugSphere->get_material())->color = glm::vec4(0.0f, 0.0f, 1.0f, 0.5f);

    glfwSetWindowTitle(glfWindow, "Setting up point cloud surface");
    auto pointCloud = new PointCloud("./pointcloud/small.las");
    auto bsplineSurface = pointCloud->convert_to_surface();
    delete pointCloud;
    bsplineSurface->set_shader(ShaderStore::get_shader("default"));
    bsplineSurface->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(bsplineSurface->get_material())->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    world->insert(bsplineSurface);
    auto vertices = bsplineSurface->get_vertices();
    auto min = glm::vec3(FLT_MAX);
    auto max = glm::vec3(-FLT_MAX);
    for (auto &vertex : vertices)
    {
        min = glm::min(min, vertex.position);
        max = glm::max(max, vertex.position);
    }
    auto center = (min + max) / 2.0f;
    auto extent = (max - min) / 2.0f;
    world->set_bounds(center, extent);

    glfwSetWindowTitle(glfWindow, "OpenGLEngineCpp");
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
        { move_camera(glm::vec3(0, 0, -1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_S, []()
        { move_camera(glm::vec3(0, 0, 1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_A, []()
        { move_camera(glm::vec3(-1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_D, []()
        { move_camera(glm::vec3(1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_SPACE, []()
        { move_camera(glm::vec3(0, 1, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_LEFT_CONTROL, []()
        { move_camera(glm::vec3(0, -1, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_ESCAPE, [&]()
        { 
            if(mouseActive)
                glfwSetInputMode(glfWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(glfWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double xpos, ypos;
            glfwGetCursorPos(glfWindow, &xpos, &ypos);
            input.process_mouse_move(glfWindow, xpos, ypos, true);
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
    input.process_keyboard(glfWindow, deltaTime);
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

    // ImGui::ShowDemoWindow();

    ImGui::Begin("Debug");
    ImGui::SetWindowSize(ImVec2(311, 235), ImGuiCond_FirstUseEver);
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Separator();
    if (ImGui::Button("Spawn on camera"))
    {
        spawn_on_camera();
    }
    ImGui::Separator();
    ImGui::TextUnformatted("w, a, s, d, space, ctrl - move camera");
    ImGui::TextUnformatted("  * forward, left, back, right, up, down");
    ImGui::TextUnformatted("f - toggle wireframe");
    ImGui::TextUnformatted("q - toggle debug");
    ImGui::TextUnformatted("esc - toggle camera control");
    ImGui::TextUnformatted("Mouse - rotate camera");
    ImGui::End();

    if (drawDebug)
    {
        ImGui::Begin("Camera");
        ImGui::SetWindowSize(ImVec2(311, 235), ImGuiCond_FirstUseEver);
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", camera.get_pos().x, camera.get_pos().y, camera.get_pos().z);
        ImGui::Text("Front: (%.2f, %.2f, %.2f)", camera.get_front().x, camera.get_front().y, camera.get_front().z);
        ImGui::Text("Up: (%.2f, %.2f, %.2f)", camera.get_up().x, camera.get_up().y, camera.get_up().z);
        ImGui::Text("Right: (%.2f, %.2f, %.2f)", camera.get_right().x, camera.get_right().y, camera.get_right().z);
        ImGui::End();

        ImGui::Begin("World");
        ImGui::SetWindowSize(ImVec2(311, 235), ImGuiCond_FirstUseEver);
        ImGui::Text("Objects after culling: %d", drawCounts.objects_culled);
        ImGui::Text("Objects filtered: %d", drawCounts.objects_filtered);
        ImGui::Text("Objects drawn: %d", drawCounts.objects_drawn);
        ImGui::End();
    }

    ImGui::Begin("Light");
    ImGui::SetWindowSize(ImVec2(311, 235), ImGuiCond_FirstUseEver);
    world->draw_light_editor();
    ImGui::End();
    frustum = Frustum::create_from_camera_and_input(&camera, &input);
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

    drawCounts = world->draw(&frustum);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(glfWindow);
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(glfWindow);
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(glfWindow);
    glfwTerminate();
}