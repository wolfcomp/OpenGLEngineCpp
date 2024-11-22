#include "mesh.h"
#include "../objects/base/GameObject.h"
#include "AABB.h"
#include "SphereCollider.h"
#include "gjk.h"

Mesh::Mesh() : Collider<Mesh>() {}

bool Mesh::contains(const Mesh &other) const
{
    return false;
}

template <>
bool Mesh::contains<GameObject *>(GameObject *const &point)
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
bool Mesh::contains<AABB *>(AABB *const &point)
{
    auto vertices = get_parent()->get_vertices();
    for (auto vertex : vertices)
    {
        if (point->contains(vertex.position))
            return true;
    }

    return false;
}

template <>
bool Mesh::contains<SphereCollider *>(SphereCollider *const &point)
{
    // sphere does not work with mesh at the start to figure out the smaller surface that is being collided with so convert over to aabb to find the intersection of points
    auto aabb = AABB();
    aabb.center = point->get_center();
    aabb.extent = glm::vec3(point->radius);
    if (!contains(aabb))
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

void Mesh::update(GameObject *object)
{
}

bool Mesh::is_on_frustum(Frustum *frustum)
{
    return false;
}

bool Mesh::is_on_or_forward_plane(Plane *plane)
{
    return false;
}