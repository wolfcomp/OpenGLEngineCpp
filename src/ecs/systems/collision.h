#pragma once

#include "base.h"

class CollisionSystem : public BaseSystem
{
public:
    CollisionSystem(ECSGlobalMap *ecs, World *world) : BaseSystem(ecs, world) {}
    void update(float delta_time) override;
};