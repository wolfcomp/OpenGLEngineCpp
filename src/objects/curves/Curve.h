#pragma once

#include "../base/GameObject.h"

template <typename T>
class CurveBase
{
public:
    virtual T get_point(float t) { return T(); };
};

template <typename T>
class Curve : public GameObject
{
private:
    CurveBase<T> curve;

public:
    Curve() : GameObject()
    {
        set_mode(GL_LINES);
    }

    Curve(CurveBase<T> curve) : Curve()
    {
        this->curve = curve;
    }

    virtual void generate_curve()
    {
        // Generate a curve from 0 to 1 with 100 points and feed it to the vertex and indices buffer
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        for (int i = 0; i < 100; i++)
        {
            float t = i / 100.0f;
            float t2 = (i + 1) / 100.0f;
            T p0 = curve.get_point(t);
            T p1 = curve.get_point(t2);
            // Add line from p0 to p1
            vertices.push_back(Vertex{p0, glm::vec3(0, 0, 0), glm::vec2(0, 0)});
            vertices.push_back(Vertex{p1, glm::vec3(0, 0, 0), glm::vec2(0, 0)});
            indices.push_back(i * 2);
            indices.push_back(i * 2 + 1);
        }
        update_vertices(vertices);
        update_indices(indices);
    }

    void pre_render() const override
    {
        GameObject::pre_render();
        glLineWidth(2.0f);
    }

    void post_render() const override
    {
        glLineWidth(1.0f);
        GameObject::post_render();
    }
};