#pragma once
#include "../base/GameObject.h"
#include "../../fileformats/las.h"
#include <glm/gtx/vec_swizzle.hpp>

class PointCloud : public GameObject
{
private:
    std::vector<char> data; // Full data of the point cloud file
    LASFile lasFile;        // Parsed data of the point cloud file

public:
    PointCloud(std::string file) : GameObject({}, {})
    {
        load_las_file(file.c_str(), data);
        parse_las_file(data.data(), data.size(), &lasFile);
        glm::vec3 scale = {lasFile.header.x_scale_factor, lasFile.header.y_scale_factor, lasFile.header.z_scale_factor};
        std::vector<Vertex> vertices;
        glm::vec3 min = {lasFile.header.min_x, lasFile.header.min_y, lasFile.header.min_z};
        glm::vec3 max = {lasFile.header.max_x, lasFile.header.max_y, lasFile.header.max_z};
        glm::vec3 translate = (max + min) / 2.0f;
        for (int i = 0; i < lasFile.header.number_of_point_records; ++i)
        {
            switch (lasFile.header.point_data_format)
            {
            case 0:
                Point0 point;
                parse_point0(lasFile.point_data + i * lasFile.header.point_data_record_length, &point);
                vertices.push_back({xzy(glm::vec3(point.x, point.y, point.z) * scale - translate), {0, 0, 0}, {0, 0}});
                break;
            case 1:
                Point1 point1;
                parse_point1(lasFile.point_data + i * lasFile.header.point_data_record_length, &point1);
                vertices.push_back({xzy(glm::vec3(point1.x, point1.y, point1.z) * scale - translate), {0, 0, 0}, {0, 0}});
                break;
            case 2:
                Point2 point2;
                parse_point2(lasFile.point_data + i * lasFile.header.point_data_record_length, &point2);
                vertices.push_back({xzy(glm::vec3(point2.x, point2.y, point2.z) * scale - translate), {point2.red, point2.green, point2.blue}, {0, 0}});
                break;
            case 3:
                Point3 point3;
                parse_point3(lasFile.point_data + i * lasFile.header.point_data_record_length, &point3);
                vertices.push_back({xzy(glm::vec3(point3.x, point3.y, point3.z) * scale - translate), {point3.red, point3.green, point3.blue}, {0, 0}});
                break;

            default:
                break;
            }
        }
        update_vertices(vertices);
        std::vector<unsigned> indices(vertices.size());
        for (size_t i = 0; i < indices.size(); i++)
        {
            indices[i] = i;
        }
        update_indices(indices);
        set_mode(GL_POINTS);
    }
    ~PointCloud() {}
};