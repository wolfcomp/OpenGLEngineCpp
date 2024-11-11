#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "base.h"
#include "../ecs_map.h"

struct TransformComponent : public BaseComponent
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

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

    /// @brief Constructs a new TransformComponent object
    /// @param position The position of the object
    /// @param rotation The rotation of the object
    /// @param scale The scale of the object
    TransformComponent(glm::vec3 position, glm::quat rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {}

    /// @brief Sets the position of the object
    /// @param position The position of the object
    void set_position(glm::vec3 position) { this->position = position; }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in quaternion
    void set_rotation(glm::quat rotation) { this->rotation = rotation; }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in euler angles
    void set_rotation(glm::vec3 rotation) { this->rotation = glm::quat(rotation); }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in normalized vector form
    void set_rotation_world_up(glm::vec3 rotation) { this->rotation = glm::quat(glm::normalize(rotation), glm::vec3(0, 1, 0)); }
    /// @brief Sets the scale of the object
    /// @param scale The scale of the object
    void set_scale(glm::vec3 scale) { this->scale = scale; }
    /// @brief Gets the position of the object
    /// @return The position of the object
    glm::vec3 get_position() { return position; }
    /// @brief Gets the rotation of the object
    /// @return The rotation of the object
    glm::quat get_rotation() { return rotation; }
    /// @brief Gets the scale of the object
    /// @return The scale of the object
    glm::vec3 get_scale() { return scale; }
    /// @brief Translates the object by a certain amount
    /// @param translation The amount to translate the object by
    void translate(glm::vec3 translation) { this->position += translation; }
    /// @brief Rotates the object by a certain amount
    /// @param rotation The amount to rotate the object by
    void rotate(glm::quat rotation) { this->rotation = rotation * this->rotation; }
    /// @brief Scales the object by a certain amount
    /// @param scale The amount to scale the object by
    void scale_by(glm::vec3 scale) { this->scale *= scale; }
};