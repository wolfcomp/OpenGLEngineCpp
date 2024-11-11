#pragma once

#include "GameObjectBase.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../colliders/Collider.h"
#include "../../input/Camera.h"

class GameObject : public GameObjectBase
{
private:
    std::vector<GameObject *> children;
    GameObject *parent;
    ColliderBase *collider;
    bool is_active;
    bool has_updated;

public:
    GameObject(std::vector<Vertex> vertices, std::vector<unsigned> indices, World *world) : GameObjectBase(vertices, indices, world), collider(nullptr), is_active(true), has_updated(false), parent(nullptr) {}
    GameObject() : GameObject({}, {}, nullptr) {}
    ~GameObject()
    {
        if (collider != nullptr)
            delete collider;
    }

    /// @brief Adds a child to the object
    /// @param child The child object to add
    void add_child(GameObject *child) { children.push_back(child); }
    /// @brief Removes a child from the object
    /// @param child The child object to remove
    void remove_child(GameObject *child) { children.erase(std::remove(children.begin(), children.end(), child), children.end()); }
    /// @brief Gets the children of the object
    /// @return The children of the object
    std::vector<GameObject *> get_children() { return children; }

    /// @brief Sets a collider for the object
    /// @tparam T The type of collider to set
    /// @param collider The collider to set
    template <typename T>
    void set_collider(Collider<T> *collider)
    {
        this->collider = collider;
        collider->set_parent(this);
        collider->update(this);
    }
    /// @brief Gets the collider of the object
    /// @return The collider of the object
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
        return get_component<TransformComponent>()->get_model_matrix();
    }

    glm::mat4x4 get_world_matrix(glm::mat4x4 world) const
    {
        if (parent != nullptr)
            return parent->get_world_matrix(world) * get_component<TransformComponent>()->get_model_matrix();
        return world * get_component<TransformComponent>()->get_model_matrix();
    }

    void set_active(bool is_active) { this->is_active = is_active; }

    bool get_active() const { return is_active; }
    bool get_has_updated() const { return has_updated; }

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