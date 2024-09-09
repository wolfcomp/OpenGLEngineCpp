#include "World.h"
#include "objects/base/SceneObject.h"
#include "objects/base/SceneUpdatableObject.h"
#include "collections/QuadTree.h"
#include "objects/debug/Line.h"
#include "objects/debug/Arrow.h"
#include "colliders/ColliderHandler.h"

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

void World::draw_debug(Line *line, Arrow *arrow)
{
    quad_tree.draw_debug(line);
    std::vector<SceneUpdatableObject *> objects;
    quad_tree.query_range(quad_tree.get_bounds(), objects);
    if (objects.size() < 3)
        return;
    SceneUpdatableObject *object1 = objects[0];
    SceneUpdatableObject *object2 = objects[1];
    SceneUpdatableObject *object3 = objects[2];

    auto f = ColliderHandler::get_collision_normal(object1->get_collider(), object2->get_collider());
    arrow->set_position(object1->get_position());
    arrow->set_rotation_world_up(f);
    arrow->draw();
    arrow->set_position(object2->get_position());
    arrow->set_rotation_world_up(-f);
    arrow->draw();
    f = ColliderHandler::get_collision_normal(object1->get_collider(), object3->get_collider());
    arrow->set_position(object1->get_position());
    arrow->set_rotation_world_up(f);
    arrow->draw();
    arrow->set_position(object3->get_position());
    arrow->set_rotation_world_up(-f);
    arrow->draw();
    f = ColliderHandler::get_collision_normal(object2->get_collider(), object3->get_collider());
    arrow->set_position(object2->get_position());
    arrow->set_rotation_world_up(f);
    arrow->draw();
    arrow->set_position(object3->get_position());
    arrow->set_rotation_world_up(-f);
    arrow->draw();
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