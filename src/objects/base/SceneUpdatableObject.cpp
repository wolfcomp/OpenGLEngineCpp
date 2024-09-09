#include "SceneUpdatableObject.h"
#include <glm/geometric.hpp>

void Physics::apply_collision(Physics *other)
{
    auto m1 = mass;
    auto m2 = other->get_mass();
    auto v1 = velocity;
    auto v2 = other->get_velocity();
    auto p1 = parent->get_position();
    auto p2 = other->parent->get_position();

    auto n = glm::normalize(p2 - p1);
    auto mEff = 1 / (1 / m1 + 1 / m2);
    auto vImp = n * dot(v1 - v2, v1 - v2);
    constexpr auto e = 1.0f;
    auto j = (1 + e) * mEff * vImp;
    auto vd1 = -j / m1 * n;
    auto vd2 = j / m2 * n;
    auto new_v1 = v1 + vd1;
    auto new_v2 = v2 + vd2;

    set_velocity(new_v1);
    other->set_velocity(new_v2);
}