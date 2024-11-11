#pragma once

#include <glm/vec3.hpp>
#include "base.h"

class PhysicsSystem : public BaseSystem
{
public:
    PhysicsSystem(ECSGlobalMap *ecs, World *world) : BaseSystem(ecs, world) {}
    void update(float delta_time) override;
};