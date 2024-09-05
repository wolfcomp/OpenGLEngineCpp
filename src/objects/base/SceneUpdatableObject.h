#pragma once

#include "SceneObject.h"

class SceneUpdatableObject : public SceneObject
{
private:
    bool is_active;
    bool has_updated;
    glm::vec3 velocity;
    glm::vec3 acceleration;

public:
    SceneUpdatableObject(std::vector<Vertex> vertices, std::vector<unsigned> indices) : SceneObject(vertices, indices), is_active(true), velocity(0), acceleration(0) {}
    ~SceneUpdatableObject() {}

    void set_active(bool is_active) { this->is_active = is_active; }
    void set_velocity(glm::vec3 velocity) { this->velocity = velocity; }
    void set_acceleration(glm::vec3 acceleration) { this->acceleration = acceleration; }

    bool get_active() const { return is_active; }
    bool get_has_updated() const { return has_updated; }
    glm::vec3 get_velocity() const { return velocity; }
    glm::vec3 get_acceleration() const { return acceleration; }

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