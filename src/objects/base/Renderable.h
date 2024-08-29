#pragma once

#include <vector>
#include "Vertex.h"

class Renderable
{
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;

public:
    Renderable(std::vector<Vertex> vertices, std::vector<unsigned> indices) : vertices(vertices), indices(indices) {};
    ~Renderable()
    {
        vertices.clear();
        indices.clear();
    };

    virtual void render() const = 0;
};
