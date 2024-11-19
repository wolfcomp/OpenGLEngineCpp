#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/QuadTree.h"
#include "collections/OcTree.h"
#include "culling/Frustum.h"
#include "Light.h"
#include "ecs/ecs_map.h"
#include "ecs/system/base.h"

class GameObject;
class Arrow;

struct DrawCounts
{
    unsigned objects_culled;
    unsigned objects_filtered;
    unsigned objects_drawn;
};

class World
{
public:
    static constexpr unsigned MAX_POINT_LIGHTS = 4;

private:
    OcTree<GameObject *> tree;
    std::vector<GameObject *> objects_non_colliders;
    PointLight *pointLights[MAX_POINT_LIGHTS];
    DirectionalLight *directionalLight = nullptr;
    SpotLight *spotLight = nullptr;
    ECSGlobalMap ecs;
    std::vector<BaseSystem *> systems;

public:
    World()
    {
        for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        {
            pointLights[i] = nullptr;
        }
    }
    ~World()
    {
        clear();
    }

    void insert(GameObject *object);

    DrawCounts draw(Frustum *frustum);

    void draw_debug(Line *line, Arrow *arrow);

    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent);

    void update(float delta_time);
    void set_point_light(PointLight *light, unsigned index);
    void set_directional_light(DirectionalLight *light);
    void set_spot_light(SpotLight *light);
    void update_point_light(unsigned index, std::function<void(PointLight *)> update);
    void update_directional_light(std::function<void(DirectionalLight *)> update);
    void update_spot_light(std::function<void(SpotLight *)> update);
    DirectionalLight *get_directional_light() { return directionalLight; }
    PointLight *get_point_light(unsigned index) { return pointLights[index]; }
    SpotLight *get_spot_light() { return spotLight; }
    ECSGlobalMap *get_ecs() { return &ecs; }
    void register_system(BaseSystem *system) { systems.push_back(system); }

    void set_shader(const Shader *shader)
    {
        for (auto light : pointLights)
        {
            if (light != nullptr)
                light->set_shader(shader);
        }
        if (directionalLight != nullptr)
            directionalLight->set_shader(shader);
        if (spotLight != nullptr)
            spotLight->set_shader(shader);
    }

    void clear()
    {
        tree.clear();
        for (auto object : objects_non_colliders)
        {
            delete object;
        }
        objects_non_colliders.clear();
        for (auto light : pointLights)
        {
            delete light;
        }
        delete directionalLight;
        delete spotLight;
    }
};
