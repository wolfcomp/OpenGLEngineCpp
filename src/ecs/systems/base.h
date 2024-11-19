#pragma once
#include "../ecs_map.h"

class World;

class BaseSystem
{
private:
    ECSGlobalMap *ecs;
    World *world;

public:
    BaseSystem(ECSGlobalMap *ecs, World *world) : ecs(ecs), world(world) {};
    virtual ~BaseSystem() { ecs = nullptr; };
    virtual void update(float delta_time) = 0;
    ECSGlobalMap *get_ecs() { return ecs; };
};