#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "colliders/AABB.h"
#include "collections/QuadTree.h"

class SceneObject;

class World
{
    QuadTree<SceneObject *> quadTree;

public:
    World() {}
    ~World() {}

    void insert(SceneObject *object);

    void draw();

    void draw_debug();

    void set_bounds(AABB bounds);
};
