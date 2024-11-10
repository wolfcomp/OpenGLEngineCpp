#pragma once

#include <glm/vec3.hpp>
#include "base.h"

class PhysicsSystem : public BaseSystem
{
public:
    void update(float delta_time) override;
};