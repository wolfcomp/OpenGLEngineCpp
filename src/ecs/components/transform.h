#pragma once

#include <glm/vec3.hpp>
#include "base.h"

struct TransformComponent : public BaseComponent
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};