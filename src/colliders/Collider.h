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

template <typename T>
class Collider
{
private:
    SceneObject *parent;
    CollisionResponse response;
    CollisionChannel channel;

protected:
    std::vector<std::function<void(Collider *)>> onCollisionCallbacks;

public:
    Collider(SceneObject *parent) : parent(parent), response(CollisionResponse::COLLIDE), channel(CollisionChannel::STATIC) {}
    Collider() : parent(nullptr) {}
    virtual ~Collider() {}
    void add_callback(std::function<void(Collider *)> callback) { onCollisionCallbacks.push_back(callback); }

    void set_response(CollisionResponse response) { this->response = response; }

    CollisionResponse get_response() const { return response; }

    virtual bool contains(const T &point) const = 0;

    virtual void update(SceneObject *object) = 0;
};