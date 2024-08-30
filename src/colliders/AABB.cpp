#include "AABB.h"
#include "../objects/debug/Line.h"
#include "../ShaderStore.h"
#include "../Material.h"
#include "../objects/base/SceneObject.h"

void AABB::draw_debug()
{
    Line line;
    line.set_shader(ShaderStore::get_shader("noLight"));
    line.set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(line.get_material())->color = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 min = center - extent;
    glm::vec3 max = center + extent;
    line.set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z));
    line.draw();
    line.set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z));
    line.draw();
    line.set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z));
    line.draw();
    line.set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z));
    line.draw();
    line.set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(max.x, min.y, max.z));
    line.draw();
    line.set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(max.x, max.y, min.z));
    line.draw();
    line.set_positions(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z));
    line.draw();
    line.set_positions(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z));
    line.draw();
    line.set_positions(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z));
    line.draw();
    line.set_positions(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z));
    line.draw();
    line.set_positions(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z));
    line.draw();
    line.set_positions(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z));
    line.draw();
}

template <>
bool AABB::contains<SceneObject *>(const SceneObject *&point) const
{
    return contains(point->get_position());
}