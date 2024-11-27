#pragma once

#include <functional>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../World.h"

#ifndef PARTICLE_WINDOW
#define PARTICLE_WINDOW
void particle_window(World *world, std::function<void(glm::vec3, glm::vec3, glm::vec4)> spawn_function);
#endif // !PARTICLE_WINDOW