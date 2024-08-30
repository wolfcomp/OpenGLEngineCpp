#include "World.h"
#include "objects/base/SceneObject.h"
#include "collections/QuadTree.h"
#include "colliders/AABB.h"

void World::insert(SceneObject *object)
{
    quadTree.insert(object);
}

void World::draw()
{
    std::vector<SceneObject *> objects;
    quadTree.query_range(quadTree.get_bounds(), objects);
    for (auto &object : objects)
    {
        object->draw();
    }
}

void World::draw_debug()
{
    quadTree.draw_debug();
}

void World::set_bounds(AABB bounds)
{
    quadTree.set_bounds(bounds);
}