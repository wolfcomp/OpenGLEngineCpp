#include "ColliderHandler.h"
#include "AABB.h"
#include "ConvexHull.h"
#include "SphereCollider.h"

static glm::vec3 get_collision_normal(AABB *a, AABB *b)
{
    glm::vec3 normal = b->center - a->center;
    float x_overlap = a->extent.x + b->extent.x - abs(normal.x);
    float y_overlap = a->extent.y + b->extent.y - abs(normal.y);
    float z_overlap = a->extent.z + b->extent.z - abs(normal.z);
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

static glm::vec3 get_collision_normal(AABB *a, SphereCollider *b)
{
    glm::vec3 closest = glm::vec3(
        glm::clamp(b->center.x, a->center.x - a->extent.x, a->center.x + a->extent.x),
        glm::clamp(b->center.y, a->center.y - a->extent.y, a->center.y + a->extent.y),
        glm::clamp(b->center.z, a->center.z - a->extent.z, a->center.z + a->extent.z));
    glm::vec3 normal = b->center - closest;
    if (glm::length(normal) < b->radius)
    {
        return glm::normalize(normal);
    }
    return glm::vec3(0);
}

static glm::vec3 get_collision_normal(SphereCollider *a, AABB *b)
{
    return -get_collision_normal(b, a);
}

static glm::vec3 get_collision_normal(SphereCollider *a, SphereCollider *b)
{
    glm::vec3 normal = a->center - b->center;
    float distance = glm::length(normal);
    if (distance < a->radius + b->radius)
    {
        return glm::normalize(normal);
    }
    return glm::normalize(normal);
}

glm::vec3 ColliderHandler::get_collision_normal(ColliderBase *a, ColliderBase *b)
{
    auto sca = dynamic_cast<SphereCollider *>(a);
    auto scb = dynamic_cast<SphereCollider *>(b);
    auto aba = dynamic_cast<AABB *>(a);
    auto abb = dynamic_cast<AABB *>(b);
    if (sca != nullptr && scb != nullptr)
    {
        return get_collision_normal(sca, scb);
    }
    else if (sca != nullptr && abb != nullptr)
    {
        return get_collision_normal(sca, abb);
    }
    else if (aba != nullptr && scb != nullptr)
    {
        return get_collision_normal(aba, scb);
    }
    else if (aba != nullptr && abb != nullptr)
    {
        return get_collision_normal(aba, abb);
    }
    auto at = type_name<decltype(a)>();
    auto bt = type_name<decltype(b)>();
    throw;
}

CollisionType ColliderHandler::get_collision_type(ColliderBase *a, ColliderBase *b)
{
    auto sca = dynamic_cast<SphereCollider *>(a);
    auto scb = dynamic_cast<SphereCollider *>(b);
    auto aba = dynamic_cast<AABB *>(a);
    auto abb = dynamic_cast<AABB *>(b);
    if (sca != nullptr && scb != nullptr)
    {
        return {sca->contains(*scb), get_collision_normal(sca, scb), a, b};
    }
    else if (sca != nullptr && abb != nullptr)
    {
        return {sca->contains(*abb), get_collision_normal(sca, abb), a, b};
    }
    else if (aba != nullptr && scb != nullptr)
    {
        return {aba->contains(*scb), get_collision_normal(aba, scb), a, b};
    }
    else if (aba != nullptr && abb != nullptr)
    {
        return {aba->contains(*abb), get_collision_normal(aba, abb), a, b};
    }
    auto at = type_name<decltype(a)>();
    auto bt = type_name<decltype(b)>();
    throw;
}

bool ColliderHandler::contains(ColliderBase *a, ColliderBase *b)
{
    auto sca = dynamic_cast<SphereCollider *>(a);
    auto scb = dynamic_cast<SphereCollider *>(b);
    auto aba = dynamic_cast<AABB *>(a);
    auto abb = dynamic_cast<AABB *>(b);
    if (sca != nullptr && scb != nullptr)
        return sca->contains(*scb);
    else if (sca != nullptr && abb != nullptr)
        return sca->contains(*abb);
    else if (aba != nullptr && scb != nullptr)
        return aba->contains(*scb);
    else if (aba != nullptr && abb != nullptr)
        return aba->contains(*abb);
    return false;
}
