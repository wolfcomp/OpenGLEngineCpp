#pragma once

#include "Curve.h"

template <typename T>
class Bezier : public Curve<T>
{
private:
    T p0;
    T p1;
    T p2;
    T p3;

public:
    Bezier(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}

    T get_point(float t) const override
    {
        float u = 1 - t;

        return p0 * powf(u, 3) +
               p1 * 3 * t * powf(u, 2) +
               p2 * 3 * powf(t, 2) * u +
               p3 * powf(t, 3);
    }
};