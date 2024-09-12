#pragma once

#include <string>
#include <vector>
#include <functional>

class SceneObject;

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
    SceneObject *parent;
    CollisionResponse response;
    CollisionChannel channel;

public:
    ColliderBase(SceneObject *parent) : parent(parent), response(CollisionResponse::COLLIDE), channel(CollisionChannel::DYNAMIC) {}
    ColliderBase() : parent(nullptr) {}
    virtual ~ColliderBase() {}

    SceneObject *get_parent() { return parent; }
    CollisionResponse get_response() { return response; }
    CollisionChannel get_channel() { return channel; }

    void set_response(CollisionResponse response) { this->response = response; }
    void set_channel(CollisionChannel channel) { this->channel = channel; }
    void set_parent(SceneObject *parent) { this->parent = parent; }

    virtual void update(SceneObject *object) = 0;
};

template <typename T>
class Collider : public ColliderBase
{
protected:
    std::vector<std::function<void(Collider *)>> onCollisionCallbacks;

public:
    Collider(SceneObject *parent) : ColliderBase(parent) {}
    Collider() : ColliderBase() {}
    virtual ~Collider() {}
    void add_callback(std::function<void(Collider *)> callback) { onCollisionCallbacks.push_back(callback); }

    virtual bool contains(const T &point) const = 0;
};