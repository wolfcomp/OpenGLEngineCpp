#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "AABB.h"
#include "Collider.h"
#include <math.h>

struct SphereCollider : public Collider
{
    glm::vec3 center;
    float radius;

    SphereCollider(glm::vec3 center, float radius) : center(center), radius(radius) {}
    ~SphereCollider() {}

    /// @brief Fallback for unsupported types
    /// @tparam T
    /// @param point The point to check for intersection
    /// @return throw error with a message
    template <typename T>
    bool contains(const T &point) const
    {
        std::string message = "Not implemented on type of ";
        throw message + typeid(T).name();
    }

    /// @brief Finds if a sphere collider intersects with a point
    /// @param point The point to check for intersection
    /// @return true if the sphere contains the point, false otherwise
    template <>
    bool contains<glm::vec3>(const glm::vec3 &point) const
    {
        return glm::distance(center, point) <= radius;
    }

    /// @brief Finds if a sphere collider intersects with another sphere collider
    /// @param other The sphere collider to check for intersection
    /// @return true if the spheres intersect, false otherwise
    template <>
    bool contains<SphereCollider>(const SphereCollider &other) const
    {
        return glm::distance(center, other.center) <= radius + other.radius;
    }

    /// @brief Finds if a sphere collider intersects with an AABB
    /// @param aabb The AABB to check for intersection
    /// @return true if the sphere intersects with the AABB, false otherwise
    template <>
    bool contains<AABB>(const AABB &aabb) const
    {
        auto distance = glm::distance(center, aabb.center);
        auto vec = center - aabb.center;
        float x = glm::max(0.0f, vec.x - aabb.extent.x);
        float y = glm::max(0.0f, vec.y - aabb.extent.y);
        float z = glm::max(0.0f, vec.z - aabb.extent.z);
        float closest = std::max(0.0f, x);
        closest = std::max(closest, y);
        closest = std::max(closest, z);
        return closest <= radius;
    }
};