#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "AABB.h"
#include "Collider.h"

class GameObjectBase;

/// @brief Not implemented yet
class ConvexHull : public Collider<ConvexHull>
{
private:
    GameObjectBase *hull;
    GameObjectBase *parent;

public:
    /// @brief Default constructor
    ConvexHull() : hull(nullptr), parent(nullptr) {};
    /// @brief Constructor that creates a convex hull based on AABB bounds
    /// @param bounds The bounds of the convex hull
    /// @param parent The GameObjectBase that is the parent of the convex hull
    ConvexHull(AABB bounds, GameObjectBase *parent);
    ~ConvexHull()
    {
        delete hull;
    };

    /// @brief Uses the gift wrapping algorithm to create a convex hull around the GameObjectBase in 2d space (x,z) and expands it to 3d space with the highest and lowest y values of the GameObjectBase
    /// @param GameObjectBase The GameObjectBase to create a convex hull around
    void gift_wrap(GameObjectBase *GameObjectBase);
    /// @brief Uses the quick hull algorithm to create a convex hull around the GameObjectBase in 3d space
    /// @param GameObjectBase The GameObjectBase to create a convex hull around
    void quick_hull(GameObjectBase *GameObjectBase);
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