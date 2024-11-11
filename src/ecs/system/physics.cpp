#include "physics.h"
#include "../components/physcis.h"

void PhysicsSystem::update(float delta_time)
{
    auto ecs = get_ecs();
    auto physics = ecs->get<PhysicsComponent>();
    if (physics == nullptr)
        return;
    for (int i = 0; i < physics->get_size(); i++)
    {
        auto component = physics->get(i);
        if (component == nullptr)
            continue;
        dynamic_cast<PhysicsComponent *>(component)->update(delta_time);
    }
}