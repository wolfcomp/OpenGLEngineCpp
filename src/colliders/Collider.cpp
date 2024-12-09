#include "Collider.h"
#include "SphereCollider.h"
#include "../objects/base/GameObject.h"

template <>
inline float ColliderBase::collision_delta<SphereCollider>(SphereCollider *collider, float delta_time)
{
    if (dynamic_cast<SphereCollider *>(this) == nullptr)
        return -1;
    auto th = dynamic_cast<SphereCollider *>(this);
    auto r1 = th->get_radius();
    auto r2 = collider->get_radius();
    auto p1 = th->get_center();
    auto p2 = collider->get_center();
    auto r = r1 + r2;
    auto d2 = glm::distance2(p1, p2);
    auto r2d2 = r * r - d2;
    if (r2d2 > 0)
    {
        auto d = glm::sqrt(d2);
        auto t = (r - d) / d;
        return t;
    }
    return -1;
}

template <>
inline float ColliderBase::collision_delta<ColliderBase>(ColliderBase *collider, float delta_time)
{
    return collision_delta(dynamic_cast<SphereCollider *>(collider), delta_time);
}

template <typename T>
inline float ColliderBase::collision_delta(T *collider, float delta_time)
{
    return -1;
}