#include "AABB.h"
#include "../objects/debug/Line.h"
#include "../ShaderStore.h"
#include "../Material.h"
#include "../objects/base/SceneObject.h"
#include "../objects/base/SceneUpdatableObject.h"

AABB::AABB(glm::vec3 center, glm::vec3 extent) : center(center), extent(extent)
{
}
AABB::AABB() : center(glm::vec3(0.0f)), extent(glm::vec3(0.0f))
{
}
AABB::~AABB()
{
}

void AABB::draw_debug(Line *line)
{
    glm::vec3 min = center - extent;
    glm::vec3 max = center + extent;
    line->set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(max.x, min.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(max.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z));
    line->draw();
}

template <>
bool AABB::contains<SceneObject *>(SceneObject *const &point) const
{
    return contains(point->get_position());
}

template <>
bool AABB::contains<SceneUpdatableObject *>(SceneUpdatableObject *const &point) const
{
    return contains(point->get_position());
}