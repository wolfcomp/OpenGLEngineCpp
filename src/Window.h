#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Window
{
    static inline GLFWwindow *glfWindow = nullptr;

    ~Window();

    int init();
    void init_listeners();
    // static void create_objects();
    void update() const;
    void render() const;
    bool should_close() const;
};