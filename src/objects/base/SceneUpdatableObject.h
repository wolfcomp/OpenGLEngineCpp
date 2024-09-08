#pragma once

#include "SceneObject.h"

class SceneUpdatableObject;

class Physics
{
private:
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
    SceneUpdatableObject *parent;

public:
    Physics(glm::vec3 velocity, glm::vec3 acceleration, float mass) : velocity(velocity), acceleration(acceleration), mass(mass) {}
    ~Physics() {}

    void set_velocity(glm::vec3 velocity) { this->velocity = velocity; }
    void set_acceleration(glm::vec3 acceleration) { this->acceleration = acceleration; }
    void set_mass(float mass) { this->mass = mass; }

    glm::vec3 get_velocity() const { return velocity; }
    glm::vec3 get_acceleration() const { return acceleration; }
    float get_mass() const { return mass; }

    void update(float delta_time)
    {
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

    void apply_collision(Physics *other);
};

class SceneUpdatableObject : public SceneObject
{
private:
    bool is_active;
    bool has_updated;
    Physics physics;

public:
    SceneUpdatableObject(std::vector<Vertex> vertices, std::vector<unsigned> indices) : SceneObject(vertices, indices), is_active(true), physics(Physics(glm::vec3(0), glm::vec3(0), 0.0f)) {}
    ~SceneUpdatableObject() {}

    void set_active(bool is_active) { this->is_active = is_active; }
    void set_velocity(glm::vec3 velocity) { physics.set_velocity(velocity); }
    void set_acceleration(glm::vec3 acceleration) { physics.set_acceleration(acceleration); }
    void set_mass(float mass) { physics.set_mass(mass); }

    bool get_active() const { return is_active; }
    bool get_has_updated() const { return has_updated; }
    glm::vec3 get_velocity() const { return physics.get_velocity(); }
    glm::vec3 get_acceleration() const { return physics.get_acceleration(); }
    float get_mass() const { return physics.get_mass(); }

    virtual void setup() {};
    /// @brief Handles any pre-update logic before the object moves
    /// @param delta_time The time since the last frame
    virtual void pre_update(float delta_time)
    {
        has_updated = false;
    };
    /// @brief Handles any update logic for the object to move
    /// @param delta_time The time since the last frame
    virtual void update(float delta_time)
    {
        has_updated = true;
    };
};