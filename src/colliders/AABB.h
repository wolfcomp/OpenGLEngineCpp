#pragma once
#include <glm/vec3.hpp>

struct AABB
{
    glm::vec3 center;
    glm::vec3 extent;

    AABB(glm::vec3 center, glm::vec3 extent) : center(center), extent(extent) {}
    AABB() : center(glm::vec3(0.0f)), extent(glm::vec3(0.0f)) {}
    ~AABB() {}

    bool intersects(const AABB &other) const
    {
        return (center.x - extent.x <= other.center.x + other.extent.x &&
                center.x + extent.x >= other.center.x - other.extent.x &&
                center.y - extent.y <= other.center.y + other.extent.y &&
                center.y + extent.y >= other.center.y - other.extent.y &&
                center.z - extent.z <= other.center.z + other.extent.z &&
                center.z + extent.z >= other.center.z - other.extent.z);
    }

    bool contains(const glm::vec3 &point) const
    {
        return (center.x - extent.x <= point.x &&
                center.x + extent.x >= point.x &&
                center.y - extent.y <= point.y &&
                center.y + extent.y >= point.y &&
                center.z - extent.z <= point.z &&
                center.z + extent.z >= point.z);
    }
};

template <typename T>
bool checkBoundry(AABB bounds, T object)
{
    throw "Not implemented on type of" + typeid(T).name();
}

template <>
bool checkBoundry(AABB bounds, glm::vec3 object)
{
    return bounds.contains(object);
}
