#include "World.h"
#include "objects/base/SceneObject.h"
#include "objects/base/SceneUpdatableObject.h"
#include "collections/QuadTree.h"
#include "objects/debug/Line.h"
#include "objects/debug/Arrow.h"
#include "colliders/ColliderHandler.h"

void World::insert(SceneUpdatableObject *object)
{
    tree.insert(object);
}

void World::draw()
{
    std::vector<SceneUpdatableObject *> objects;
    tree.query_range(tree.get_bounds(), objects);
    for (auto &object : objects)
    {
        object->draw();
    }
}

void World::draw_debug(Line *line, Arrow *arrow)
{
    tree.draw_debug(line);
}

void World::set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
{
    tree.set_bounds(center, extent);
}

void World::update(float delta_time)
{
    std::vector<SceneUpdatableObject *> objects;
    AABB bounds = tree.get_bounds();
    tree.query_range(bounds, objects);
    unsigned index = 0;
    for (auto &object : objects)
    {
        if (object != nullptr)
        {
            if (object->get_active())
            {
                auto model = object->get_model_matrix();
                auto minVertex = glm::vec3(model * glm::vec4(object->get_min_vertex().position, 1));
                auto maxVertex = glm::vec3(model * glm::vec4(object->get_max_vertex().position, 1));
                std::vector<SceneUpdatableObject *> in_range_objects;
                bounds.center = (minVertex + maxVertex) / 2.0f; // center of the object
                bounds.extent = (maxVertex - minVertex) * 2.0f; // double the size of the object
                tree.query_range(bounds, in_range_objects);
                bool collided = false;
                for (auto &in_range_object : in_range_objects)
                {
                    if (in_range_object->get_active() && in_range_object != object)
                    {
                        // TODO: figure out why recalculating when balls overlap doesn't work properly (balls get stuck together for a few seconds before separating)
                        if (ColliderHandler::contains(in_range_object->get_collider(), object->get_collider()))
                            object->apply_collision(in_range_object);
                        // auto cd = object->get_collider()->collision_delta(in_range_object->get_collider(), delta_time);
                        // if (cd < 1)
                        // {
                        //     auto colliding_delta_time = cd * delta_time;
                        //     object->pre_update(-colliding_delta_time);
                        //     object->update(-colliding_delta_time);
                        //     object->apply_collision(in_range_object);
                        //     object->pre_update(delta_time + colliding_delta_time);
                        //     object->update(delta_time + colliding_delta_time);
                        //     collided = true;
                        //     break;
                        // }
                    }
                }
                if (!collided)
                {
                    object->pre_update(delta_time);
                    object->update(delta_time);
                }
                if (!tree.get_bounds().contains(object->get_position()))
                {
                    // ball went out of bounds the last frame revert
                    auto pos = object->get_position();
                    object->update(-delta_time);
                    auto new_pos = object->get_position();
                    bounds = tree.get_bounds();
                    // find out how much the ball went out of bounds by in delta
                    auto col_delta = glm::clamp(pos, -bounds.extent, bounds.extent);
                    pos = pos - col_delta;
                    // find which side the ball is closest to
                    auto normal = ColliderHandler::get_collision_normal(&bounds, object->get_collider());
                    // apply the collision
                    object->apply_collision(normal);
                    // update the object
                    object->update(delta_time);
                }
            }
        }
        index++;
    }
    tree.recalculate();
}