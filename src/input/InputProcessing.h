#pragma once
#include <map>
#include <glm/glm.hpp>
#include <functional>

#include "Camera.h"
#include "../shaders/Shader.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

struct MouseInput
{
    double x_pos;
    double y_pos;
    double x_pos_offset;
    double y_pos_offset;
    double x_scroll;
    double y_scroll;
};

class InputProcessing
{
    glm::mat4 projection;
    std::map<int, std::function<void()>> keyboard_listeners;
    std::map<int, void (*)(MouseInput)> mouse_listeners;
    std::map<int, bool> should_repeat;
    std::map<int, bool> key_pressed;
    float zoom;
    float aspect;
    int width;
    int height;
    MouseInput last;

public:
    InputProcessing();
    void change_aspect(float width, float height);
    void process_keyboard(GLFWwindow *window, double delta_time);
    void process_mouse_button(GLFWwindow *window, int button, int action, int mods);
    void process_mouse_move(GLFWwindow *window, double x_offset, double y_offset, bool resetPosition = false);
    void process_mouse_scroll(GLFWwindow *window, double x_scroll, double y_scroll);
    void attach_keyboard_listener(int key, std::function<void()> event_handler, bool repeat);
    int attach_mouse_listener(void (*event_handler)(MouseInput input));
    void remove_keyboard_listener(int key);
    void remove_mouse_listener(int listener);
    void set_shader(const Shader *shader);
    glm::mat4 get_projection() const;
    glm::vec2 get_screen_size() const;
    void cleanup();
    float get_aspect_ratio() const;
    float get_fov_y() const;
    float get_near_z() const;
    float get_far_z() const;
    bool is_key_pressed(int key) { return key_pressed[key]; };
};