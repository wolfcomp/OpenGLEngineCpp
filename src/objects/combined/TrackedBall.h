#pragma once

#include "../primitives/IcoSphere.h"
#include "../curves/BSpline.h"
#include "../../shaders/ShaderStore.h"
#include "../../shaders/Material.h"

class TrackedBall : public IcoSphere
{
private:
    BSpline<glm::vec3> *curve;
    float time = 0;
    float trackTime = 0.5f;

public:
    TrackedBall() : IcoSphere()
    {
        curve = new BSpline<glm::vec3>({}, 2);
        curve->set_material(new ColorMaterial(glm::vec4(1, 0, 0, 1)));
        curve->set_shader(ShaderStore::get_shader("no_light"));
    }

    virtual void update(float delta_time) override
    {
        time += delta_time;
        if (time > trackTime)
        {
            time = 0;
            curve->add_point(get_component<TransformComponent>()->position);
        }
    }

    GameObject *get_curve()
    {
        return curve;
    }
};