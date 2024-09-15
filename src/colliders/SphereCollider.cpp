#include "SphereCollider.h"
#include "../objects/base/SceneObject.h"

void SphereCollider::update(SceneObject *object)
{
    auto scale = object->get_scale();
    auto max_scale = glm::max(scale.x, glm::max(scale.y, scale.z));
    radius = max_scale;
}

glm::vec3 SphereCollider::get_center() { return get_parent()->get_position(); }