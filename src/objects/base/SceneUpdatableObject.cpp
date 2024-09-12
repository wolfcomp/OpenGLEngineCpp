#include "SceneUpdatableObject.h"
#include <glm/geometric.hpp>

#include "../../colliders/ColliderHandler.h"
#include "../primitives/IcoSphere.h"

/// @brief gets the collision vector from a and b where b is static
/// @param a the object to get a new vector from
/// @param b the object normal to retreive the new vector from
glm::vec3 get_new_collision_vector(const glm::vec3 a, const glm::vec3 b)
{
    auto w = glm::cross(a, b) / sqrtf(powf(glm::length(a) * glm::length(b), 2));
    auto theta = acos(glm::dot(a, b) / (glm::length(a) * glm::length(b)));
    auto q = glm::rotate(glm::mat4(1.0f), theta, w);
    return {q * glm::vec4(b, 1.0f)};
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

        auto n = (p1 - p2) / glm::length(p1 - p2);
        auto vr = v1 - v2;
        auto vn = (vr * n) * n;

        // auto n = normalize(p2 - p1);
        // auto mEff = 1 / (1 / m1 + 1 / m2);
        // auto vImp = n * dot(glm::normalize(v1 - v2), glm::normalize(v1 - v2));
        // auto j = (1 + restitution) * mEff * vImp;
        // auto vd1 = -j / m1 * n;
        // auto vd2 = j / m2 * n;
        // auto v1n = v1 + vd1;
        // auto v2n = v2 + vd2;

        auto v1n = (m1 - m2) / (m1 + m2) * v1 + (2 * m2) / (m1 + m2) * v2;
        auto v2n = (m2 - m1) / (m1 + m2) * v1 + (2 * m1) / (m1 + m2) * v2;
        new_velocity = std::make_tuple(v1n - vn, v2n + vn);
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
    v = get_new_collision_vector(v, normal) * length(v);
    set_velocity(v);
}