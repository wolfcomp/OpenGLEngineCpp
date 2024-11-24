#pragma once

#include "../base/GameObject.h"
#include "../../Window.h"
#include "BSpline.h"
#include <map>
#include <format>
#include <glm/gtx/exterior_product.hpp>

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

    glm::vec3 barycentric_claude(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, const glm::vec3 &p)
    {
        glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);
        float denom = d00 * d11 - d01 * d01;

        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        return glm::vec3(u, v, w);
    }

    glm::vec3 *barycentric(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, const glm::vec3 &p)
    {
        auto baryc = barycentric_claude(a, b, c, p);

        // auto n = b - a;
        // auto m = c - a;
        // auto baryc = glm::cross(n, m);
        // auto v = glm::length(baryc);

        // auto f = b.xz() - p;
        // auto g = c.xz() - p;
        // baryc.x = (glm::cross(f, g) / v);

        // f = c.xz() - p;
        // g = a.xz() - p;
        // baryc.y = (glm::cross(f, g) / v);

        // f = a.xz() - p;
        // g = b.xz() - p;
        // baryc.z = (glm::cross(f, g) / v);

        if (baryc.x > 1 || baryc.y > 1 || baryc.z > 1 || baryc.x < 0 || baryc.y < 0 || baryc.z < 0)
            return nullptr;

        return &baryc;
    }

    float *get_y_at_index(int i, int j, int size, glm::vec3 position)
    {
        auto vertices = get_vertices_ptr();
        auto v0 = vertices->at(i * size + j);
        auto v1 = vertices->at((i + 1) * size + j);
        auto v2 = vertices->at(i * size + (j + 1));
        auto v3 = vertices->at((i + 1) * size + (j + 1));
        auto baryc = barycentric(v0.position, v1.position, v2.position, position);
        auto first = true;
        if (baryc == nullptr)
        {
            baryc = barycentric(v2.position, v1.position, v3.position, position);
            first = false;
        }
        if (baryc == nullptr)
            return nullptr;
        if (first)
        {
            auto y = v0.position.y * baryc->x + v1.position.y * baryc->y + v2.position.y * baryc->z;
            return &y;
        }
        auto y = v2.position.y * baryc->x + v1.position.y * baryc->y + v3.position.y * baryc->z;
        return &y;
    }

    glm::vec3 get_normal_at_index(int i, int j, int size, glm::vec3 position)
    {
        auto vertices = get_vertices_ptr();
        auto v0 = vertices->at(i * size + j);
        auto v1 = vertices->at((i + 1) * size + j);
        auto v2 = vertices->at(i * size + (j + 1));
        auto v3 = vertices->at((i + 1) * size + (j + 1));
        auto baryc = barycentric(v0.position, v1.position, v2.position, position);
        auto first = true;
        if (baryc == nullptr)
        {
            baryc = barycentric(v2.position, v1.position, v3.position, position);
            first = false;
        }
        if (baryc == nullptr)
            return glm::vec3(0);
        if (first)
            return v0.normal * baryc->x + v1.normal * baryc->y + v2.normal * baryc->z;
        return v2.normal * baryc->x + v1.normal * baryc->y + v3.normal * baryc->z;
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
            glfwSetWindowTitle(Window::glfWindow, std::format("Processing BSplineSurface vertexes: {:.2f}%", (float)(i * num_v) / (num_v * num_v) * 100).c_str());
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
            glfwSetWindowTitle(Window::glfWindow, std::format("Processing BSplineSurface indices: {:.2f}%", (float)(i * num_v) / (num_v * num_v) * 100).c_str());
        }

        update_vertices(vertices);
        update_indices(indices);
    }

    std::tuple<float, glm::vec3> get_y_at(glm::vec3 position)
    {
        auto vertices = get_vertices_ptr();
        auto lastDistanceSquared = FLT_MAX;
        auto lastVertexIndexX = 0;
        auto size = static_cast<int>(sqrt(vertices->size()));
        for (int i = 0; i < size; i++)
        {
            auto distanceSquared = glm::distance2(vertices->at(i).position.xz(), position.xz());
            if (distanceSquared < lastDistanceSquared)
            {
                lastDistanceSquared = distanceSquared;
                lastVertexIndexX = i;
            }
            else
            {
                break;
            }
        }
        auto lastVertexIndexY = 0;
        lastDistanceSquared = FLT_MAX;
        for (int i = 0; i < size; i++)
        {
            auto distanceSquared = glm::distance2(vertices->at(i * size).position.xz(), position.xz());
            if (distanceSquared < lastDistanceSquared)
            {
                lastDistanceSquared = distanceSquared;
                lastVertexIndexY = i;
            }
            else
            {
                break;
            }
        }
        auto y = get_y_at_index(lastVertexIndexX, lastVertexIndexY, size, position);
        if (y != nullptr)
        {
            auto yCopy = *y;
            return std::make_tuple(yCopy, get_normal_at_index(lastVertexIndexX, lastVertexIndexY, size, position));
        }
        lastVertexIndexX++;
        y = get_y_at_index(lastVertexIndexX, lastVertexIndexY, size, position);
        if (y != nullptr)
        {
            auto yCopy = *y;
            return std::make_tuple(yCopy, get_normal_at_index(lastVertexIndexX, lastVertexIndexY, size, position));
        }
        lastVertexIndexY++;
        lastVertexIndexX--;
        y = get_y_at_index(lastVertexIndexX, lastVertexIndexY, size, position);
        if (y != nullptr)
        {
            auto yCopy = *y;
            return std::make_tuple(yCopy, get_normal_at_index(lastVertexIndexX, lastVertexIndexY, size, position));
        }
        lastVertexIndexX++;
        y = get_y_at_index(lastVertexIndexX, lastVertexIndexY, size, position);
        if (y != nullptr)
        {
            auto yCopy = *y;
            return std::make_tuple(yCopy, get_normal_at_index(lastVertexIndexX, lastVertexIndexY, size, position));
        }
        auto min_bounds = get_min_vertex().position;
        return std::make_tuple(min_bounds.y, glm::vec3(0));
    }
};