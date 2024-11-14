#include "mesh.h"
#include "../objects/base/GameObject.h"
#include "AABB.h"
#include "SphereCollider.h"

Mesh::Mesh() : Collider<Mesh>() {}

bool Mesh::contains(const Mesh &other) const
{
    return false;
}

template <>
bool Mesh::contains<GameObject *>(GameObject *const &point) const
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