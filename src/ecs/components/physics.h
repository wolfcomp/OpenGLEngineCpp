#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/simd/common.h>
#include <immintrin.h>
#include "base.h"
#include "../ecs_map.h"

struct PhysicsComponent : public BaseComponent
{
    alignas(16) glm::vec3 velocity = glm::vec3(0);     // 16 byte alignment due to SIMD instructions
    alignas(16) glm::vec3 acceleration = glm::vec3(0); // 16 byte alignment due to SIMD instructions
    float mass = 1.0f;
    float dragCoefficient = 0.1f;
    static float restitution;

    void update(float delta_time)
    {
        apply_drag(dragCoefficient);
        velocity += acceleration * delta_time;
    }

    void apply_force(glm::vec3 force)
    {
        acceleration += force / mass;
    }

    void apply_force_no_mass(glm::vec3 force)
    {
        acceleration += force;
    }

    void apply_impulse(glm::vec3 impulse)
    {
        velocity += impulse / mass;
    }

    void apply_impulse_no_mass(glm::vec3 impulse)
    {
        velocity += impulse;
    }

    void apply_friction(float friction)
    {
        auto friction_force = velocity * friction;
        apply_force(-friction_force);
    }

    void apply_gravity(glm::vec3 gravity)
    {
        apply_force(gravity);
    }

    void apply_gravity(float gravity)
    {
        apply_force(glm::vec3(0, -gravity * mass, 0));
    }

    void apply_drag(glm::vec3 drag)
    {
        auto drag_force = velocity * drag;
        apply_force(-drag_force);
    }

    void apply_drag(float drag)
    {
        auto drag_force = velocity * drag;
        if (isnan(drag_force.x) || isnan(drag_force.y) || isnan(drag_force.z))
            return;
        apply_impulse(-drag_force);
        apply_force(-drag_force);
    }

    void apply_collision(PhysicsComponent *other, glm::vec3 normal);
    void apply_collision(glm::vec3 normal);
};

template <>
void ECSGlobalMap::insert<PhysicsComponent>(UUID id, PhysicsComponent *value);