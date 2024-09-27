#include "ConvexHull.h"
#include "../shaders/ShaderStore.h"
#include "../shaders/Material.h"
#include "../objects/base/GameObject.h"

ConvexHull::ConvexHull(AABB bounds, GameObjectBase *parent) : parent(parent)
{
    hull = new GameObjectBase();
    hull->set_shader(ShaderStore::get_shader("noLight"));
    hull->set_mode(GL_LINE);
    hull->set_material(new ColorMaterial());
    dynamic_cast<ColorMaterial *>(hull->get_material())->color = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);

    auto min = bounds.center - bounds.extent;
    auto max = bounds.center + bounds.extent;

    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(min.x, min.y, min.z)),
        Vertex(glm::vec3(max.x, min.y, min.z)),
        Vertex(glm::vec3(min.x, min.y, max.z)),
        Vertex(glm::vec3(max.x, min.y, max.z)),
        Vertex(glm::vec3(min.x, max.y, min.z)),
        Vertex(glm::vec3(max.x, max.y, min.z)),
        Vertex(glm::vec3(min.x, max.y, max.z)),
        Vertex(glm::vec3(max.x, max.y, max.z))};

    std::vector<unsigned> indices = {
        0, 1,
        0, 2,
        1, 3,
        2, 3,
        4, 5,
        4, 6,
        5, 7,
        6, 7,
        0, 4,
        1, 5,
        2, 6,
        3, 7};

    hull->update_vertices(vertices);
    hull->update_indices(indices);
}

void ConvexHull::gift_wrap(GameObjectBase *GameObjectBase)
{
    // TODO implement gift wrapping algorithm
}

void ConvexHull::quick_hull(GameObjectBase *GameObjectBase)
{
    // TODO implement quick hull algorithm
}

void ConvexHull::draw_debug()
{
    hull->draw();
}

template <>
bool ConvexHull::contains<ConvexHull>(const ConvexHull &collider) const
{
    // TODO implement intersection check with GJK algorithm
    return false;
}