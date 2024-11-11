#pragma once

#include "../base/GameObjectBase.h"

class Line : public GameObjectBase
{
private:
public:
    Line() : GameObjectBase({Vertex(), Vertex()}, {0, 1}, nullptr)
    {
        set_mode(GL_LINES);
    };
    ~Line() {};
    void set_positions(glm::vec3 start, glm::vec3 end)
    {
        std::vector<Vertex> vertices = {
            Vertex(start),
            Vertex(end),
        };
        update_vertices(vertices);
    }

    void pre_render() const override
    {
        GameObjectBase::pre_render();
        get_shader()->set_mat4("model", glm::mat4x4(1.0f));
        glLineWidth(5);
    }

    void post_render() const override
    {
        glLineWidth(1);
        GameObjectBase::post_render();
    }
};
