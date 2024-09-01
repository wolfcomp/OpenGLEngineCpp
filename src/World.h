#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/OcTree.h"

class SceneObject;

class World
{
    OcTree<SceneObject *> ocTree;

public:
    World() {}
    ~World()
    {
        std::vector<SceneObject *> objects;
        ocTree.query_range(ocTree.get_bounds(), objects);
        for (auto &object : objects)
        {
            delete object;
        }
    }

    void insert(SceneObject *object);

    void draw();

    void draw_debug();

    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent);
};
