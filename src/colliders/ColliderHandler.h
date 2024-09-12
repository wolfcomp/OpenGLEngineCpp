#pragma once

#include <glm/vec3.hpp>
#include <detail/typename.h>
#include "Collider.h"

struct CollisionType
{
    bool is_colliding;
    /// @brief The normal of the collision from the perspective of collider a to collider b
    glm::vec3 normal;
    ColliderBase *a;
    ColliderBase *b;
};

namespace ColliderHandler
{
    template <class TA, class TB>
    bool contains(const Collider<TA> &a, const Collider<TB> &b)
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

    bool contains(ColliderBase *a, ColliderBase *b);

    /// @brief Gets the collision normal between two colliders
    /// @param a The collider to get the normal of
    /// @param b the collider that is colliding with a
    /// @return
    glm::vec3 get_collision_normal(ColliderBase *a, ColliderBase *b);

    template <class TA, class TB>
    bool intersects(const Collider<TA> &a, const Collider<TB> &b)
    {
        return contains(a, b);
    }

    CollisionType get_collision_type(ColliderBase *a, ColliderBase *b);
};