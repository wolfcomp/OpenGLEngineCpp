#include "particle.h"
#include <imgui/imgui.h>
#include <algorithm>

glm::vec3 currentMinSpawnPos = {-5, 4, -5};
glm::vec3 currentMaxSpawnPos = {5, 4, 5};
glm::vec3 currentSpawnScale = {.5, .5, .5};
glm::vec4 currentSpawnColor = {1, 1, 1, 1};
bool spawnMultiple = false;

void particle_window(World *world, std::function<void(glm::vec3, glm::vec3, glm::vec4)> spawn_function)
{
    ImGui::Begin("Particles");
    ImGui::SetWindowSize(ImVec2(311, 235), ImGuiCond_FirstUseEver);
    ImGui::Text("Bounds");
    auto bounds = world->get_bounds();
    auto max = bounds.center + bounds.extent;
    auto min = bounds.center - bounds.extent;
    ImGui::Text("Min: (%.2f, %.2f, %.2f)", min.x, min.y, min.z);
    ImGui::Text("Max: (%.2f, %.2f, %.2f)", max.x, max.y, max.z);
    ImGui::Separator();
    ImGui::Checkbox("Spawn multiple", &spawnMultiple);
    auto size = currentSpawnScale;
    if (ImGui::SliderFloat3("Size", &size.x, 0.1f, 10.0f))
        currentSpawnScale = glm::clamp(size, glm::vec3(0.1f), glm::vec3(10.0f));
    auto color = currentSpawnColor;
    if (ImGui::ColorEdit4("Color", &color.x))
        currentSpawnColor = color;
    if (spawnMultiple)
    {
        auto minSpawn = currentMinSpawnPos;
        auto maxSpawn = currentMaxSpawnPos;
        ImGui::Columns(2);
        if (ImGui::SliderFloat("Min Position X", &minSpawn.x, min.x, max.x))
            currentMinSpawnPos.x = std::clamp(minSpawn.x, min.x, max.x);
        ImGui::NextColumn();
        if (ImGui::SliderFloat("Max Position X", &maxSpawn.x, min.x, max.x))
            currentMaxSpawnPos.x = std::clamp(maxSpawn.x, min.x, max.x);
        ImGui::NextColumn();
        if (ImGui::SliderFloat("Min Position Y", &minSpawn.y, min.y, max.y))
            currentMinSpawnPos.y = std::clamp(minSpawn.y, min.y, max.y);
        ImGui::NextColumn();
        if (ImGui::SliderFloat("Max Position Y", &maxSpawn.y, min.y, max.y))
            currentMaxSpawnPos.y = std::clamp(maxSpawn.y, min.y, max.y);
        ImGui::NextColumn();
        if (ImGui::SliderFloat("Min Position Z", &minSpawn.z, min.z, max.z))
            currentMinSpawnPos.z = std::clamp(minSpawn.z, min.z, max.z);
        ImGui::NextColumn();
        if (ImGui::SliderFloat("Max Position Z", &maxSpawn.z, min.z, max.z))
            currentMaxSpawnPos.z = std::clamp(maxSpawn.z, min.z, max.z);
        ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Button("Spawn multiple");
        if (ImGui::IsItemHovered())
            if (ImGui::BeginTooltip())
            {
                ImGui::Text("This will spawn particles in a grid pattern based on the spawn scale and min/max positions");
                ImGui::EndTooltip();
            }
        if (ImGui::IsItemClicked())
        {
            for (float x = currentMinSpawnPos.x; x <= currentMaxSpawnPos.x; x += currentSpawnScale.x * 2)
            {
                for (float y = currentMinSpawnPos.y; y <= currentMaxSpawnPos.y; y += currentSpawnScale.y * 2)
                {
                    for (float z = currentMinSpawnPos.z; z <= currentMaxSpawnPos.z; z += currentSpawnScale.z * 2)
                    {
                        spawn_function({x, y, z}, currentSpawnScale, currentSpawnColor);
                    }
                }
            }
        }
    }
    else
    {
        auto pos = currentMinSpawnPos;
        if (ImGui::SliderFloat("Position X", &pos.x, min.x, max.x))
            currentMinSpawnPos.x = std::clamp(pos.x, min.x, max.x);
        if (ImGui::SliderFloat("Position Y", &pos.y, min.y, max.y))
            currentMinSpawnPos.y = std::clamp(pos.y, min.y, max.y);
        if (ImGui::SliderFloat("Position Z", &pos.z, min.z, max.z))
            currentMinSpawnPos.z = std::clamp(pos.z, min.z, max.z);
        if (ImGui::Button("Spawn on position"))
        {
            spawn_function(currentMinSpawnPos, currentSpawnScale, currentSpawnColor);
        }
    }
    ImGui::End();
}