#pragma once

#include "Collider.h"

class Line;
class AABB;
class SphereCollider;

class Mesh : public Collider<Mesh>
{
public:
    Mesh();
    ~Mesh() {};

    template <typename T>
    bool contains(const T &point) const
    {
        std::string message = "Not implemented on type of ";
        throw message + typeid(T).name();
    }

    bool contains(const Mesh &other) const override;

    template <>
    bool contains<GameObject *>(GameObject *const &point) const;

    // template <>
    // bool contains<AABB *>(AABB *const &point) const;

    // template <>
    // bool contains<SphereCollider *>(SphereCollider *const &point) const;

    void update(GameObject *object) override;

    bool is_on_frustum(Frustum *frustum) override;

    bool is_on_or_forward_plane(Plane *plane);
};