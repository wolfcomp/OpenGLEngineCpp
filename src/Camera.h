#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Shader.h"

class Camera
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::vec3 eulerAngles;

    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;

    void update_camera_vectors();

public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    void set_position(glm::vec3 position);
    void set_rotation(float yaw, float pitch);
    glm::mat4 get_view_matrix() const;
    glm::mat4 get_inverse_view_matrix() const;
    glm::vec3 get_pos() const;
    glm::vec3 get_rotation() const;
    glm::vec3 get_movement(glm::vec3 direction) const;
    glm::vec3 get_front() const;
    void set_shader(const Shader *shad);
    void process_mouse(float xoffset, float yoffset, bool constrainPitch = true);
};
