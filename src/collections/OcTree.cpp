#include "OcTree.h"
#include "../objects/base/GameObject.h"
#include "../objects/base/GameObject.h"
#include "../objects/debug/Line.h"
#include <vector>

template <typename T>
void OcTree<T>::recalculate()
{
    throw "Cannot recalculate OcTree of unknown type";
}

template <>
void OcTree<GameObject *>::recalculate()
{
    std::vector<GameObject *> all_objects;
    query<GameObject *>(all_objects);

    glm::vec3 min = glm::vec3(FLT_MAX);
    glm::vec3 max = glm::vec3(FLT_MIN);
    for (auto &object : all_objects)
    {
        pop(object);
        auto transform = object->get_component<TransformComponent>();
        if (transform == nullptr)
            continue;
        auto position = transform->position;
        min = glm::min(min, position);
        max = glm::max(max, position);
    }
    if (min == glm::vec3(FLT_MAX) || max == glm::vec3(FLT_MIN))
        return;
    auto center = (min + max) / 2.0f;
    auto extent = (max - min) / 2.0f;
    set_bounds(center, extent);
    unsubdivide();
    for (auto &object : all_objects)
    {
        insert(object);
    }
}

void OcTreeBase::draw_debug(Line *line, bool draw_bounds)
{
    if (draw_bounds)
    {
        boundary.draw_debug(line);
    }
    if (is_leaf())
        return;
    auto center = boundary.center;
    auto extent = boundary.extent;
    auto extentDivide = extent / 2.0f;
    line->set_positions(center + glm::vec3(-extent.x, 0, 0), center + glm::vec3(extent.x, 0, 0));
    line->draw();
    line->set_positions(center + glm::vec3(0, -extent.y, 0), center + glm::vec3(0, extent.y, 0));
    line->draw();
    line->set_positions(center + glm::vec3(0, 0, -extent.z), center + glm::vec3(0, 0, extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(extent.x, -extent.y, 0), center + glm::vec3(extent.x, extent.y, 0));
    line->draw();
    line->set_positions(center + glm::vec3(extent.x, 0, -extent.z), center + glm::vec3(extent.x, 0, extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(-extent.x, -extent.y, 0), center + glm::vec3(-extent.x, extent.y, 0));
    line->draw();
    line->set_positions(center + glm::vec3(-extent.x, 0, -extent.z), center + glm::vec3(-extent.x, 0, extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(-extent.x, extent.y, 0), center + glm::vec3(extent.x, extent.y, 0));
    line->draw();
    line->set_positions(center + glm::vec3(0, extent.y, -extent.z), center + glm::vec3(0, extent.y, extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(-extent.x, -extent.y, 0), center + glm::vec3(extent.x, -extent.y, 0));
    line->draw();
    line->set_positions(center + glm::vec3(0, -extent.y, -extent.z), center + glm::vec3(0, -extent.y, extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(-extent.x, 0, extent.z), center + glm::vec3(extent.x, 0, extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(0, -extent.y, extent.z), center + glm::vec3(0, extent.y, extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(-extent.x, 0, -extent.z), center + glm::vec3(extent.x, 0, -extent.z));
    line->draw();
    line->set_positions(center + glm::vec3(0, -extent.y, -extent.z), center + glm::vec3(0, extent.y, -extent.z));
    line->draw();
}