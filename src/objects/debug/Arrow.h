#pragma once

#include "../base/Renderable.h"
#include <glm/gtx/vector_angle.hpp>

class Arrow : public Renderable
{
    glm::mat4 model;

public:
    Arrow(glm::vec3 direction, glm::vec3 position) : Renderable({
                                                                    Vertex({0.05f, 0, 0}),
                                                                    Vertex({0, 0, 0.05f}),
                                                                    Vertex({0, 0, -0.05f}),
                                                                    Vertex({-0.05f, 0, 0}),
                                                                    Vertex({0.05f, 1, 0}),
                                                                    Vertex({0, 1, 0.05f}),
                                                                    Vertex({0, 1, -0.05f}),
                                                                    Vertex({-0.05f, 1, 0}),
                                                                    Vertex({0.2f, 1, 0}),
                                                                    Vertex({0, 1, 0.2f}),
                                                                    Vertex({0, 1, -0.2f}),
                                                                    Vertex({-0.2f, 1, 0}),
                                                                    Vertex({0, 1.4f, 0}),
                                                                },
                                                                {// base
                                                                 0, 1, 2, 1, 3, 2,

                                                                 // shaft
                                                                 0, 4, 1, 1, 4, 5,

                                                                 1, 7, 3, 1, 7, 5,

                                                                 2, 6, 3, 3, 6, 7,

                                                                 2, 6, 0, 0, 6, 4,

                                                                 // arrow base
                                                                 8, 9, 10, 9, 11, 10,

                                                                 // arrow head
                                                                 8, 9, 12, 9, 10, 12,

                                                                 10, 11, 12, 11, 8, 12})
    {
        double angle = std::atan2(direction.y, direction.x);
        glm::mat4 glmrotxy = glm::rotate(angle, glm::dvec3(0, 0, -1));
        double angleZ = -std::asin(direction.z);
        glm::mat4 glmrotz = glm::rotate(angleZ, glm::dvec3(-1, 0, 0));
        this->model = glm::translate(glm::mat4(1.0f), position);
        this->model = this->model * (glmrotxy * glmrotz);
    }

    void pre_render() const override
    {
        Renderable::pre_render();
        glLineWidth(5);
        get_shader()->set_mat4("model", model);
    }

    void post_render() const override
    {
        Renderable::post_render();
        glLineWidth(1);
    }
};