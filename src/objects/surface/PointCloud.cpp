#include "PointCloud.h"
#include "../curves/BSplineSurface.h"
#include "../curves/BSpline.h"
#include <algorithm>

GameObject *PointCloud::convert_to_surface()
{
    std::vector<glm::vec3> points = {};
    std::map<float, std::vector<glm::vec3>> point_map = {};
    std::vector<Vertex> vertices = get_vertices();
    for (auto vertex : vertices)
    {
        if (point_map.find(vertex.position.z) == point_map.end())
        {
            point_map[vertex.position.z] = {};
        }
        point_map[vertex.position.z].push_back(vertex.position);
    }
    for (auto &pair : point_map)
    {
        std::sort(pair.second.begin(), pair.second.end(), [](glm::vec3 a, glm::vec3 b)
                  { return a.x < b.x; });
    }
    auto size = point_map.size();
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
        for (int j = 0; j < size; j++)
        {
            auto curIndex = static_cast<int>(point_map_row.size() * (static_cast<float>(j) / size));
            points.push_back(point_map_row[curIndex]);
        }
    }
    std::vector<float> knot_vector = BSpline<glm::vec3>::get_knot_vector(size - 1);
    auto surface = new BSplineSurface(2, 2, size - 1, size - 1, knot_vector, knot_vector, points, 0.5);
    return surface;
}