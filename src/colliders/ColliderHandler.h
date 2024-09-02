#pragma once

#include "AABB.h"
#include "ColliderHandler.h"
#include "SphereCollider.h"

namespace ColliderHandler
{
    template <typename TA, typename TB>
    bool contains(const TA &a, const TB &b)
    {
        return a.contains(b);
    }

    template <typename TA, typename TB>
    glm::vec3 get_collision_normal(const TA &a, const TB &b)
    {
        throw "Not implemented";
    }

    template <typename TA, typename TB>
    bool intersects(const TA &a, const TB &b)
    {
        return contains(a, b);
    }
}