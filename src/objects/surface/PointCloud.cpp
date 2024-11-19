#include "PointCloud.h"
#include "../curves/BSplineSurface.h"
#include "../curves/BSpline.h"
#include "../../Window.h"
#include <algorithm>
#include <ranges>
#include <numeric>
#include <format>

GameObject *PointCloud::convert_to_surface()
{
    std::vector<glm::vec3> points = {};
    std::map<float, std::vector<glm::vec3>> point_map = {};
    std::vector<Vertex> vertices = get_vertices();
    float min_x = vertices[0].position.x;
    float max_x = vertices[0].position.x;
    float min_z = vertices[0].position.z;
    float max_z = vertices[0].position.z;
    for (auto vertex : vertices)
    {
        if (point_map.find(vertex.position.z) == point_map.end())
        {
            point_map[vertex.position.z] = {};
        }
        if (vertex.position.x < min_x)
        {
            min_x = vertex.position.x;
        }
        if (vertex.position.x > max_x)
        {
            max_x = vertex.position.x;
        }
        if (vertex.position.z < min_z)
        {
            min_z = vertex.position.z;
        }
        if (vertex.position.z > max_z)
        {
            max_z = vertex.position.z;
        }
        point_map[vertex.position.z].push_back(vertex.position);
    }
    for (auto &pair : point_map)
    {
        std::sort(pair.second.begin(), pair.second.end(), [](glm::vec3 a, glm::vec3 b)
                  { return a.x < b.x; });
    }
    // X Size step is the min and maximum point value in all rows
    // Z Size step is the index of the itteration in the map
    auto size = point_map.size();
    points.reserve(size * size);
    float x_step = (max_x - min_x) / size;
    float last_y = 0;
    for (int i = 0; i < size; i++)
    {
        auto map_index = 0;
        auto actual_map_index = 0.0f;
        for (auto it = point_map.begin(); it != point_map.end(); ++it)
        {
            if (map_index == i)
            {
                actual_map_index = it->first;
                break;
            }
            map_index++;
        }
        auto point_map_row = point_map[actual_map_index];
        float prev_x = min_x;
        for (int j = 0; j < size; j++)
        {
            auto f = point_map_row | std::ranges::views::filter([prev_x, x_step](glm::vec3 point)
                                                                { return point.x >= prev_x && point.x < prev_x + x_step; });
            auto y_sum = std::accumulate(f.begin(), f.end(), 0.0f, [](float sum, glm::vec3 point)
                                         { return sum + point.y; });
            auto f_count = std::distance(f.begin(), f.end());
            if (f_count == 0)
            {
                points.push_back(glm::vec3(prev_x + x_step / 2, last_y, actual_map_index));
                prev_x += x_step;
                continue;
            }
            auto y_avg = y_sum / f_count;
            last_y = y_avg;
            points.push_back(glm::vec3(prev_x + x_step / 2, y_avg, actual_map_index));
            prev_x += x_step;
        }
        glfwSetWindowTitle(Window::glfWindow, std::format("Processing point cloud to surface: {:.2f}%", (float)(i * size) / (size * size) * 100).c_str());
    }
    glfwSetWindowTitle(Window::glfWindow, "Generating knot vector for surface");
    std::vector<float> knot_vector = BSpline<glm::vec3>::get_knot_vector(size - 1);
    glfwSetWindowTitle(Window::glfWindow, "Creating surface from point cloud");
    auto surface = new BSplineSurface(2, 2, size, size, knot_vector, knot_vector, points, 0.5);
    return surface;
}