#include <filesystem>
#include "Material.h"
#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int create_texture(const char *path, bool gamma_correct, bool silent_error)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        GLenum internal_format;
        if (nrChannels == 1)
            internal_format = format = GL_RED;
        else if (nrChannels == 3)
        {
            format = GL_RGB;
            internal_format = gamma_correct ? GL_SRGB : GL_RGB;
        }
        else if (nrChannels == 4)
        {
            format = GL_RGBA;
            internal_format = gamma_correct ? GL_SRGB_ALPHA : GL_RGBA;
        }
        else
        {
            internal_format = format = GL_RGB;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        if (!silent_error)
        {
            std::cerr << "ERROR::MATERIAL::CREATE_TEXTURE" << std::endl;
            std::cerr << "Failed to load texture" << std::endl;
            std::cerr << path << std::endl;
        }
        texture = 0;
    }
    stbi_image_free(data);
    return texture;
}

std::string get_path(std::filesystem::path p, std::string ext)
{
    return (p.parent_path().generic_string() + "/" + p.stem().generic_string() + ext + p.extension().generic_string());
}

void TextureMaterial::load_texture(const std::string block, const std::string path)
{
    // load and create textures
    std::filesystem::path p = "textures";
    p /= block;
    p /= path;
    diffuseTexture = create_texture(get_path(p, "_d").c_str(), true, false);
    specularTexture = create_texture(get_path(p, "_s").c_str(), false, true);
    normalTexture = create_texture(get_path(p, "_n").c_str(), false, true);
}

void Material::use(Shader *shader)
{
    shader->set_float("material.shininess", shininess);
}

void TextureMaterial::use(Shader *shader)
{
    Material::use(shader);
    shader->set_int("material.diffuse", 0);
    shader->set_int("material.specular", 1);
    shader->set_int("material.normal", 2);
    shader->set_int("material.hasMap", 1 | (specularTexture != 0) << 1 | (normalTexture != 0) << 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
}

void ColorMaterial::use(Shader *shader)
{
    Material::use(shader);
    shader->set_vec4("material.color", color);
    shader->set_int("material.hasMap", 0);
}