#pragma once

struct GLFWwindow;

struct Window
{
    GLFWwindow *window;
    float *fpsPtr;

    ~Window();

    int init();
    void init_listeners();
    // static void create_objects();
    void update() const;
    void render() const;
    bool should_close() const;
};