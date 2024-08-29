#pragma once

#include "Renderable.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AABB.h"

class SceneObject : public Renderable
{
private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

public:
    SceneObject(std::vector<Vertex> vertices, std::vector<unsigned> indices) : Renderable(vertices, indices) {}
    ~SceneObject() {}

    void set_position(glm::vec3 position) { this->position = position; }
    void set_rotation(glm::quat rotation) { this->rotation = rotation; }
    void set_scale(glm::vec3 scale) { this->scale = scale; }
    glm::vec3 get_position() const { return position; }
    glm::quat get_rotation() const { return rotation; }
    glm::vec3 get_scale() const { return scale; }

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

template <>
bool checkBoundry(AABB bounds, SceneObject object)
{
    return bounds.contains(object.get_position());
}