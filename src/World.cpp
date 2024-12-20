#include "World.h"
#include "objects/base/GameObject.h"
#include "objects/base/GameObject.h"
#include "collections/QuadTree.h"
#include "objects/debug/Line.h"
#include "objects/debug/Arrow.h"
#include "colliders/ColliderHandler.h"
#include <imgui/imgui.h>

glm::vec3 checkLoc = glm::vec3(0, 0, 0);

void World::insert(GameObject *object)
{
    object->attatch_to_world(this);
    object->register_ecs(&ecs);
    if (object->get_collider() == nullptr)
    {
        objects_non_colliders.push_back(object);
        return;
    }
    tree.insert(object);
}
void World::insert(GameObject *object, glm::vec3 position)
{
    object->attatch_to_world(this);
    object->register_ecs(&ecs);
    object->get_component<TransformComponent>()->set_position(position);
    if (object->get_collider() == nullptr)
    {
        objects_non_colliders.push_back(object);
        return;
    }
    tree.insert(object);
}
void World::insert(GameObject *object, glm::vec3 position, glm::vec3 scale)
{
    object->attatch_to_world(this);
    object->register_ecs(&ecs);
    auto transform = object->get_component<TransformComponent>();
    transform->set_position(position);
    transform->set_scale(scale);
    if (object->get_collider() == nullptr)
    {
        objects_non_colliders.push_back(object);
        return;
    }
    tree.insert(object);
}
void World::insert(GameObject *object, glm::vec3 position, glm::vec3 scale, glm::quat rotation)
{
    object->attatch_to_world(this);
    object->register_ecs(&ecs);
    auto transform = object->get_component<TransformComponent>();
    transform->set_position(position);
    transform->set_scale(scale);
    transform->set_rotation(rotation);
    if (object->get_collider() == nullptr)
    {
        objects_non_colliders.push_back(object);
        return;
    }
    tree.insert(object);
}

DrawCounts World::draw(Frustum *frustum)
{
    std::vector<GameObject *> objects;
    DrawCounts counts = {0, 0, 0};
    auto tuple = tree.query_range(tree.get_bounds(), objects, frustum);
    counts.objects_culled = std::get<0>(tuple);
    counts.objects_filtered = std::get<1>(tuple);
    for (auto &object : objects)
    {
        if ((object->get_collider()->is_on_frustum(frustum) || object->get_uuid() == surface_id) && object->should_render())
        {
            counts.objects_drawn++;
            object->draw();
        }
    }
    for (auto &object : objects_non_colliders)
    {
        if (object->should_render())
        {
            counts.objects_drawn++;
            object->draw();
        }
    }
    return counts;
}

void World::draw_debug(Line *line, Arrow *arrow)
{
    tree.draw_debug(line);
    if (directionalLight != nullptr)
    {
        auto dir = glm::normalize(directionalLight->direction);
        arrow->get_component<TransformComponent>()->set_position(glm::vec3(0));
        auto zAngle = glm::dot(dir, glm::vec3(0, 0, 1));
        auto xAngle = glm::dot(dir, glm::vec3(1, 0, 0));
        auto yAngle = glm::dot(dir, glm::vec3(0, 1, 0));
        arrow->get_component<TransformComponent>()->set_rotation(glm::vec3(xAngle, yAngle, zAngle));
        dynamic_cast<ColorMaterial *>(arrow->get_material())->color = glm::vec4(directionalLight->ambient.get_rgb_vec3(), 1.0f);
        arrow->draw();
    }
}

void World::set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
{
    tree.set_bounds(center, extent);
}

void World::update(float delta_time)
{
    for (auto &system : systems)
    {
        system->update(delta_time);
    }
    std::vector<GameObject *> objects;
    AABB bounds = tree.get_bounds();
    tree.query_range(bounds, objects);
    for (auto &object : objects)
    {
        if (object != nullptr)
        {
            if (object->get_active())
            {
                object->update(delta_time);
            }
        }
    }
    tree.recalculate();
}

void World::set_directional_light(DirectionalLight *light)
{
    directionalLight = light;
}

void World::set_spot_light(SpotLight *light)
{
    spotLight = light;
}

void World::set_point_light(PointLight *light, unsigned index)
{
    if (index >= MAX_POINT_LIGHTS)
        throw std::runtime_error("Index out of bounds");
    pointLights[index] = light;
}

void World::update_point_light(unsigned index, std::function<void(PointLight *)> update)
{
    if (index >= MAX_POINT_LIGHTS)
        throw std::runtime_error("Index out of bounds");
    update(pointLights[index]);
}

void World::update_directional_light(std::function<void(DirectionalLight *)> update)
{
    update(directionalLight);
}

void World::update_spot_light(std::function<void(SpotLight *)> update)
{
    update(spotLight);
}

void World::draw_light_editor()
{
    ImGui::Text("Directional Light");
    auto color = directionalLight->ambient.get_hsv();
    if (ImGui::ColorEdit3("Ambient", color, ImGuiColorEditFlags_InputHSV))
        directionalLight->ambient.set_from_hsv(color);
    color = directionalLight->diffuse.get_hsv();
    if (ImGui::ColorEdit3("Diffuse", color, ImGuiColorEditFlags_InputHSV))
        directionalLight->diffuse.set_from_hsv(color);
    color = directionalLight->specular.get_hsv();
    if (ImGui::ColorEdit3("Specular", color, ImGuiColorEditFlags_InputHSV))
        directionalLight->specular.set_from_hsv(color);
    ImGui::Text("Direction");
    ImGui::SliderFloat("X", &directionalLight->direction.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Y", &directionalLight->direction.y, -1.0f, 1.0f);
    ImGui::SliderFloat("Z", &directionalLight->direction.z, -1.0f, 1.0f);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        if (pointLights[i] != nullptr)
        {
            ImGui::Text("Point Light %d", i);
            color = pointLights[i]->ambient.get_hsv();
            if (ImGui::ColorEdit3("Ambient", color, ImGuiColorEditFlags_InputHSV))
                pointLights[i]->ambient.set_from_hsv(color);
            color = pointLights[i]->diffuse.get_hsv();
            if (ImGui::ColorEdit3("Diffuse", color, ImGuiColorEditFlags_InputHSV))
                pointLights[i]->diffuse.set_from_hsv(color);
            color = pointLights[i]->specular.get_hsv();
            if (ImGui::ColorEdit3("Specular", color, ImGuiColorEditFlags_InputHSV))
                pointLights[i]->specular.set_from_hsv(color);
            ImGui::SliderFloat("Constant", &pointLights[i]->constant, 0.0f, 1.0f);
            ImGui::SliderFloat("Linear", &pointLights[i]->linear, 0.0f, 1.0f);
            ImGui::SliderFloat("Quadratic", &pointLights[i]->quadratic, 0.0f, 1.0f);
            ImGui::SliderFloat("X", &pointLights[i]->position.x, -10.0f, 10.0f);
            ImGui::SliderFloat("Y", &pointLights[i]->position.y, -10.0f, 10.0f);
            ImGui::SliderFloat("Z", &pointLights[i]->position.z, -10.0f, 10.0f);
        }
    }
    if (spotLight != nullptr)
    {
        ImGui::Text("Spot Light");
        color = spotLight->ambient.get_hsv();
        if (ImGui::ColorEdit3("Ambient", color, ImGuiColorEditFlags_InputHSV))
            spotLight->ambient.set_from_hsv(color);
        color = spotLight->diffuse.get_hsv();
        if (ImGui::ColorEdit3("Diffuse", color, ImGuiColorEditFlags_InputHSV))
            spotLight->diffuse.set_from_hsv(color);
        color = spotLight->specular.get_hsv();
        if (ImGui::ColorEdit3("Specular", color, ImGuiColorEditFlags_InputHSV))
            spotLight->specular.set_from_hsv(color);
        ImGui::SliderFloat("Constant", &spotLight->constant, 0.0f, 1.0f);
        ImGui::SliderFloat("Linear", &spotLight->linear, 0.0f, 1.0f);
        ImGui::SliderFloat("Quadratic", &spotLight->quadratic, 0.0f, 1.0f);
        ImGui::SliderFloat("CutOff", &spotLight->cutOff, 0.0f, 90.0f);
        ImGui::SliderFloat("OuterCutOff", &spotLight->outerCutOff, 0.0f, 90.0f);
        ImGui::SliderFloat("X", &spotLight->position.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Y", &spotLight->position.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Z", &spotLight->position.z, -10.0f, 10.0f);
        ImGui::SliderFloat("Direction X", &spotLight->direction.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Direction Y", &spotLight->direction.y, -1.0f, 1.0f);
        ImGui::SliderFloat("Direction Z", &spotLight->direction.z, -1.0f, 1.0f);
    }
}

GameObject *World::get_object(UUID id)
{
    auto object = tree.get_node([&id](GameObject *object)
                                { return object->get_uuid() == id; });
    if (object == nullptr)
    {
        for (auto &object : objects_non_colliders)
        {
            if (object->get_uuid() == id)
                return object;
        }
    }
    return object;
}
