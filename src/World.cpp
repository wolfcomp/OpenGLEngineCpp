#include "World.h"
#include "objects/base/SceneObject.h"
#include "objects/base/SceneUpdatableObject.h"
#include "collections/QuadTree.h"
#include "objects/debug/Line.h"

void World::insert(SceneUpdatableObject *object)
{
    quad_tree.insert(object);
}

void World::draw()
{
    std::vector<SceneUpdatableObject *> objects;
    quad_tree.query_range(quad_tree.get_bounds(), objects);
    for (auto &object : objects)
    {
        object->draw();
    }
}

void World::draw_debug(Line *line)
{
    quad_tree.draw_debug(line);
}

void World::set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
{
    quad_tree.set_bounds(center, extent);
}

void World::update(float delta_time)
{
    std::vector<SceneUpdatableObject *> objects;
    quad_tree.query_range(quad_tree.get_bounds(), objects);
    for (auto &object : objects)
    {
        if (object != nullptr)
        {
            object->pre_update(delta_time);
            if (object->get_active())
                object->update(delta_time);
        }
    }
    quad_tree.recalculate();
}