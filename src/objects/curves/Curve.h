#pragma once

#include "../base/GameObject.h"

template <typename T>
class Curve : public GameObject
{
public:
    /// @brief Get a point on the curve at a given time
    /// @param t The delta time to get the point at
    /// @return The point on the curve at the given time
    virtual T get_point(float t) const { return T(); }
};