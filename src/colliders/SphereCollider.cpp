#include "SphereCollider.h"
#include "../objects/base/GameObject.h"

glm::vec3 SphereCollider::get_center() { return get_parent()->get_component<TransformComponent>()->position; }
glm::vec3 SphereCollider::get_scale() { return get_parent()->get_component<TransformComponent>()->scale; }

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
    return plane->getSignedDistanceToPlane(get_parent()->get_component<TransformComponent>()->position) > -radius;
}

glm::vec3 SphereCollider::find_furthest_point(glm::vec3 direction)
{
    return get_center() + glm::normalize(direction) * radius;
}