#include "GameObjectBase.h"

unsigned VBO, VAO, EBO;

void GameObjectBase::draw() const
{
    pre_render();
    glBindVertexArray(VAO);
    render();
    post_render();
    glBindVertexArray(0);
}

void GameObjectBase::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // texture coord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

Vertex GameObjectBase::get_min_vertex() const
{
    Vertex min = vertices[0];
    for (auto &vertex : vertices)
    {
        if (vertex.position.x < min.position.x)
        {
            min.position.x = vertex.position.x;
        }
        if (vertex.position.y < min.position.y)
        {
            min.position.y = vertex.position.y;
        }
        if (vertex.position.z < min.position.z)
        {
            min.position.z = vertex.position.z;
        }
    }
    return min;
}

Vertex GameObjectBase::get_max_vertex() const
{
    Vertex max = vertices[0];
    for (auto &vertex : vertices)
    {
        if (vertex.position.x > max.position.x)
        {
            max.position.x = vertex.position.x;
        }
        if (vertex.position.y > max.position.y)
        {
            max.position.y = vertex.position.y;
        }
        if (vertex.position.z > max.position.z)
        {
            max.position.z = vertex.position.z;
        }
    }
    return max;
}