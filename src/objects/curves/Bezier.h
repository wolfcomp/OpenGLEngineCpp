#pragma once

#include "Curve.h"

template <typename T>
class BezierBase : public CurveBase<T>
{
private:
    T p0;
    T p1;
    T p2;
    T p3;

public:
    BezierBase() : p0(T()), p1(T()), p2(T()), p3(T()) {}
    BezierBase(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}
    T get_point(float t) override;
};

template <typename T>
class Bezier : public Curve<T>
{

public:
    Bezier(T p0, T p1, T p2, T p3) : Curve<T>(BezierBase<T>(p0, p1, p2, p3))
    {
        this->generate_curve();
    }
};