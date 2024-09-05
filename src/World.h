#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/QuadTree.h"

class SceneUpdatableObject;

class World
{
    QuadTree<SceneUpdatableObject *> quad_tree;

public:
    World() {}
    ~World()
    {
        std::vector<SceneUpdatableObject *> objects;
        quad_tree.query_range(quad_tree.get_bounds(), objects);
        for (auto &object : objects)
        {
            delete object;
        }
    }

    void insert(SceneUpdatableObject *object);

    void draw();

    void draw_debug(Line *line);

    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent);

    void update(float delta_time);
};
