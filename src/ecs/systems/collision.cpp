#include "collision.h"
#include "../components/physics.h"
#include "../components/transform.h"
#include "../../World.h"
#include "../../objects/base/GameObject.h"
#include "../../colliders/mesh.h"
#include "../../colliders/SphereCollider.h"
#include "../../objects/curves/BSplineSurface.h"

void CollisionSystem::update(float delta_time)
{
    auto ecs = get_ecs();
    auto physics = ecs->get<PhysicsComponent>();
    if (physics == nullptr)
        return;
    auto world = get_world();
    auto surface = dynamic_cast<BSplineSurface *>(world->get_object(world->get_surface_id()));
    if (surface == nullptr)
        return;
    for (int i = 0; i < physics->get_size(); i++)
    {
        auto component = physics->get_value_pair<PhysicsComponent>(i);
        if (component == nullptr)
            continue;
        auto object = world->get_object(component->id);
        if (object == nullptr)
            continue;
        auto transform = object->get_component<TransformComponent>();
        if (transform == nullptr)
            continue;
        auto collider = dynamic_cast<SphereCollider *>(object->get_collider());
        auto y = surface->get_y_at(transform->position);
        auto scale = transform->scale;
        if (std::get<0>(y) > transform->position.y + scale.y)
        {
            // component->value->acceleration = glm::vec3(0);
            component->value->apply_force(std::get<1>(y) + glm::normalize(std::get<1>(y)) * scale);
            transform->position.y = std::get<0>(y);
        }
    }
}