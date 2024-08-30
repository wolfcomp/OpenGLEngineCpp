#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords)
    {
        this->position = position;
        this->normal = normal;
        this->texCoords = texCoords;
    }

    Vertex(glm::vec3 position, glm::vec3 normal)
    {
        this->position = position;
        this->normal = normal;
        this->texCoords = glm::vec2(0.0f);
    }

    Vertex(glm::vec3 position)
    {
        this->position = position;
        this->normal = glm::vec3(0.0f);
        this->texCoords = glm::vec2(0.0f);
    }

    Vertex()
    {
        this->position = glm::vec3(0.0f);
        this->normal = glm::vec3(0.0f);
        this->texCoords = glm::vec2(0.0f);
    }
};