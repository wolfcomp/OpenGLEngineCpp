#pragma once

#include "Collider.h"

class Line;
class AABB;
class SphereCollider;

class Mesh : public Collider<Mesh>
{
public:
    Mesh() : Collider() {}
    Mesh(GameObject *parent) : Collider(parent) {}
    ~Mesh() {};

    template <typename T>
    bool contains(T point)
    {
        return false;
        std::string message = "Not implemented on type of ";
        throw message + typeid(T).name();
    }

    bool contains(const Mesh &other) const override;

    template <>
    bool contains<GameObject *>(GameObject *point);

    template <>
    bool contains<AABB *>(AABB *point);

    template <>
    bool contains<SphereCollider *>(SphereCollider *point);

    void update(GameObject *object) override;

    bool is_on_frustum(Frustum *frustum) override;

    bool is_on_or_forward_plane(Plane *plane);

    glm::vec3 find_furthest_point(glm::vec3 direction) override;
};