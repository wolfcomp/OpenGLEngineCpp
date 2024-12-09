#include "mesh.h"
#include "../objects/base/GameObject.h"
#include "AABB.h"
#include "SphereCollider.h"
#include "gjk.h"
#include <ranges>

bool Mesh::contains(Mesh &other)
{
    return false;
}

template <>
bool Mesh::contains<GameObject *>(GameObject *point)
{
    auto other = point->get_collider();
    if (auto aabb = dynamic_cast<AABB *>(other))
    {
        return contains(aabb);
    }
    else if (auto sphere = dynamic_cast<SphereCollider *>(other))
    {
        return contains(sphere);
    }
    return false;
}

template <>
bool Mesh::contains<AABB *>(AABB *point)
{
    auto vertices = get_parent()->get_vertices() | std::views::filter([point](auto vertex)
                                                                      { return vertex.position.x >= point->min.x && vertex.position.x <= point->max.x; }) |
                    std::views::filter([point](auto vertex)
                                       { return vertex.position.y >= point->min.y && vertex.position.y <= point->max.y; });
    for (auto vertex : vertices)
    {
        if (point->contains(vertex.position))
            return true;
    }

    return false;
}

template <>
bool Mesh::contains<SphereCollider *>(SphereCollider *point)
{
    // sphere does not work with mesh at the start to figure out the smaller surface that is being collided with so convert over to aabb to find the intersection of points
    auto aabb = AABB(point->get_center(), point->get_scale());
    if (!contains(&aabb))
        return false;

    // if aabb is inside the mesh then the sphere might be inside the mesh and start checking the points of the intersected aabb to see if the sphere is inside the mesh with gjk

    std::vector<glm::vec3> points = {};
    auto vertices = get_parent()->get_vertices();
    for (auto vertex : vertices)
    {
        if (aabb.contains(vertex.position))
            points.push_back(vertex.position);
    }

    return GJK::gjk(*point, points);
}

bool Mesh::is_on_frustum(Frustum *frustum)
{
    return false;
}

bool Mesh::is_on_or_forward_plane(Plane *plane)
{
    return false;
}

glm::vec3 Mesh::find_furthest_point(glm::vec3 direction)
{
    return glm::vec3(0.0f);
}