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
        if (std::get<0>(y) + scale.y > transform->position.y)
        {
            component->value->acceleration.y = 0;
            component->value->apply_force(std::get<1>(y) + glm::normalize(std::get<1>(y)) * scale);
            transform->position.y = std::get<0>(y) + scale.y;
        }
        // for (int j = 0; j < physics->get_size(); j++)
        // {
        //     auto other = physics->get_value_pair<PhysicsComponent>(j);
        //     if (other == nullptr)
        //         continue;
        //     if (component->id == other->id)
        //         continue;
        //     auto other_object = world->get_object(other->id);
        //     if (other_object == nullptr)
        //         continue;
        //     auto other_object_collider = dynamic_cast<SphereCollider *>(other_object->get_collider());
        //     if (other_object_collider == nullptr)
        //         continue;
        //     if (collider->contains(*other_object_collider))
        //     {
        //         auto other_transform = other_object->get_component<TransformComponent>();
        //         if (other_transform == nullptr)
        //             continue;
        //         auto normal = glm::normalize(transform->position - other_transform->position);
        //         component->value->apply_collision(other->value, normal);
        //     }
        // }
    }
}