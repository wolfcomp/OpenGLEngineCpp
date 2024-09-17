#include "SceneUpdatableObject.h"
#include <glm/geometric.hpp>

#include "../../colliders/ColliderHandler.h"
#include "../primitives/IcoSphere.h"

/// @brief gets the collision vector from a and b where b is static
/// @param a the object to get a new vector from
/// @param b the object normal to retreive the new vector from
glm::vec3 get_new_collision_vector(const glm::vec3 a, const glm::vec3 b)
{
    auto angle = glm::acos(glm::dot(a, b));
    auto axis = glm::normalize(glm::cross(a, b));
    auto axisNan = glm::isnan(axis);
    if (axisNan.x || axisNan.y || axisNan.z)
    {
        // a and b are parallel and therefor the new vector should be the negative of a
        return -a;
    }
    auto q = glm::angleAxis(angle, axis);
    return glm::normalize(q * -b);
}

constexpr float restitution = 1.0f;

std::tuple<glm::vec3, glm::vec3> get_new_velocity(Physics *a, Physics *b)
{
    auto pa = a->get_parent()->get_collider();
    auto pb = b->get_parent()->get_collider();
    std::tuple<glm::vec3, glm::vec3> new_velocity;

    if (pa->get_channel() == STATIC)
    {
        // a is a static object and therefor shall not have a velocity
        auto n = ColliderHandler::get_collision_normal(pa, pb);
        auto vb = b->get_velocity();
        vb = get_new_collision_vector(vb, n) * length(vb);
        new_velocity = std::make_tuple(glm::vec3(0, 0, 0), vb);
    }
    else if (pb->get_channel() == STATIC)
    {
        // b is a static object and therefor shall not have a velocity
        auto n = ColliderHandler::get_collision_normal(pa, pb);
        auto va = a->get_velocity();
        va = get_new_collision_vector(va, n) * length(va);
        new_velocity = std::make_tuple(va, glm::vec3(0, 0, 0));
    }
    else
    {
        // both objects are dynamic and shall have velocity
        auto m1 = a->get_mass();
        auto m2 = b->get_mass();
        auto v1 = a->get_velocity();
        auto v2 = b->get_velocity();
        auto p1 = a->get_parent()->get_position();
        auto p2 = b->get_parent()->get_position();

        auto v1n = v1 - (m2 * 2 / (m1 + m2)) * (dot(v1 - v2, p1 - p2) / powf(length(p1 - p2), 2)) * (p1 - p2);
        auto v2n = v2 - (m1 * 2 / (m2 + m1)) * (dot(v2 - v1, p2 - p1) / powf(length(p2 - p1), 2)) * (p2 - p1);
        new_velocity = std::make_tuple(v1n, v2n);
    }

    return new_velocity;
}

void Physics::apply_collision(Physics *other)
{
    const auto t = get_new_velocity(this, other);
    set_velocity(std::get<0>(t));
    other->set_velocity(std::get<1>(t));
}

void Physics::apply_collision(glm::vec3 normal)
{
    auto v = get_velocity();
    v = get_new_collision_vector(normalize(v), normal) * length(v);
    set_velocity(v);
}