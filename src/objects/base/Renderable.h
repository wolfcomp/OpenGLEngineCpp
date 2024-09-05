#pragma once

#include <vector>
#include "Vertex.h"
#include "../../Shader.h"
#include "../../Material.h"
#include <glad/glad.h>

class Renderable
{
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    // Shaders can be shared between objects
    Shader *shader = nullptr;
    // Materials are per object currently, TODO make a shared library for materials to be shared between objects
    Material *material = nullptr;
    GLenum mode = GL_TRIANGLES;

public:
    Renderable(std::vector<Vertex> vertices, std::vector<unsigned> indices) : vertices(vertices), indices(indices) {};
    Renderable() {};
    ~Renderable()
    {
        vertices.clear();
        indices.clear();
        delete material;
    };

    void update_vertices(std::vector<Vertex> vertices) { this->vertices = vertices; }
    void update_indices(std::vector<unsigned> indices) { this->indices = indices; }
    void set_shader(Shader *shader) { this->shader = shader; }
    void set_mode(GLenum mode) { this->mode = mode; }
    void set_material(Material *material) { this->material = material; }
    Material *get_material() const { return material; }
    Shader *get_shader() const { return shader; }
    virtual void pre_render() const
    {
        shader->use();
        material->use(shader);
    }
    virtual void render() const
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
        glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    }
    virtual void post_render() const {}
    void draw() const;
    static void setup();
};
