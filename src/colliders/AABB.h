#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <vector>
#include "../objects/base/Vertex.h"
#include "Collider.h"

class Line;

class AABB : public Collider<AABB>
{
public:
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

    bool contains(const AABB &other) const override
    {
        return (center.x - extent.x <= other.center.x + other.extent.x &&
                center.x + extent.x >= other.center.x - other.extent.x &&
                center.y - extent.y <= other.center.y + other.extent.y &&
                center.y + extent.y >= other.center.y - other.extent.y &&
                center.z - extent.z <= other.center.z + other.extent.z &&
                center.z + extent.z >= other.center.z - other.extent.z);
    }

    template <>
    bool contains<GameObject *>(GameObject *const &point) const;

    void update(GameObject *object) override;

    bool is_on_frustum(Frustum *frustum) override;

    bool is_on_or_forward_plane(Plane *plane);

    glm::vec3 find_furthest_point(glm::vec3 direction) override;
};
