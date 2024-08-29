#include "InputProcessing.h"

#include "glfw/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

InputProcessing::InputProcessing()
{
    zoom = 45.0f;
    aspect = 16.0f / 9.0f;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    last = {0, 0, 0, 0};
}

void InputProcessing::change_aspect(const float width, const float height)
{
    this->width = width;
    this->height = height;
    aspect = width / height;
    projection = glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
}

void InputProcessing::set_shader(const Shader *shader)
{
    shader->set_mat4("projection", projection);
}

void InputProcessing::process_keyboard(GLFWwindow *window, const double delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    for (const auto &[key, func] : keyboard_listeners)
    {
        if (glfwGetKey(window, key) == GLFW_PRESS && (should_repeat[key] || !key_pressed[key]))
        {
            key_pressed[key] = true;
            func();
        }
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
        {
            key_pressed[key] = false;
        }
    }
}

void InputProcessing::attach_keyboard_listener(const int key, void (*event_handler)(), bool repeat)
{
    keyboard_listeners[key] = event_handler;
    should_repeat[key] = repeat;
    key_pressed[key] = false;
}

void InputProcessing::remove_keyboard_listener(const int key)
{
    keyboard_listeners.erase(key);
}

glm::mat4 InputProcessing::get_projection() const
{
    return projection;
}

int InputProcessing::attach_mouse_listener(void (*event_handler)(MouseInput input))
{
    auto i = mouse_listeners.size();
    mouse_listeners[i] = event_handler;
    return i;
}

void InputProcessing::remove_mouse_listener(int listener)
{
    mouse_listeners.erase(listener);
}

void InputProcessing::process_mouse_move(GLFWwindow *window, double x_offset, double y_offset)
{
    if (!key_pressed[GLFW_MOUSE_BUTTON_LEFT] && !key_pressed[GLFW_MOUSE_BUTTON_RIGHT])
        return;

    auto new_x_rel = x_offset - last.x_pos;
    auto new_y_rel = last.y_pos - y_offset;

    last.x_pos = x_offset;
    last.y_pos = y_offset;
    last.x_pos_offset = new_x_rel;
    last.y_pos_offset = new_y_rel;
    last.x_scroll = 0;
    last.y_scroll = 0;

    for (const auto &[key, func] : mouse_listeners)
    {
        func(last);
    }
}

void InputProcessing::process_mouse_button(GLFWwindow *window, int key, int action, int mods)
{
    if ((key == GLFW_MOUSE_BUTTON_LEFT || key == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS && !key_pressed[GLFW_MOUSE_BUTTON_LEFT] && !key_pressed[GLFW_MOUSE_BUTTON_RIGHT])
    {
        glfwGetCursorPos(window, &last.x_pos, &last.y_pos);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !key_pressed[GLFW_MOUSE_BUTTON_RIGHT])
    {
        last.x_pos_offset = 0;
        last.y_pos_offset = 0;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && !key_pressed[GLFW_MOUSE_BUTTON_LEFT])
    {
        last.x_pos_offset = 0;
        last.y_pos_offset = 0;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    key_pressed[key] = action == GLFW_PRESS;
}

void InputProcessing::process_mouse_scroll(GLFWwindow *window, double x_scroll, double y_scroll)
{
    last.x_scroll = x_scroll;
    last.y_scroll = y_scroll;
    last.x_pos_offset = 0;
    last.y_pos_offset = 0;

    for (const auto &[key, func] : mouse_listeners)
    {
        func(last);
    }
}

void InputProcessing::cleanup()
{
    keyboard_listeners.clear();
    mouse_listeners.clear();
    should_repeat.clear();
    key_pressed.clear();
}

glm::vec2 InputProcessing::get_screen_size() const
{
    return {width, height};
}