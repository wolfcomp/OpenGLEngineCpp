#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;

    glm::mat4x4 get_model_matrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = model * glm::mat4_cast(rotation);
        model = glm::scale(model, scale);
        return model;
    }

    glm::mat4x4 get_world_matrix(glm::mat4x4 parent) const
    {
        return parent * get_model_matrix();
    }
};