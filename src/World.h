#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/QuadTree.h"
#include "collections/OcTree.h"
#include "culling/Frustum.h"

class GameObject;
class Arrow;

struct DrawCounts
{
    unsigned objects_total;
    unsigned objects_filtered;
    unsigned objects_drawn;
};

class World
{
    OcTree<GameObject *> tree;
    std::vector<GameObject *> objects_non_colliders;

public:
    World() {}
    ~World()
    {
        clear();
    }

    void insert(GameObject *object);

    DrawCounts draw(Frustum *frustum);

    void draw_debug(Line *line, Arrow *arrow);

    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent);

    void update(float delta_time);

    void clear()
    {
        tree.clear();
    }
};
