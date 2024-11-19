#include "physics.h"
#include "../components/physics.h"
#include "../components/transform.h"

void PhysicsSystem::update(float delta_time)
{
    auto ecs = get_ecs();
    auto physics = ecs->get<PhysicsComponent>();
    if (physics == nullptr)
        return;
    for (int i = 0; i < physics->get_size(); i++)
    {
        auto component = physics->get_value_pair<PhysicsComponent>(i);
        if (component == nullptr)
            continue;
        component->value->update(delta_time);
        component->value->apply_gravity(9.8f);
        auto position = ecs->get<TransformComponent>(component->id);
        if (position != nullptr)
        {
            position->position += component->value->velocity * delta_time;
        }
        delete component;
    }
}