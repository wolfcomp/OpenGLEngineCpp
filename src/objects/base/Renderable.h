#pragma once

#include <vector>
#include "Vertex.h"
#include "../../Shader.h"
#include <glad/glad.h>

unsigned VBO, VAO, EBO;

class Renderable
{
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    Shader *shader;
    GLenum mode = GL_TRIANGLES;

public:
    Renderable(std::vector<Vertex> vertices, std::vector<unsigned> indices) : vertices(vertices), indices(indices) {};
    Renderable() {};
    ~Renderable()
    {
        vertices.clear();
        indices.clear();
    };

    void update_vertices(std::vector<Vertex> vertices) { this->vertices = vertices; }
    void update_indices(std::vector<unsigned> indices) { this->indices = indices; }
    void set_shader(Shader *shader) { this->shader = shader; }
    void set_mode(GLenum mode) { this->mode = mode; }
    Shader *get_shader() const { return shader; }
    virtual void pre_render() const
    {
        shader->use();
        glBindVertexArray(VAO);
    }
    virtual void render() const
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
        glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    }
    virtual void post_render() const
    {
        glBindVertexArray(0);
    }
    static void setup()
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
};
