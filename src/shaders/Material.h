#pragma once
#include <string>
#include <glm/vec4.hpp>

class Shader;

struct Material
{
    float shininess = 32.0f;

    void set_shininess(float shininess);
    virtual void use(Shader *shader);
};

struct TextureMaterial : public Material
{
    unsigned int diffuseTexture = 0;
    unsigned int specularTexture = 0;
    unsigned int normalTexture = 0;

    void load_texture(const std::string block, const std::string path);
    void use(Shader *shader) override;
};

struct ColorMaterial : public Material
{
    glm::vec4 color;
    void use(Shader *shader) override;
    ColorMaterial() : color(glm::vec4(1.0f)) {};
    ColorMaterial(glm::vec4 color)
    {
        this->color = color;
    }
};