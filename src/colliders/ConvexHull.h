#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "AABB.h"

class Renderable;

class ConvexHull
{
private:
    Renderable *hull;
    Renderable *parent;

public:
    /// @brief Default constructor
    ConvexHull() : hull(nullptr), parent(nullptr) {};
    /// @brief Constructor that creates a convex hull based on AABB bounds
    /// @param bounds The bounds of the convex hull
    /// @param parent The renderable that is the parent of the convex hull
    ConvexHull(AABB bounds, Renderable *parent);
    ~ConvexHull()
    {
        delete hull;
    };

    /// @brief Uses the gift wrapping algorithm to create a convex hull around the renderable in 2d space (x,z) and expands it to 3d space with the highest and lowest y values of the renderable
    /// @param renderable The renderable to create a convex hull around
    void gift_wrap(Renderable *renderable);
    /// @brief Uses the quick hull algorithm to create a convex hull around the renderable in 3d space
    /// @param renderable The renderable to create a convex hull around
    void quick_hull(Renderable *renderable);
    /// @brief Draws the convex hull in debug mode
    void draw_debug();
    /// @brief Checks if the convex hull intersects with another collider and if so returns true
    /// @tparam T
    /// @param collider The collider to check for intersection with
    /// @return true If the convex hull intersects with the collider
    template <typename T>
    bool contains(const T &collider) const
    {
        std::string message = "Not implemented on type of ";
        throw message + typeid(T).name();
    }
};