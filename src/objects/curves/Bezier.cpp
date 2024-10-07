#include "Bezier.h"

template <typename T>
T BezierBase<T>::get_point(float t) { return T(); }

template <>
glm::vec3 BezierBase<glm::vec3>::get_point(float t)
{
    const auto one_minus_t = 1.0f - t;

    return p0 * powf(one_minus_t, 3) +
           p1 * 3.0f * t * powf(one_minus_t, 2) +
           p2 * 3.0f * powf(t, 2) * one_minus_t +
           p3 * powf(t, 3);
}

template class BezierBase<glm::vec3>;