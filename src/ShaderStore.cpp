#include "ShaderStore.h"

#include <map>
#include <vector>

std::map<unsigned, Shader *> shaders;
std::vector<void (*)(const Shader *)> shaderCallbacks;
std::map<unsigned, std::string> shaderNames;

Shader *ShaderStore::add_shader(const std::string &name, const char *vertex_path, const char *fragment_path)
{
    const auto shader = new Shader(vertex_path, fragment_path);
    for (const auto &callback : shaderCallbacks)
    {
        shader->add_params_callback(callback);
    }
    const auto id = shaders.size();
    shaders[id] = shader;
    shaderNames[id] = name;
    return shader;
}

Shader *ShaderStore::get_shader(const unsigned id)
{
    return shaders[id];
}

Shader *ShaderStore::get_shader(const std::string name)
{
    if (shaderNames.empty())
    {
        return nullptr;
    }
    for (const auto &shader : shaderNames)
    {
        if (shader.second == name)
        {
            return shaders[shader.first];
        }
    }
    return nullptr;
}

void ShaderStore::remove_all_shaders()
{
    for (const auto &shader : shaders)
    {
        delete shader.second;
    }
    shaders.clear();
}

void ShaderStore::remove_shader(const unsigned id)
{
    delete shaders[id];
    shaders.erase(id);
}

void ShaderStore::add_params_callback(void func(const Shader *))
{
    shaderCallbacks.push_back(func);
    for (const auto &shader : shaders)
    {
        shader.second->add_params_callback(func);
    }
}
