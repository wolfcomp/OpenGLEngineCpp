#pragma once

#include <glm/fwd.hpp>
#include <string>

class Shader;

class ShadowProcessor
{
    unsigned int depth_map_fbo;
    unsigned int depth_map;

    const unsigned int SHADOW_WIDTH = 2048;
    const unsigned int SHADOW_HEIGHT = 2048;

    int shadow_map_index = 15;
    std::string shadow_map_name = "shadowMap";

public:
    ShadowProcessor() = default;
    ShadowProcessor(const ShadowProcessor &) = delete;
    ShadowProcessor(ShadowProcessor &&) = delete;
    ShadowProcessor &operator=(const ShadowProcessor &) = delete;
    ShadowProcessor &operator=(ShadowProcessor &&) = delete;

    void init();
    void init(int shadow_map_index, const std::string &shadow_map_name);
    void bind_buffer();
    void unbind_buffer(const glm::vec2 &window_size);
    void set_shader(const Shader *shader);
    glm::mat4 get_light_space_matrix(const glm::vec3 &light_pos, const glm::vec3 &light_dir);
    glm::mat4 get_light_space_matrix(const glm::vec3 &light_pos, const glm::vec3 &light_dir, const glm::mat4 &light_projection);
    void cleanup();
};