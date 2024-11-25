#pragma once
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include "AABB.h"
#include "Collider.h"
#include <math.h>

class SphereCollider : public Collider<SphereCollider>
{
public:
    float radius;

    SphereCollider() : radius(0.0f), Collider() {}
    SphereCollider(GameObject *parent) : radius(0.0f), Collider(parent) {}
    SphereCollider(GameObject *parent, float radius) : radius(radius), Collider(parent) {}
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
        return glm::distance(const_cast<SphereCollider *>(this)->get_center(), point) <= radius;
    }

    /// @brief Finds if a sphere collider intersects with another sphere collider
    /// @param other The sphere collider to check for intersection
    /// @return true if the spheres intersect, false otherwise
    bool contains(SphereCollider &other) override
    {
        glm::vec3 center1 = get_center();
        glm::vec3 center2 = other.get_center();

        // Get direction vector between centers
        glm::vec3 direction = center2 - center1;
        float distance2 = glm::length2(direction);

        // Early exit if centers are at the same point
        if (distance2 < std::numeric_limits<float>::epsilon())
        {
            return true;
        }

        // Normalize direction to get unit vector
        direction = direction / std::sqrt(distance2);

        // Use full scale values for surface-to-surface collision
        glm::vec3 radius1 = get_scale() / 2.0f;
        glm::vec3 radius2 = other.get_scale() / 2.0f;

        // Project radii onto collision direction
        float r1 = glm::abs(glm::dot(radius1, direction));
        float r2 = glm::abs(glm::dot(radius2, direction));

        // Compare squared distance with squared sum of radii
        float sumRadii = r1 + r2;
        return distance2 <= sumRadii * sumRadii;
    }

    /// @brief Finds if a sphere collider intersects with an AABB
    /// @param aabb The AABB to check for intersection
    /// @return true if the sphere intersects with the AABB, false otherwise
    template <>
    bool contains<AABB>(const AABB &aabb) const
    {
        auto distance = glm::distance(const_cast<SphereCollider *>(this)->get_center(), aabb.center);
        auto vec = const_cast<SphereCollider *>(this)->get_center() - aabb.center;
        float x = glm::max(0.0f, vec.x - aabb.extent.x);
        float y = glm::max(0.0f, vec.y - aabb.extent.y);
        float z = glm::max(0.0f, vec.z - aabb.extent.z);
        float closest = std::max(0.0f, x);
        closest = std::max(closest, y);
        closest = std::max(closest, z);
        return closest <= radius;
    }

    void update(GameObject *object) override;

    float get_radius() override { return radius; }
    glm::vec3 get_center() override;
    glm::vec3 get_scale();
    bool is_on_frustum(Frustum *frustum) override;
    bool is_on_or_forward_plane(Plane *plane);
    glm::vec3 find_furthest_point(glm::vec3 direction) override;
};