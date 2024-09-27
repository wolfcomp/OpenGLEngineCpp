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
    Bezier(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3), Curve<T>()
    {
        this->generate_curve();
    }

    T get_point(float t) const override;
};