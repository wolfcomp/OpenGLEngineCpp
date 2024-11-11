#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "base.h"
#include "../ecs_map.h"

struct PhysicsComponent : public BaseComponent
{
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
    float dragCoefficient = 0.1f;

    void update(float delta_time)
    {
        apply_drag(dragCoefficient);
        velocity += acceleration * delta_time;
    }

    void apply_force(glm::vec3 force)
    {
        acceleration += force / mass;
    }

    void apply_impulse(glm::vec3 impulse)
    {
        velocity += impulse / mass;
    }

    void apply_friction(float friction)
    {
        auto friction_force = glm::normalize(velocity) * friction;
        apply_force(-friction_force);
    }

    void apply_gravity(glm::vec3 gravity)
    {
        apply_force(gravity);
    }

    void apply_gravity(float gravity)
    {
        apply_force(glm::vec3(0, -gravity, 0));
    }

    void apply_drag(glm::vec3 drag)
    {
        auto drag_force = glm::normalize(velocity) * drag;
        apply_force(-drag_force);
    }

    void apply_drag(float drag)
    {
        auto drag_force = glm::normalize(velocity) * drag;
        apply_force(-drag_force);
    }

    void apply_collision(PhysicsComponent *other);
    void apply_collision(glm::vec3 normal);
};

template <>
void ECSGlobalMap::insert<PhysicsComponent>(UUID id, PhysicsComponent *value);