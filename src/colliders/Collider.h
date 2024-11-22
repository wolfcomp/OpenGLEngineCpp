#pragma once

#include <string>
#include <vector>
#include <functional>
#include <glm/vec3.hpp>
#include "../culling/Frustum.h"

class GameObject;

enum CollisionResponse
{
    IGNORE,
    OVERLAP,
    COLLIDE
};

enum CollisionChannel
{
    STATIC,
    DYNAMIC
};

class ColliderBase
{
private:
    GameObject *parent;
    CollisionResponse response;
    CollisionChannel channel;

public:
    ColliderBase(GameObject *parent) : parent(parent), response(CollisionResponse::COLLIDE), channel(CollisionChannel::DYNAMIC) {}
    ColliderBase() : parent(nullptr) {}
    virtual ~ColliderBase() {}

    GameObject *get_parent() { return parent; }
    CollisionResponse get_response() { return response; }
    CollisionChannel get_channel() { return channel; }

    void set_response(CollisionResponse response) { this->response = response; }
    void set_channel(CollisionChannel channel) { this->channel = channel; }
    void set_parent(GameObject *parent) { this->parent = parent; }

    virtual void update(GameObject *object) = 0;
    virtual float get_radius() { return 0.0f; }
    virtual glm::vec3 get_center() { return glm::vec3(0.0f); }
    virtual bool is_on_frustum(Frustum *frustum) { return true; }
    template <typename T>
    float collision_delta(T *collider, float delta_time);

    virtual glm::vec3 find_furthest_point(glm::vec3 direction) = 0;
    glm::vec3 support(ColliderBase &other, glm::vec3 direction)
    {
        return this->find_furthest_point(direction) - other.find_furthest_point(-direction);
    }
    glm::vec3 support(glm::vec3 direction)
    {
        return this->find_furthest_point(direction);
    }
};

template <typename T>
class Collider : public ColliderBase
{
protected:
    std::vector<std::function<void(Collider *)>> onCollisionCallbacks;

public:
    Collider(GameObject *parent) : ColliderBase(parent) {}
    Collider() : ColliderBase() {}
    virtual ~Collider() {}
    void add_callback(std::function<void(Collider *)> callback) { onCollisionCallbacks.push_back(callback); }

    virtual bool contains(const T &point) const = 0;
};