#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/QuadTree.h"
#include "collections/OcTree.h"

class SceneUpdatableObject;
class Arrow;

class World
{
    OcTree<SceneUpdatableObject *> tree;

public:
    World() {}
    ~World()
    {
        clear();
    }

    void insert(SceneUpdatableObject *object);

    void draw();

    void draw_debug(Line *line, Arrow *arrow);

    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent);

    void update(float delta_time);

    void clear()
    {
        tree.clear();
    }
};
