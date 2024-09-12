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
}

void World::set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
{
    quad_tree.set_bounds(center, extent);
}

void World::update(float delta_time)
{
    std::vector<SceneUpdatableObject *> objects;
    AABB bounds = quad_tree.get_bounds();
    quad_tree.query_range(bounds, objects);
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
                bounds.center = (minVertex + maxVertex) / 2.0f;
                bounds.extent = maxVertex - minVertex;
                quad_tree.query_range(bounds, in_range_objects);
                for (auto &in_range_object : in_range_objects)
                {
                    if (in_range_object->get_active() && in_range_object != object)
                    {
                        if (ColliderHandler::contains(in_range_object->get_collider(), object->get_collider()))
                        {
                            object->apply_collision(in_range_object);
                        }
                    }
                }
                object->pre_update(delta_time);
                object->update(delta_time);
                if (!quad_tree.get_bounds().contains(object->get_position()))
                {
                    // ball went out of bounds the last frame revert
                    auto pos = object->get_position();
                    object->update(-delta_time);
                    // find which side the ball is closest to
                    auto center = quad_tree.get_bounds().center;
                    auto extent = quad_tree.get_bounds().extent;
                    auto diff = pos - center;
                    auto closest = glm::vec3(0);
                    if (abs(diff.x) > extent.x)
                    {
                        closest.x = extent.x * (diff.x < 0 ? -1 : 1);
                    }
                    else if (abs(diff.y) > extent.y)
                    {
                        closest.y = extent.y * (diff.y < 0 ? -1 : 1);
                    }
                    else if (abs(diff.z) > extent.z)
                    {
                        closest.z = extent.z * (diff.z < 0 ? -1 : 1);
                    }
                    auto normal = glm::normalize(closest - pos);
                    // only apply the normal of the closest side
                    if (normal.x > normal.y && normal.x > normal.z)
                    {
                        normal.y = 0;
                        normal.z = 0;
                    }
                    else if (normal.y > normal.x && normal.y > normal.z)
                    {
                        normal.x = 0;
                        normal.z = 0;
                    }
                    else
                    {
                        normal.x = 0;
                        normal.y = 0;
                    }
                    // apply the collision
                    object->apply_collision(normal);
                    // update the object
                    object->update(delta_time);
                }
            }
        }
        index++;
    }
    quad_tree.recalculate();
}