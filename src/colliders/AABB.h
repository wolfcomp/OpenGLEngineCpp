#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <vector>
#include "../objects/base/Vertex.h"
#include "Collider.h"

class SceneObject;
class SceneUpdatableObject;
class Line;

struct AABB : public Collider
{
    glm::vec3 center;
    glm::vec3 extent;

    AABB(glm::vec3 center, glm::vec3 extent);
    AABB();
    ~AABB();

    void draw_debug(Line *line);

    template <typename T>
    bool contains(const T &point) const
    {
        std::string message = "Not implemented on type of ";
        throw message + typeid(T).name();
    }

    template <>
    bool contains<glm::vec3>(const glm::vec3 &point) const
    {
        return (center.x - extent.x <= point.x &&
                center.x + extent.x >= point.x &&
                center.y - extent.y <= point.y &&
                center.y + extent.y >= point.y &&
                center.z - extent.z <= point.z &&
                center.z + extent.z >= point.z);
    }

    template <>
    bool contains<AABB>(const AABB &other) const
    {
        return (center.x - extent.x <= other.center.x + other.extent.x &&
                center.x + extent.x >= other.center.x - other.extent.x &&
                center.y - extent.y <= other.center.y + other.extent.y &&
                center.y + extent.y >= other.center.y - other.extent.y &&
                center.z - extent.z <= other.center.z + other.extent.z &&
                center.z + extent.z >= other.center.z - other.extent.z);
    }

    template <>
    bool contains<SceneObject *>(SceneObject *const &point) const;

    template <>
    bool contains<SceneUpdatableObject *>(SceneUpdatableObject *const &point) const;
};
