#include "ColliderHandler.h"
#include "AABB.h"
#include "ConvexHull.h"
#include "SphereCollider.h"

template <>
glm::vec3 ColliderHandler::get_collision_normal(const AABB &a, const AABB &b)
{
    glm::vec3 normal = b.center - a.center;
    float x_overlap = a.extent.x + b.extent.x - abs(normal.x);
    float y_overlap = a.extent.y + b.extent.y - abs(normal.y);
    float z_overlap = a.extent.z + b.extent.z - abs(normal.z);
    if (x_overlap < y_overlap && x_overlap < z_overlap)
    {
        normal.y = 0;
        normal.z = 0;
    }
    else if (y_overlap < x_overlap && y_overlap < z_overlap)
    {
        normal.x = 0;
        normal.z = 0;
    }
    else
    {
        normal.x = 0;
        normal.y = 0;
    }
    return glm::normalize(normal);
}

template <>
glm::vec3 ColliderHandler::get_collision_normal(const AABB &a, const SphereCollider &b)
{
    glm::vec3 closest = glm::vec3(
        glm::clamp(b.center.x, a.center.x - a.extent.x, a.center.x + a.extent.x),
        glm::clamp(b.center.y, a.center.y - a.extent.y, a.center.y + a.extent.y),
        glm::clamp(b.center.z, a.center.z - a.extent.z, a.center.z + a.extent.z));
    glm::vec3 normal = b.center - closest;
    if (glm::length(normal) < b.radius)
    {
        return glm::normalize(normal);
    }
    return glm::vec3(0);
}

template <>
glm::vec3 ColliderHandler::get_collision_normal(const SphereCollider &a, const AABB &b)
{
    return -get_collision_normal(b, a);
}

template <>
glm::vec3 ColliderHandler::get_collision_normal(const SphereCollider &a, const SphereCollider &b)
{
    glm::vec3 normal = b.center - a.center;
    float distance = glm::length(normal);
    if (distance < a.radius + b.radius)
    {
        return glm::normalize(normal);
    }
    return glm::vec3(0);
}