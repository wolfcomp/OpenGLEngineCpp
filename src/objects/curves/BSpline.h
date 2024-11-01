#pragma once

#include "Curve.h"

template <typename T>
class BSplineBase : public CurveBase<T>
{
private:
    std::vector<T> points;
    std::vector<float> knot_vector;
    int degree;
    // Helper function to generate a uniform knot vector
    std::vector<float> generate_knot_vector(int num_points, int degree) const
    {
        int n = num_points + degree + 1;
        std::vector<float> knot_vector(n);
        for (int i = 0; i <= degree; ++i)
        {
            knot_vector[i] = 0.0f;
        }
        for (int i = degree + 1; i < num_points; ++i)
        {
            knot_vector[i] = (i - degree) / static_cast<float>(num_points - degree);
        }
        for (int i = num_points; i < n; ++i)
        {
            knot_vector[i] = 1.0f;
        }
        return knot_vector;
    }

    // Helper function to find the knot span index
    int find_knot_span(float t, const std::vector<float> &knot_vector) const
    {
        int n = points.size() - 1;
        if (t >= knot_vector[n + 1])
        {
            return n;
        }
        if (t <= knot_vector[degree])
        {
            return degree;
        }
        int low = degree;
        int high = n + 1;
        int mid = (low + high) / 2;
        while (t < knot_vector[mid] || t >= knot_vector[mid + 1])
        {
            if (t < knot_vector[mid])
            {
                high = mid;
            }
            else
            {
                low = mid;
            }
            mid = (low + high) / 2;
        }
        return mid;
    }

public:
    BSplineBase() : points({}), degree(0) {}
    BSplineBase(std::vector<T> points, int degree) : points(points), degree(degree) {}
    BSplineBase(std::vector<T> points, int degree, std::vector<float> knot_vector) : points(points), degree(degree), knot_vector(knot_vector) {}

    /// @brief Get a point on the curve at a given time
    /// @param t The delta time to get the point at (0-1)
    /// @return The point on the curve at the given time
    T get_point(float t) override
    {
        int num_points = points.size();
        if (num_points == 0)
        {
            throw std::runtime_error("No control points defined.");
        }

        if (knot_vector.empty())
            knot_vector = generate_knot_vector(num_points, degree);
        int span = find_knot_span(t, knot_vector);

        std::vector<T> de_boor_points(degree + 1);
        for (int j = 0; j <= degree; ++j)
        {
            de_boor_points[j] = points[span - degree + j];
        }

        for (int r = 1; r <= degree; ++r)
        {
            for (int j = degree; j >= r; --j)
            {
                float alpha = (t - knot_vector[span - degree + j]) / (knot_vector[span + 1 + j - r] - knot_vector[span - degree + j]);
                de_boor_points[j] = (1.0f - alpha) * de_boor_points[j - 1] + alpha * de_boor_points[j];
            }
        }

        return de_boor_points[degree];
    }
};

template <typename T>
class BSpline : public Curve<T>
{
public:
    BSpline(std::vector<T> points, int degree) : Curve<T>(BSplineBase<T>(points, degree))
    {
        this->generate_curve();
    }

    /// @brief Get the control points of the curve in b2 format
    /// @param num_points The number of control points to generate
    /// @return The control points of the curve in b2 format
    static std::vector<float> get_knot_vector(int num_points)
    {
        std::vector<float> knot_vector(num_points + 5);
        knot_vector[0] = knot_vector[1] = knot_vector[2] = 0;
        for (int i = 3; i < num_points + 2; i++)
        {
            knot_vector[i] = i - 2;
        }
        knot_vector[num_points + 2] = knot_vector[num_points + 3] = knot_vector[num_points + 4] = num_points;
        return knot_vector;
    }
};
