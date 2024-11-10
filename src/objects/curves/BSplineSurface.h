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
    std::vector<glm::vec3> points;
    std::vector<float> knot_vector_u;
    std::vector<float> knot_vector_v;

    std::pair<glm::vec3, glm::vec3> b2(float tu, float tv, int iu, int iv)
    {
        glm::vec3 bv;
        glm::vec3 bu;

        auto w12 = (tu - knot_vector_u[iu - 1]) / (knot_vector_u[iu + 1] - knot_vector_u[iu - 1]);
        auto w22 = (tu - knot_vector_u[iu]) / (knot_vector_u[iu + 2] - knot_vector_u[iu]);
        auto w11 = (tu - knot_vector_u[iu]) / (knot_vector_u[iu + 1] - knot_vector_u[iu]);

        bu.x = (1 - w11) * (1 - w12);
        bu.z = w11 * w22;
        bu.y = 1 - bu.x - bu.z;

        w12 = (tv - knot_vector_v[iv - 1]) / (knot_vector_v[iv + 1] - knot_vector_v[iv - 1]);
        w22 = (tv - knot_vector_v[iv]) / (knot_vector_v[iv + 2] - knot_vector_v[iv]);
        w11 = (tv - knot_vector_v[iv]) / (knot_vector_v[iv + 1] - knot_vector_v[iv]);

        bv.x = (1 - w11) * (1 - w12);
        bv.z = w11 * w22;
        bv.y = 1 - bv.x - bv.z;

        return std::make_pair(bu, bv);
    }

    glm::vec3 evaluateBiquadratic(int iu, int iv, glm::vec3 bu, glm::vec3 bv)
    {

        glm::vec3 r(0);
        glm::mat3x3 m(0);
        glm::vec3 tmp_points[3][3];

        auto du = iu - degree_u;
        auto dv = iv - degree_v;

        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                auto u = du + i;
                auto v = dv + j;
                m[j][i] = bu[i] * bv[j];
                tmp_points[j][i] = points[u + v * num_points_u];
                r += bu[i] * bv[j] * points[u + v * num_points_u];
            }
        }

        return r;
    }

    int find_knot_interval(float t, int degree, int num_points, std::vector<float> knot_vector)
    {
        int n = num_points - 1;
        while (t < knot_vector[n] && n > degree)
            n--;
        return n;
    }

public:
    BSplineSurface(int degree_u, int degree_v, int num_points_u, int num_points_v, std::vector<float> knot_vector_u, std::vector<float> knot_vector_v, std::vector<glm::vec3> points, float spacing = 0.1f) : GameObject()
    {
        this->degree_u = degree_u;
        this->degree_v = degree_v;
        this->num_points_u = num_points_u;
        this->num_points_v = num_points_v;
        this->knot_vector_u = knot_vector_u;
        this->knot_vector_v = knot_vector_v;
        this->points = points;

        computeSurface(spacing);
    }

    void computeSurface(float spacing = 0.1f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        int num_u = 1 + (knot_vector_u[num_points_u] - knot_vector_u[degree_u]) / spacing;
        int num_v = 1 + (knot_vector_v[num_points_v] - knot_vector_v[degree_v]) / spacing;

        for (int i = 0; i < num_u; i++)
        {
            for (int j = 0; j < num_v; j++)
            {
                float u = i * spacing;
                float v = j * spacing;

                auto iu = find_knot_interval(u, degree_u, num_points_u, knot_vector_u);
                auto iv = find_knot_interval(v, degree_v, num_points_v, knot_vector_v);

                auto coeff_pair = b2(u, v, iu, iv);

                glm::vec3 p = evaluateBiquadratic(iu, iv, coeff_pair.first, coeff_pair.second);

                vertices.push_back(Vertex{p, glm::vec3(0, 1, 0), glm::vec2(0, 0)});
            }
        }

        for (int i = 0; i < num_u - 1; i++)
        {
            for (int j = 0; j < num_v - 1; j++)
            {
                auto p0 = i * num_v + j;
                auto p1 = i * num_v + j + 1;
                auto p2 = (i + 1) * num_v + j + 1;
                auto p3 = (i + 1) * num_v + j;
                auto n1 = glm::normalize(glm::cross(vertices[p1].position - vertices[p0].position, vertices[p2].position - vertices[p0].position));
                auto n2 = glm::normalize(glm::cross(vertices[p2].position - vertices[p0].position, vertices[p3].position - vertices[p0].position));
                indices.push_back(p0);
                indices.push_back(p1);
                indices.push_back(p2);
                vertices[p0].normal = n1;
                vertices[p1].normal = n1;
                vertices[p2].normal = n1;

                indices.push_back(p0);
                indices.push_back(p2);
                indices.push_back(p3);
                vertices[p0].normal = glm::normalize(n2 + vertices[p0].normal);
                vertices[p2].normal = glm::normalize(n2 + vertices[p2].normal);
                vertices[p3].normal = n2;
            }
        }

        update_vertices(vertices);
        update_indices(indices);
    }
};