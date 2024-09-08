#pragma once

#include <glm/vec3.hpp>
#include "Collider.h"

namespace ColliderHandler
{
    template <class TA, class TB>
    bool contains(const TA &a, const TB &b)
    {
        auto t = a.contains(b);
        if (t)
        {
            for (auto &callback : a.onCollisionCallbacks)
            {
                callback(b);
            }
            for (auto &callback : b.onCollisionCallbacks)
            {
                callback(a);
            }
        }
        return t;
    }

    /// @brief Gets the collision normal between two colliders
    /// @tparam TA The type of the first collider
    /// @tparam TB The type of the second collider
    /// @param a The collider to get the normal of
    /// @param b the collider that is colliding with a
    /// @return
    template <class TA, class TB>
    glm::vec3 get_collision_normal(TA &a, TB &b)
    {
        throw "Not implemented";
    }

    template <class TA, class TB>
    glm::vec3 get_collision_normal(Collider<TA> &a, Collider<TB> &b);

    template <class TA, class TB>
    bool intersects(const TA &a, const TB &b)
    {
        return contains(a, b);
    }
};