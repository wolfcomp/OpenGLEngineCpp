#pragma once

#include "../base/GameObject.h"
#include <vector>
#include <glm/gtx/vector_angle.hpp>

const std::vector<Vertex> vertexes = {
    Vertex({0.05f, 0, 0}),
    Vertex({0, 0, 0.05f}),
    Vertex({0, 0, -0.05f}),
    Vertex({-0.05f, 0, 0}),
    Vertex({0.05f, 1, 0}),
    Vertex({0, 1, 0.05f}),
    Vertex({0, 1, -0.05f}),
    Vertex({-0.05f, 1, 0}),
    Vertex({0.2f, 1, 0}),
    Vertex({0, 1, 0.2f}),
    Vertex({0, 1, -0.2f}),
    Vertex({-0.2f, 1, 0}),
    Vertex({0, 1.4f, 0}),
};
const std::vector<unsigned> vindices = { // base
    0, 1, 2, 1, 3, 2,

    // shaft
    0, 4, 1, 1, 4, 5,

    1, 7, 3, 1, 7, 5,

    2, 6, 3, 3, 6, 7,

    2, 6, 0, 0, 6, 4,

    // arrow base
    8, 9, 10, 9, 11, 10,

    // arrow head
    8, 9, 12, 9, 10, 12,

    10, 11, 12, 11, 8, 12};

class Arrow : public GameObject
{
    glm::mat4 model;

public:
    Arrow() : GameObject(vertexes, vindices, nullptr)
    {
    }

    void pre_render() const override
    {
        GameObject::pre_render();
        glLineWidth(5);
    }

    void post_render() const override
    {
        glLineWidth(1);
        GameObject::post_render();
    }
};