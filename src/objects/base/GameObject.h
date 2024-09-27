#pragma once

#include "GameObjectBase.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../colliders/Collider.h"
#include "../../input/Camera.h"
#include "Transform.h"

class Physics
{
private:
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
    GameObject *parent;

public:
    Physics(glm::vec3 velocity, glm::vec3 acceleration, float mass) : velocity(velocity), acceleration(acceleration), mass(mass) {}
    Physics(GameObject *parent) : parent(parent), velocity(0.0f, 0.0f, 0.0f), acceleration(0.0f, 0.0f, 0.0f), mass(0.0f) {}
    Physics(GameObject *parent, glm::vec3 velocity, glm::vec3 acceleration, float mass) : parent(parent), velocity(velocity), acceleration(acceleration), mass(mass) {}
    ~Physics() {}

    void set_velocity(glm::vec3 velocity) { this->velocity = velocity; }
    void set_acceleration(glm::vec3 acceleration) { this->acceleration = acceleration; }
    void set_mass(float mass) { this->mass = mass; }

    glm::vec3 get_velocity() const { return velocity; }
    glm::vec3 get_acceleration() const { return acceleration; }
    float get_mass() const { return mass; }
    GameObject *get_parent() const { return parent; }

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
    void apply_collision(glm::vec3 normal);
};

class GameObject : public GameObjectBase
{
private:
    Transform transform;
    std::vector<GameObject *> children;
    GameObject *parent;
    ColliderBase *collider;
    bool is_active;
    bool has_updated;
    Physics physics;

public:
    GameObject(std::vector<Vertex> vertices, std::vector<unsigned> indices) : GameObjectBase(vertices, indices), transform({glm::vec3(0), glm::vec3(0), glm::quat(1, 0, 0, 0)}), collider(nullptr), physics(this), is_active(true), has_updated(false), parent(nullptr) {}
    ~GameObject()
    {
        if (collider != nullptr)
            delete collider;
    }

    void set_position(glm::vec3 position)
    {
        this->transform.position = position;
        if (collider != nullptr)
            collider->update(this);
    }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in quaternion
    void set_rotation(glm::quat rotation) { this->transform.rotation = rotation; }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in euler angles
    void set_rotation(glm::vec3 rotation) { this->transform.rotation = glm::quat(rotation); }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in normalized vector form
    void set_rotation_world_up(glm::vec3 rotation) { this->transform.rotation = glm::quat(glm::normalize(rotation), glm::vec3(0, 1, 0)); }
    void set_scale(glm::vec3 scale)
    {
        this->transform.scale = scale;
        if (collider != nullptr)
            collider->update(this);
    }
    glm::vec3 get_position() { return transform.position; }
    glm::vec3 get_world_position()
    {
        glm::vec3 world_position = transform.position;
        if (parent != nullptr)
            world_position += parent->get_world_position();
        return world_position;
    }
    glm::quat get_rotation() { return transform.rotation; }
    glm::vec3 get_scale() { return transform.scale; }

    void translate(glm::vec3 translation)
    {
        transform.position += translation;
        if (collider != nullptr)
            collider->update(this);
    }
    void rotate(glm::quat rotation) { this->transform.rotation = rotation * this->transform.rotation; }
    void scale_by(glm::vec3 scale)
    {
        this->transform.scale *= scale;
        if (collider != nullptr)
            collider->update(this);
    }

    void add_child(GameObject *child) { children.push_back(child); }
    void remove_child(GameObject *child) { children.erase(std::remove(children.begin(), children.end(), child), children.end()); }
    std::vector<GameObject *> get_children() { return children; }

    template <typename T>
    void set_collider(Collider<T> *collider)
    {
        this->collider = collider;
        collider->set_parent(this);
        collider->update(this);
    }
    virtual ColliderBase *get_collider() { return collider; }

    void pre_render() const override
    {
        GameObjectBase::pre_render();
        get_shader()->set_mat4("model", get_model_matrix());
    }

    void render() const override
    {
        GameObjectBase::render();
        for (auto &child : children)
        {
            child->pre_render();
            child->render();
            child->post_render();
        }
    }

    virtual void render(const Camera &camera) const
    {
        render();
    }

    glm::mat4x4 get_model_matrix() const
    {
        return transform.get_model_matrix();
    }

    glm::mat4x4 get_world_matrix(glm::mat4x4 world) const
    {
        if (parent != nullptr)
            return parent->get_world_matrix(world) * transform.get_model_matrix();
        return world * transform.get_model_matrix();
    }

    void set_active(bool is_active) { this->is_active = is_active; }
    void set_velocity(glm::vec3 velocity) { physics.set_velocity(velocity); }
    void set_acceleration(glm::vec3 acceleration) { physics.set_acceleration(acceleration); }
    void set_mass(float mass) { physics.set_mass(mass); }

    bool get_active() const { return is_active; }
    bool get_has_updated() const { return has_updated; }
    glm::vec3 get_velocity() const { return physics.get_velocity(); }
    glm::vec3 get_acceleration() const { return physics.get_acceleration(); }
    float get_mass() const { return physics.get_mass(); }
    Physics *get_physics() { return &physics; }

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

    void apply_collision(GameObject *other) { physics.apply_collision(&other->physics); }
    void apply_collision(glm::vec3 normal) { physics.apply_collision(normal); }
};