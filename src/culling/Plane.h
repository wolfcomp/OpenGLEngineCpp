#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

struct Plane
{
    glm::vec3 normal = {0, 1, 0};
    float distance;

    Plane() = default;

    Plane(const glm::vec3 &p1, const glm::vec3 &norm)
        : normal(glm::normalize(norm)),
          distance(glm::dot(normal, p1))
    {
    }

    float getSignedDistanceToPlane(const glm::vec3 &point) const
    {
        return glm::dot(normal, point) - distance;
    }
};