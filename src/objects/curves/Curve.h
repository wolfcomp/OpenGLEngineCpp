#pragma once

#include "../base/GameObject.h"

template <typename T>
class CurveBase
{
public:
    virtual T get_point(float t) { return T(); };
    virtual void add_point(T point) {};
    virtual bool has_points() { return false; };
};

template <typename T>
class Curve : public GameObject
{
private:
    CurveBase<T> *curve;
    float resolution = 100;

public:
    Curve() : GameObject()
    {
        set_mode(GL_LINES);
    }

    Curve(CurveBase<T> *curve) : Curve()
    {
        this->curve = curve;
        this->generate_curve();
    }

    Curve(CurveBase<T> curve, float resolution) : Curve(curve)
    {
        this->resolution = resolution;
    }

    ~Curve()
    {
        delete curve;
    }

    virtual void generate_curve()
    {
        if (!curve->has_points())
            return;
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        for (int i = 0; i < resolution; i++)
        {
            float t = i / resolution;
            float t2 = (i + 1) / resolution;
            T p0 = curve->get_point(t);
            T p1 = curve->get_point(t2);
            vertices.push_back(Vertex{p0, glm::vec3(0, 0, 0), glm::vec2(0, 0)});
            vertices.push_back(Vertex{p1, glm::vec3(0, 0, 0), glm::vec2(0, 0)});
            indices.push_back(i * 2);
            indices.push_back(i * 2 + 1);
        }
        update_vertices(vertices);
        update_indices(indices);
    }

    void set_resolution(float resolution)
    {
        this->resolution = resolution;
        generate_curve();
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

    void add_point(T point)
    {
        curve->add_point(point);
        generate_curve();
    }

    bool has_points()
    {
        return this->curve->has_points();
    }
};