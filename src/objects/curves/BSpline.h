#pragma once

#include "Curve.h"

template <typename T>
class BSpline : public Curve<T>
{
private:
    std::vector<T> points;
    int degree;

public:
    /// @brief Get a point on the curve at a given time
    /// @param t The delta time to get the point at (0-1)
    /// @return The point on the curve at the given time
    T get_point(float t) const override
    {
    }
};
