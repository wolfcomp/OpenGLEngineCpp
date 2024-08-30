#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/QuadTree.h"
#include "objects/base/Renderable.h"

class World
{
    QuadTree<Renderable> quadTree;

public:
    World() {}
    ~World() {}

    void insert(Renderable object)
    {
        quadTree.insert(object);
    }

    void draw()
    {
        std::vector<Renderable> objects;
        quadTree.query_range(quadTree.get_bounds(), objects);
        for (auto &object : objects)
        {
            object.pre_render();
            object.render();
            object.post_render();
        }
    }

    void draw_debug()
    {
        quadTree.draw_debug();
    }
};
