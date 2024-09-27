#include "SphereCollider.h"
#include "../objects/base/GameObject.h"

void SphereCollider::update(GameObject *object)
{
    auto scale = object->get_scale();
    auto max_scale = glm::max(scale.x, glm::max(scale.y, scale.z));
    radius = max_scale;
}

glm::vec3 SphereCollider::get_center() { return get_parent()->get_position(); }

bool SphereCollider::is_on_frustum(Frustum *frustum)
{
    return is_on_or_forward_plane(&frustum->left_face) &&
           is_on_or_forward_plane(&frustum->right_face) &&
           is_on_or_forward_plane(&frustum->top_face) &&
           is_on_or_forward_plane(&frustum->bottom_face) &&
           is_on_or_forward_plane(&frustum->near_face) &&
           is_on_or_forward_plane(&frustum->far_face);
}

bool SphereCollider::is_on_or_forward_plane(Plane *plane)
{
    return plane->getSignedDistanceToPlane(get_parent()->get_world_position()) > -radius;
}