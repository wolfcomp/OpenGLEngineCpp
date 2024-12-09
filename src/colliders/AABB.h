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
    glm::vec3 min;
    glm::vec3 max;

    AABB(glm::vec3 center, glm::vec3 extent);
    AABB();
    ~AABB();

    void draw_debug(Line *line);
    void recalculate()
    {
        min = center - extent;
        max = center + extent;
    }

    template <typename T>
    bool contains(const T &point) const
    {
        std::string message = "Not implemented on type of ";
        throw message + typeid(T).name();
    }

    template <>
    bool contains<glm::vec3>(const glm::vec3 &point) const
    {
        return (min.x <= point.x &&
                max.x >= point.x &&
                min.y <= point.y &&
                max.y >= point.y &&
                min.z <= point.z &&
                max.z >= point.z);
    }

    bool contains(AABB &other) override
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

    bool is_on_frustum(Frustum *frustum) override;

    bool is_on_or_forward_plane(Plane *plane);

    glm::vec3 find_furthest_point(glm::vec3 direction) override;
};
