#pragma once

#include "../base/SceneUpdatableObject.h"
#include "../../colliders/AABB.h"

class Cube : public SceneUpdatableObject
{
public:
    Cube() : SceneUpdatableObject({// face 1
                                   Vertex({-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}),
                                   Vertex({1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}),
                                   Vertex({1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}),
                                   Vertex({-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}),
                                   // face 2
                                   Vertex({-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),
                                   Vertex({1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}),
                                   Vertex({1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}),
                                   Vertex({-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}),
                                   // face 3
                                   Vertex({-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}),
                                   Vertex({1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}),
                                   Vertex({1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}),
                                   Vertex({-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}),
                                   // face 4
                                   Vertex({-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}),
                                   Vertex({1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}),
                                   Vertex({1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}),
                                   Vertex({-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}),
                                   // face 5
                                   Vertex({1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
                                   Vertex({1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
                                   Vertex({1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
                                   Vertex({1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
                                   // face 6
                                   Vertex({-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
                                   Vertex({-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
                                   Vertex({-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
                                   Vertex({-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f})},
                                  {// face 1
                                   0, 1, 2, 2, 3, 0,
                                   // face 2
                                   4, 5, 6, 6, 7, 4,
                                   // face 3
                                   8, 9, 10, 10, 11, 8,
                                   // face 4
                                   12, 13, 14, 14, 15, 12,
                                   // face 5
                                   16, 17, 18, 18, 19, 16,
                                   // face 6
                                   20, 21, 22, 22, 23, 20})
    {
        set_collider(new AABB());
    }
    Cube(glm::vec3 position) : Cube()
    {
        set_position(position);
    }
    ~Cube() {}

    AABB *get_collider() override { return dynamic_cast<AABB *>(SceneObject::get_collider()); }
};