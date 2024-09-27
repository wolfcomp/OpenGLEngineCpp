#include "Shadow.h"

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "Shader.h"

void ShadowProcessor::init()
{
    glGenFramebuffers(1, &depth_map_fbo);
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowProcessor::init(int shadow_map_index, const std::string &shadow_map_name)
{
    init();
    this->shadow_map_index = shadow_map_index;
    this->shadow_map_name = shadow_map_name;
}

void ShadowProcessor::bind_buffer()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowProcessor::unbind_buffer(const glm::vec2 &window_size)
{
    glViewport(0, 0, window_size.x, window_size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowProcessor::set_shader(const Shader *shader)
{
    glActiveTexture(GL_TEXTURE0 + shadow_map_index);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    shader->set_int(shadow_map_name, shadow_map_index);
}

glm::mat4 ShadowProcessor::get_light_space_matrix(const glm::vec3 &light_pos, const glm::vec3 &light_dir)
{
    const float near_plane = 0.1f;
    const float far_plane = 10.0f;
    auto pos = light_pos - light_dir * 5.0f;
    const glm::mat4 light_view = glm::lookAt(pos, pos + light_dir, glm::vec3(0.0f, 1.0f, 0.0f));
    const auto projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    return projection * light_view;
}

glm::mat4 ShadowProcessor::get_light_space_matrix(const glm::vec3 &light_pos, const glm::vec3 &light_dir, const glm::mat4 &light_projection)
{
    const glm::mat4 light_view = glm::lookAt(light_pos, light_pos + light_dir, glm::vec3(0.0f, 1.0f, 0.0f));
    return light_projection * light_view;
}

void ShadowProcessor::cleanup()
{
    glDeleteFramebuffers(1, &depth_map_fbo);
    glDeleteTextures(1, &depth_map);
}