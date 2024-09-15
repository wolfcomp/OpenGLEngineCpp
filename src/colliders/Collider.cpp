#include "Collider.h"
#include "SphereCollider.h"
#include "../objects/base/SceneUpdatableObject.h"

template <>
float ColliderBase::collision_delta<SphereCollider>(SphereCollider *collider, float delta_time)
{
    if (dynamic_cast<SphereCollider *>(this) == nullptr)
        return -1;
    auto th = dynamic_cast<SphereCollider *>(this);
    auto v1 = dynamic_cast<SceneUpdatableObject *>(th->get_parent())->get_velocity();
    auto v2 = dynamic_cast<SceneUpdatableObject *>(collider->get_parent())->get_velocity();
    auto p1 = th->get_center();
    auto p2 = collider->get_center();
    auto p0 = p1 - v1;
    auto q0 = p2 - v2;
    auto A = p0 - q0;
    auto B = v1 - v2;
    auto AB = glm::dot(A, B);
    auto AA = glm::dot(A, A);
    auto BB = glm::dot(B, B);
    auto d = th->radius + collider->radius;
    auto rot = AB * AB - BB * (AA - d * d);
    float t = -1;
    if (rot >= 0)
        t = -AB - sqrtf(rot);
    if (BB > glm::epsilon<float>())
        t /= BB;
    return t;
}

template <>
float ColliderBase::collision_delta<ColliderBase>(ColliderBase *collider, float delta_time)
{
    return collision_delta(dynamic_cast<SphereCollider *>(collider), delta_time);
}

template <typename T>
float ColliderBase::collision_delta(T *collider, float delta_time)
{
    return -1;
}