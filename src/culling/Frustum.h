#pragma once
#include "Plane.h"
#include "../input/Camera.h"
#include "../input/InputProcessing.h"

struct Frustum
{
    Plane top_face;
    Plane bottom_face;

    Plane right_face;
    Plane left_face;

    Plane far_face;
    Plane near_face;

    static Frustum create_from_camera(Camera *camera, float aspect, float fov_y, float near_z, float far_z)
    {
        Frustum frustum;
        const float halfVSide = far_z * tan(fov_y / 2);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = far_z * camera->get_front();

        frustum.near_face = {camera->get_pos() + near_z * camera->get_front(), camera->get_front()};
        frustum.far_face = {camera->get_pos() + frontMultFar, -camera->get_front()};
        frustum.right_face = {camera->get_pos(), glm::cross(frontMultFar - camera->get_right() * halfHSide, camera->get_up())};
        frustum.left_face = {camera->get_pos(), glm::cross(camera->get_up(), frontMultFar + camera->get_right() * halfHSide)};
        frustum.top_face = {camera->get_pos(), glm::cross(camera->get_right(), frontMultFar - camera->get_up() * halfVSide)};
        frustum.bottom_face = {camera->get_pos(), glm::cross(frontMultFar + camera->get_up() * halfVSide, camera->get_right())};
        return frustum;
    }

    static Frustum create_from_camera_and_input(Camera *camera, InputProcessing *input)
    {
        return create_from_camera(camera, input->get_aspect_ratio(), input->get_fov_y(), input->get_near_z(), input->get_far_z());
    }
};