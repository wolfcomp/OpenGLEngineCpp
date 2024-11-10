#pragma once

#include <glm/vec3.hpp>
#include "base.h"

struct PhysicsComponent : public BaseComponent
{
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
};