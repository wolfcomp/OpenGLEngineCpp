#pragma once

#include "../base/SceneUpdatableObject.h"
#include "../../colliders/SphereCollider.h"

class IcoSphere : public SceneUpdatableObject
{
private:
    const float H_ANGLE = glm::pi<float>() / 180 * 72;
    const float V_ANGLE = atanf(1.0f / 2);

public:
    IcoSphere(glm::vec3 position) : SceneUpdatableObject({}, {})
    {
        set_position(position);
        set_collider(new SphereCollider(this));
    };
    IcoSphere() : SceneUpdatableObject({}, {}) {};
    ~IcoSphere() {};
    void create(int subdivisions)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices = {
            // top cone
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 1,

            // center cylinder
            6, 2, 1,
            6, 7, 2,
            7, 3, 2,
            7, 8, 3,
            8, 4, 3,
            8, 9, 4,
            9, 5, 4,
            9, 10, 5,
            10, 1, 5,
            6, 1, 10,

            // bottom cone
            11, 6, 10,
            11, 7, 6,
            11, 8, 7,
            11, 9, 8,
            11, 10, 9};
        vertices.resize(12);
        float h_angle1 = -glm::pi<float>() / 2;
        float h_angle2 = h_angle1 - H_ANGLE / 2;
        vertices[0] = Vertex(glm::vec3(0, 0, 1));
        vertices[11] = -vertices[0];
        const auto z = sinf(V_ANGLE);
        const auto xy = cosf(V_ANGLE);
        for (int i = 1; i < 6; i++)
        {
            const auto i2 = i + 5;
            vertices[i] = Vertex(glm::vec3(xy * cosf(h_angle1), xy * sinf(h_angle1), z));
            vertices[i2] = Vertex(glm::vec3(xy * cosf(h_angle2), xy * sinf(h_angle2), -z));
            h_angle1 += H_ANGLE;
            h_angle2 += H_ANGLE;
        }

        if (subdivisions > 0)
        {
            std::vector<Vertex> old_vertices;
            std::vector<unsigned> old_indices;
            unsigned index = 0;
            Vertex v1, v2, v3, nv1, nv2, nv3;
            for (unsigned i = 0; i < subdivisions; i++)
            {
                old_vertices = vertices;
                old_indices = indices;
                vertices.clear();
                indices.clear();
                index = 0;

                for (unsigned j = 0; j < old_indices.size(); j += 3)
                {
                    v1 = old_vertices[old_indices[j]];
                    v2 = old_vertices[old_indices[j + 1]];
                    v3 = old_vertices[old_indices[j + 2]];

                    nv1 = Vertex::compute_half(v1, v2);
                    nv2 = Vertex::compute_half(v2, v3);
                    nv3 = Vertex::compute_half(v1, v3);

                    vertices.push_back(v1);
                    vertices.push_back(nv1);
                    vertices.push_back(nv3);

                    vertices.push_back(nv1);
                    vertices.push_back(v2);
                    vertices.push_back(nv2);

                    vertices.push_back(nv1);
                    vertices.push_back(nv2);
                    vertices.push_back(nv3);

                    vertices.push_back(nv3);
                    vertices.push_back(nv2);
                    vertices.push_back(v3);

                    for (char k = 0; k < 12; k++)
                    {
                        indices.push_back(index + k);
                    }
                    index += 12;
                }
            }
        }

        update_vertices(vertices);
        update_indices(indices);
    }

    void setup() override
    {
        create(3);
    }

    void update(float delta_time) override
    {
        auto pos = get_position();
        auto toCenterVec = -pos;
        auto speed = get_velocity();
        speed += toCenterVec * delta_time;
        set_velocity(speed);
        pos += speed * delta_time;
        set_position(pos);
        SceneUpdatableObject::update(delta_time);
    }
};