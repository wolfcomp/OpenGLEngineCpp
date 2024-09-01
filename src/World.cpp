#include "World.h"
#include "objects/base/SceneObject.h"
#include "collections/QuadTree.h"

void World::insert(SceneObject *object)
{
    ocTree.insert(object);
}

void World::draw()
{
    std::vector<SceneObject *> objects;
    ocTree.query_range(ocTree.get_bounds(), objects);
    for (auto &object : objects)
    {
        object->draw();
    }
}

void World::draw_debug()
{
    ocTree.draw_debug();
}

void World::set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
{
    ocTree.set_bounds(center, extent);
}