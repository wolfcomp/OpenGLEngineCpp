#pragma once
#include <glm/vec3.hpp>
#include "HSL.h"
#include "shaders/Shader.h"
#include "shaders/Shadow.h"
#include <vector>

struct Light
{
    hsl ambient;
    hsl diffuse;
    hsl specular;

    virtual void set_shader(const Shader *shader)
    {
        shader->set_vec3(get_name() + ".ambient", ambient.get_rgb_vec3());
        shader->set_vec3(get_name() + ".diffuse", diffuse.get_rgb_vec3());
        shader->set_vec3(get_name() + ".specular", specular.get_rgb_vec3());
    }

    virtual std::string get_name()
    {
        return "light";
    }
};

struct DirectionalLight : public Light
{
    glm::vec3 direction;

    void set_shader(const Shader *shader) override
    {
        Light::set_shader(shader);
        shader->set_vec3(get_name() + ".direction", direction);
    }

    std::string get_name() override
    {
        return "dirLight";
    }
};

struct PointLight : public Light
{
    int index;
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    void set_shader(const Shader *shader) override
    {
        Light::set_shader(shader);
        shader->set_vec3(get_name() + ".position", position);
        shader->set_float(get_name() + ".constant", constant);
        shader->set_float(get_name() + ".linear", linear);
        shader->set_float(get_name() + ".quadratic", quadratic);
    }

    std::string get_name() override
    {
        return "pointLights[" + std::to_string(index) + "]";
    }
};

struct SpotLight : public PointLight
{
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    ShadowProcessor *shadowProcessor;

    SpotLight()
    {
        shadowProcessor = new ShadowProcessor();
        shadowProcessor->init(16, get_name() + ".shadowMap");
    }

    void set_shader(const Shader *shader) override
    {
        PointLight::set_shader(shader);
        shader->set_vec3(get_name() + ".direction", direction);
        shader->set_float(get_name() + ".cutOff", cutOff);
        shader->set_float(get_name() + ".outerCutOff", outerCutOff);
        shadowProcessor->set_shader(shader);
    }

    std::string get_name() override
    {
        return "spotLight";
    }
};

class LightManager
{
    std::vector<Light *> lights;

public:
    void add_light(Light *light)
    {
        if (dynamic_cast<DirectionalLight *>(light))
            for (auto l : lights)
                if (dynamic_cast<DirectionalLight *>(l))
                    throw std::runtime_error("Only one directional light is allowed");
        lights.push_back(light);
    }

    void set_shader(const Shader *shader)
    {
        for (auto light : lights)
        {
            light->set_shader(shader);
        }
    }

    std::vector<Light *> get_lights()
    {
        lights = this->lights;
    }

    void cleanup()
    {
        for (auto light : lights)
        {
            delete light;
        }
        lights.clear();
    }
};