#pragma once
#include "Shader.h"

class ShaderStore
{
public:
    static Shader *get_shader(unsigned id);
    static Shader *get_shader(std::string name);
    static Shader *add_shader(const std::string &name, const char *vertex_path, const char *fragment_path);
    static void remove_shader(unsigned id);
    static void remove_all_shaders();
    static void add_params_callback(void func(const Shader *));
    static void load_shaders();
};
