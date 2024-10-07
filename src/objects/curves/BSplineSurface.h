#pragma once

#include "../base/GameObject.h"
#include "BSpline.h"
#include <map>

class BSplineSurface : public GameObject
{
private:
    int degree_u;
    int degree_v;
    int num_points_u;
    int num_points_v;
    std::vector<std::vector<glm::vec3>> points;
    std::vector<float> knot_vector_u;
    std::vector<float> knot_vector_v;
    std::map<int, BSplineBase<glm::vec3>> splines_u{};
    std::map<int, BSplineBase<glm::vec3>> splines_v{};

public:
    BSplineSurface(int degree_u, int degree_v, int num_points_u, int num_points_v, std::vector<float> knot_vector_u, std::vector<float> knot_vector_v, std::vector<std::vector<glm::vec3>> points) : GameObject()
    {
        this->degree_u = degree_u;
        this->degree_v = degree_v;
        this->num_points_u = num_points_u;
        this->num_points_v = num_points_v;
        this->knot_vector_u = knot_vector_u;
        this->knot_vector_v = knot_vector_v;
        this->points = points;

        computeSurface();
    }

    std::pair<glm::vec3, glm::vec3> get_point(float u, float v)
    {
        glm::vec3 p(0, 0, 0), n(0, 0, 0);
        int iu0 = std::ceil(u - 1);
        int iv0 = std::ceil(v - 1);
        if (iu0 < 0)
            iu0 = 0;
        if (iv0 < 0)
            iv0 = 0;
        int iu1 = iu0 + 1;
        int iv1 = iv0 + 1;
        if (iu1 >= num_points_u)
            iu1 = num_points_u - 1;
        if (iv1 >= num_points_v)
            iv1 = num_points_v - 1;

        if (!splines_u.contains(iv0))
        {
            auto f = points[iv0];
            splines_u[iv0] = BSplineBase<glm::vec3>(f, degree_u, knot_vector_u);
        }
        if (!splines_u.contains(iv1))
        {
            auto f = points[iv1];
            splines_u[iv1] = BSplineBase<glm::vec3>(f, degree_u, knot_vector_u);
        }
        if (!splines_v.contains(iu0))
        {
            std::vector<glm::vec3> tmp;
            for (int i = 0; i < num_points_v; i++)
            {
                tmp.push_back(points[i][iu0]);
            }
            splines_v[iu0] = BSplineBase<glm::vec3>(tmp, degree_v, knot_vector_v);
        }
        if (!splines_v.contains(iu1))
        {
            std::vector<glm::vec3> tmp;
            for (int i = 0; i < num_points_v; i++)
            {
                tmp.push_back(points[i][iu1]);
            }
            splines_v[iu1] = BSplineBase<glm::vec3>(tmp, degree_v, knot_vector_v);
        }

        auto pu0 = splines_u[iv0].get_point(u);
        auto pu1 = splines_u[iv1].get_point(u);
        auto pv0 = splines_v[iu0].get_point(v);
        auto pv1 = splines_v[iu1].get_point(v);

        while (u > 1)
            u -= 1;
        while (v > 1)
            v -= 1;

        auto tu = 1 - u;
        auto tv = 1 - v;

        auto pu = pu0 * tv + pu1 * v;
        auto pv = pv0 * tu + pv1 * u;

        p = (pu + pv) / 2.0f;
        n = glm::normalize(glm::cross(pu1 - pu0, pv1 - pv0));

        return {p, n};
    }

    void computeSurface(float spacing = 0.1f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        int num_u = (1 / spacing) * num_points_u;
        int num_v = (1 / spacing) * num_points_v;

        for (int i = 0; i < num_u; i++)
        {
            auto f = i;
            for (int j = 0; j < num_v; j++)
            {
                float u = i * spacing;
                float v = j * spacing;
                auto [p, n] = get_point(u, v);
                vertices.push_back(Vertex{p, n, glm::vec2(0, 0)});
            }
        }

        for (int i = 0; i < num_u - 1; i++)
        {
            for (int j = 0; j < num_v - 1; j++)
            {
                indices.push_back(i * num_v + j);
                indices.push_back(i * num_v + j + 1);
                indices.push_back((i + 1) * num_v + j + 1);

                indices.push_back(i * num_v + j);
                indices.push_back((i + 1) * num_v + j + 1);
                indices.push_back((i + 1) * num_v + j);
            }
        }

        update_vertices(vertices);
        update_indices(indices);
    }
};