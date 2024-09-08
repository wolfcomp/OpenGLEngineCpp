#include "SphereCollider.h"
#include "../objects/base/SceneObject.h"

void SphereCollider::update(SceneObject *object)
{
    center = object->get_position();
    radius = glm::length(object->get_scale());
}