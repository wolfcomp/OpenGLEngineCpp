#include "Camera.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"

#define MOUSE_SENSITIVITY 0.1f
#define PITCH_LIMIT 89.9f

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0, 0)
{
}

Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch)
{
    this->position = position;
    this->worldUp = up;
    this->eulerAngles = glm::vec3(yaw, pitch, 0);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    update_camera_vectors();
}

void Camera::update_camera_vectors()
{
    // calculate the new front vector
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(eulerAngles.x)) * cos(glm::radians(eulerAngles.y));
    new_front.y = sin(glm::radians(eulerAngles.y));
    new_front.z = sin(glm::radians(eulerAngles.x)) * cos(glm::radians(eulerAngles.y));
    auto new_up = glm::vec3(0.0f, 1.0f, 0.0f);
    // new_up = rotateZ(new_up, glm::radians(eulerAngles.z));
    front = normalize(new_front);
    // also re-calculate the right and up vector
    right = normalize(cross(front, new_up));
    up = normalize(cross(right, front));
    this->viewMatrix = lookAt(this->position, this->position + this->front, this->up);
    this->inverseViewMatrix = inverse(this->viewMatrix);
}

glm::mat4 Camera::get_view_matrix() const
{
    return viewMatrix;
}

glm::mat4 Camera::get_inverse_view_matrix() const
{
    return inverseViewMatrix;
}

void Camera::set_position(const glm::vec3 position)
{
    this->position = position;
    this->viewMatrix = lookAt(this->position, this->position + this->front, this->up);
    this->inverseViewMatrix = inverse(this->viewMatrix);
}

void Camera::set_rotation(float yaw, float pitch)
{
    this->eulerAngles = glm::vec3(yaw, pitch, 0);
    update_camera_vectors();
}

glm::vec3 Camera::get_pos() const
{
    return position;
}

void Camera::set_shader(const Shader *shad)
{
    shad->set_mat4("view", viewMatrix);
    shad->set_vec3("viewPos", position);
}

glm::vec3 Camera::get_rotation() const
{
    return eulerAngles;
}

void Camera::process_mouse(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MOUSE_SENSITIVITY;
    yoffset *= MOUSE_SENSITIVITY;

    eulerAngles.x += xoffset;
    eulerAngles.y += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (eulerAngles.y > PITCH_LIMIT)
            eulerAngles.y = PITCH_LIMIT;
        if (eulerAngles.y < -PITCH_LIMIT)
            eulerAngles.y = -PITCH_LIMIT;
    }

    if (eulerAngles.x > 180.0f)
        eulerAngles.x -= 360.0f;
    if (eulerAngles.x < -180.0f)
        eulerAngles.x += 360.0f;

    // update front, right and up vectors using the updated Euler angles
    update_camera_vectors();
}

glm::vec3 Camera::get_movement(glm::vec3 direction) const
{
    return glm::vec3(glm::vec4(direction, 1) * viewMatrix);
}

glm::vec3 Camera::get_front() const
{
    return front;
}