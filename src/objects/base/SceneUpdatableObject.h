#pragma once

#include "SceneObject.h"

class SceneUpdatableObject : public SceneObject
{
private:
    bool is_active;
    glm::vec3 velocity;
    glm::vec3 acceleration;

public:
};