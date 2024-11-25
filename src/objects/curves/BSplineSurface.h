#pragma once

#include "../base/GameObject.h"
#include "../../Window.h"
#include "BSpline.h"
#include <map>
#include <array>
#include <optional>
#include <format>
#include <glm/gtx/exterior_product.hpp>

class BSplineSurface : public GameObject
{
private:
    int degree_u;
    int degree_v;
    int num_points_u;
    int num_points_v;
    std::vector<glm::vec3> points;
    std::vector<float> knot_vector_u;
    std::vector<float> knot_vector_v;

    std::pair<glm::vec3, glm::vec3> b2(float tu, float tv, int iu, int iv)
    {
        glm::vec3 bv;
        glm::vec3 bu;

        auto w12 = (tu - knot_vector_u[iu - 1]) / (knot_vector_u[iu + 1] - knot_vector_u[iu - 1]);
        auto w22 = (tu - knot_vector_u[iu]) / (knot_vector_u[iu + 2] - knot_vector_u[iu]);
        auto w11 = (tu - knot_vector_u[iu]) / (knot_vector_u[iu + 1] - knot_vector_u[iu]);

        bu.x = (1 - w11) * (1 - w12);
        bu.z = w11 * w22;
        bu.y = 1 - bu.x - bu.z;

        w12 = (tv - knot_vector_v[iv - 1]) / (knot_vector_v[iv + 1] - knot_vector_v[iv - 1]);
        w22 = (tv - knot_vector_v[iv]) / (knot_vector_v[iv + 2] - knot_vector_v[iv]);
        w11 = (tv - knot_vector_v[iv]) / (knot_vector_v[iv + 1] - knot_vector_v[iv]);

        bv.x = (1 - w11) * (1 - w12);
        bv.z = w11 * w22;
        bv.y = 1 - bv.x - bv.z;

        return std::make_pair(bu, bv);
    }

    glm::vec3 evaluateBiquadratic(int iu, int iv, glm::vec3 bu, glm::vec3 bv)
    {

        glm::vec3 r(0);
        glm::mat3x3 m(0);
        glm::vec3 tmp_points[3][3];

        auto du = iu - degree_u;
        auto dv = iv - degree_v;

        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                auto u = du + i;
                auto v = dv + j;
                m[j][i] = bu[i] * bv[j];
                tmp_points[j][i] = points[u + v * num_points_u];
                r += bu[i] * bv[j] * points[u + v * num_points_u];
            }
        }

        return r;
    }

    int find_knot_interval(float t, int degree, int num_points, std::vector<float> knot_vector)
    {
        int n = num_points - 1;
        while (t < knot_vector[n] && n > degree)
            n--;
        return n;
    }

public:
    BSplineSurface(int degree_u, int degree_v, int num_points_u, int num_points_v, std::vector<float> knot_vector_u, std::vector<float> knot_vector_v, std::vector<glm::vec3> points, float spacing = 0.1f) : GameObject()
    {
        this->degree_u = degree_u;
        this->degree_v = degree_v;
        this->num_points_u = num_points_u;
        this->num_points_v = num_points_v;
        this->knot_vector_u = knot_vector_u;
        this->knot_vector_v = knot_vector_v;
        this->points = points;

        computeSurface(spacing);
    }

    void computeSurface(float spacing = 0.1f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        int num_u = 1 + (knot_vector_u[num_points_u] - knot_vector_u[degree_u]) / spacing;
        int num_v = 1 + (knot_vector_v[num_points_v] - knot_vector_v[degree_v]) / spacing;

        for (int i = 0; i < num_u; i++)
        {
            for (int j = 0; j < num_v; j++)
            {
                float u = i * spacing;
                float v = j * spacing;

                auto iu = find_knot_interval(u, degree_u, num_points_u, knot_vector_u);
                auto iv = find_knot_interval(v, degree_v, num_points_v, knot_vector_v);

                auto coeff_pair = b2(u, v, iu, iv);

                glm::vec3 p = evaluateBiquadratic(iu, iv, coeff_pair.first, coeff_pair.second);

                vertices.push_back(Vertex{p, glm::vec3(0, 1, 0), glm::vec2(0, 0)});
            }
            glfwSetWindowTitle(Window::glfWindow, std::format("Processing BSplineSurface vertexes: {:.2f}%", (float)(i * num_v) / (num_v * num_v) * 100).c_str());
        }

        for (int i = 0; i < num_u - 1; i++)
        {
            for (int j = 0; j < num_v - 1; j++)
            {
                auto p0 = i * num_v + j;
                auto p1 = i * num_v + j + 1;
                auto p2 = (i + 1) * num_v + j + 1;
                auto p3 = (i + 1) * num_v + j;
                auto n1 = glm::normalize(glm::cross(vertices[p1].position - vertices[p0].position, vertices[p2].position - vertices[p0].position));
                auto n2 = glm::normalize(glm::cross(vertices[p2].position - vertices[p0].position, vertices[p3].position - vertices[p0].position));
                indices.push_back(p0);
                indices.push_back(p1);
                indices.push_back(p2);
                vertices[p0].normal = n1;
                vertices[p1].normal = n1;
                vertices[p2].normal = n1;

                indices.push_back(p0);
                indices.push_back(p2);
                indices.push_back(p3);
                vertices[p0].normal = glm::normalize(n2 + vertices[p0].normal);
                vertices[p2].normal = glm::normalize(n2 + vertices[p2].normal);
                vertices[p3].normal = n2;
            }
            glfwSetWindowTitle(Window::glfWindow, std::format("Processing BSplineSurface indices: {:.2f}%", (float)(i * num_v) / (num_v * num_v) * 100).c_str());
        }

        update_vertices(vertices);
        update_indices(indices);
    }

private:
    // Spatial cell for quick lookups
    struct SpatialCell
    {
        std::vector<int> vertex_indices;
        glm::vec2 min_bounds;
        glm::vec2 max_bounds;
    };

    // Cache for recent results
    struct CacheEntry
    {
        glm::vec3 position;
        int vertex_index;
        float y;
        glm::vec3 normal;
        bool valid;
    };

    // Spatial acceleration grid
    mutable std::vector<SpatialCell> spatial_grid;
    mutable int spatial_grid_size = 0;
    mutable glm::vec2 cell_size;
    mutable glm::vec2 terrain_min;
    mutable glm::vec2 terrain_max;
    mutable bool spatial_grid_initialized = false;

    // LRU cache for multiple positions
    static constexpr size_t CACHE_SIZE = 16;
    mutable std::array<CacheEntry, CACHE_SIZE> position_cache;
    mutable size_t cache_index = 0;

    void init_spatial_grid() const
    {
        if (spatial_grid_initialized)
            return;

        auto vertices = get_vertices_ptr();
        auto grid_resolution = static_cast<int>(sqrt(vertices->size())) / 4; // Adjust divisor based on terrain size
        spatial_grid_size = grid_resolution * grid_resolution;

        // Find terrain bounds
        terrain_min = glm::vec2(FLT_MAX);
        terrain_max = glm::vec2(-FLT_MAX);

        for (const auto &vertex : *vertices)
        {
            terrain_min.x = std::min(terrain_min.x, vertex.position.x);
            terrain_min.y = std::min(terrain_min.y, vertex.position.z);
            terrain_max.x = std::max(terrain_max.x, vertex.position.x);
            terrain_max.y = std::max(terrain_max.y, vertex.position.z);
        }

        // Initialize spatial grid
        cell_size = (terrain_max - terrain_min) / static_cast<float>(grid_resolution);
        spatial_grid.resize(spatial_grid_size);

        // Initialize cell bounds
        for (int z = 0; z < grid_resolution; ++z)
        {
            for (int x = 0; x < grid_resolution; ++x)
            {
                int index = z * grid_resolution + x;
                spatial_grid[index].min_bounds = terrain_min + glm::vec2(x, z) * cell_size;
                spatial_grid[index].max_bounds = spatial_grid[index].min_bounds + cell_size;
            }
        }

        // Populate cells with vertex indices
        for (size_t i = 0; i < vertices->size(); ++i)
        {
            const auto &pos = vertices->at(i).position;
            glm::vec2 pos2d(pos.x, pos.z);

            // Get cell indices for vertex position
            int cell_x = static_cast<int>((pos.x - terrain_min.x) / cell_size.x);
            int cell_z = static_cast<int>((pos.z - terrain_min.y) / cell_size.y);

            // Clamp to valid range
            cell_x = std::clamp(cell_x, 0, grid_resolution - 1);
            cell_z = std::clamp(cell_z, 0, grid_resolution - 1);

            int cell_index = cell_z * grid_resolution + cell_x;
            spatial_grid[cell_index].vertex_indices.push_back(static_cast<int>(i));
        }

        spatial_grid_initialized = true;

        // Initialize cache
        for (auto &entry : position_cache)
        {
            entry.valid = false;
        }
    }

    std::vector<int> get_nearby_vertices(const glm::vec2 &pos) const
    {
        init_spatial_grid();

        // Get cell containing the position
        int cell_x = static_cast<int>((pos.x - terrain_min.x) / cell_size.x);
        int cell_z = static_cast<int>((pos.y - terrain_min.y) / cell_size.y);

        // Clamp to valid range
        int grid_resolution = static_cast<int>(sqrt(spatial_grid_size));
        cell_x = std::clamp(cell_x, 0, grid_resolution - 1);
        cell_z = std::clamp(cell_z, 0, grid_resolution - 1);

        std::vector<int> nearby_vertices;

        // Check neighboring cells
        for (int dz = -1; dz <= 1; ++dz)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                int check_x = cell_x + dx;
                int check_z = cell_z + dz;

                if (check_x < 0 || check_x >= grid_resolution ||
                    check_z < 0 || check_z >= grid_resolution)
                {
                    continue;
                }

                int cell_index = check_z * grid_resolution + check_x;
                nearby_vertices.insert(nearby_vertices.end(),
                                       spatial_grid[cell_index].vertex_indices.begin(),
                                       spatial_grid[cell_index].vertex_indices.end());
            }
        }

        return nearby_vertices;
    }

public:
    std::tuple<float, glm::vec3> get_y_at(const glm::vec3 &position) const
    {
        const glm::vec2 pos_2d(position.x, position.z);

        // Check cache first
        const float cache_threshold_sq = 0.01f; // Smaller threshold for more precise cache hits
        for (const auto &cache_entry : position_cache)
        {
            if (cache_entry.valid &&
                glm::distance2(position, cache_entry.position) < cache_threshold_sq)
            {
                return std::make_tuple(cache_entry.y, cache_entry.normal);
            }
        }

        // Get nearby vertices using spatial grid
        const auto nearby_vertices = get_nearby_vertices(pos_2d);

        // Find closest vertex among nearby vertices
        auto vertices = get_vertices_ptr();
        float min_dist = FLT_MAX;
        int closest_idx = -1;

        for (int idx : nearby_vertices)
        {
            const auto &vertex = vertices->at(idx).position;
            float dist = glm::distance2(glm::vec2(vertex.x, vertex.z), pos_2d);
            if (dist < min_dist)
            {
                min_dist = dist;
                closest_idx = idx;
            }
        }

        if (closest_idx == -1)
        {
            // Fallback if no nearby vertices found
            auto min_bounds = get_min_vertex().position;
            return std::make_tuple(min_bounds.y, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // Find quad containing the point
        if (auto result = find_containing_quad(closest_idx, pos_2d))
        {
            // Update cache
            position_cache[cache_index] = {
                position,
                closest_idx,
                std::get<0>(*result),
                std::get<1>(*result),
                true};
            cache_index = (cache_index + 1) % CACHE_SIZE;

            return *result;
        }

        // Fallback
        auto min_bounds = get_min_vertex().position;
        return std::make_tuple(min_bounds.y, glm::vec3(0.0f, 1.0f, 0.0f));
    }

private:
    std::optional<std::tuple<float, glm::vec3>> find_containing_quad(int vertex_idx,
                                                                     const glm::vec2 &pos_2d) const
    {
        auto vertices = get_vertices_ptr();
        const int grid_size = static_cast<int>(sqrt(vertices->size()));

        // Get vertex position in grid
        int vertex_z = vertex_idx / grid_size;
        int vertex_x = vertex_idx % grid_size;

        // Check all potential quads this vertex could be part of
        static const std::array<std::array<std::pair<int, int>, 4>, 4> quad_offsets = {{
            {{{0, 0}, {-1, 0}, {0, -1}, {-1, -1}}}, // Upper-left
            {{{0, 0}, {1, 0}, {0, -1}, {1, -1}}},   // Upper-right
            {{{0, 0}, {-1, 0}, {0, 1}, {-1, 1}}},   // Lower-left
            {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}}      // Lower-right
        }};

        for (const auto &quad : quad_offsets)
        {
            if (auto result = check_quad(vertex_x, vertex_z, grid_size, quad, pos_2d))
            {
                return result;
            }
        }

        return std::nullopt;
    }

    std::optional<std::tuple<float, glm::vec3>> check_quad(int base_x, int base_z, int grid_size,
                                                           const std::array<std::pair<int, int>, 4> &offsets,
                                                           const glm::vec2 &pos_2d) const
    {
        auto vertices = get_vertices_ptr();

        // Verify all vertices exist
        for (const auto &[dx, dz] : offsets)
        {
            int x = base_x + dx;
            int z = base_z + dz;
            if (x < 0 || x >= grid_size || z < 0 || z >= grid_size)
            {
                return std::nullopt;
            }
        }

        // Get vertex positions
        std::array<Vertex, 4> quad_vertices;
        for (int i = 0; i < 4; ++i)
        {
            int x = base_x + offsets[i].first;
            int z = base_z + offsets[i].second;
            quad_vertices[i] = vertices->at(z * grid_size + x);
        }

        // Quick AABB test
        glm::vec2 min_bounds(FLT_MAX), max_bounds(-FLT_MAX);
        for (const auto &vertex : quad_vertices)
        {
            min_bounds.x = std::min(min_bounds.x, vertex.position.x);
            min_bounds.y = std::min(min_bounds.y, vertex.position.z);
            max_bounds.x = std::max(max_bounds.x, vertex.position.x);
            max_bounds.y = std::max(max_bounds.y, vertex.position.z);
        }

        if (pos_2d.x < min_bounds.x || pos_2d.x > max_bounds.x ||
            pos_2d.y < min_bounds.y || pos_2d.y > max_bounds.y)
        {
            return std::nullopt;
        }

        // Convert to 2D for triangle tests
        const glm::vec2 points[4] = {
            glm::vec2(quad_vertices[0].position.x, quad_vertices[0].position.z),
            glm::vec2(quad_vertices[1].position.x, quad_vertices[1].position.z),
            glm::vec2(quad_vertices[2].position.x, quad_vertices[2].position.z),
            glm::vec2(quad_vertices[3].position.x, quad_vertices[3].position.z)};

        // Check both triangles
        bool in_lower = point_in_triangle(pos_2d, points[0], points[1], points[3]);
        bool in_upper = !in_lower && point_in_triangle(pos_2d, points[0], points[3], points[2]);

        if (!in_lower && !in_upper)
        {
            return std::nullopt;
        }

        // Calculate height and normal
        float y;
        glm::vec3 normal;

        if (in_lower)
        {
            auto barycentric = calculate_barycentric(pos_2d, points[0], points[1], points[3]);
            y = quad_vertices[0].position.y * barycentric.x +
                quad_vertices[1].position.y * barycentric.y +
                quad_vertices[3].position.y * barycentric.z;
            normal = quad_vertices[0].normal * barycentric.x +
                     quad_vertices[1].normal * barycentric.y +
                     quad_vertices[3].normal * barycentric.z;
        }
        else
        {
            auto barycentric = calculate_barycentric(pos_2d, points[0], points[3], points[2]);
            y = quad_vertices[0].position.y * barycentric.x +
                quad_vertices[1].position.y * barycentric.y +
                quad_vertices[3].position.y * barycentric.z;
            normal = quad_vertices[0].normal * barycentric.x +
                     quad_vertices[1].normal * barycentric.y +
                     quad_vertices[3].normal * barycentric.z;
        }

        return std::make_tuple(y, normal);
    }

    // Edge function for point-in-triangle test
    float edge_function(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &p) const
    {
        return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
    }

    bool point_in_triangle(const glm::vec2 &p, const glm::vec2 &a, const glm::vec2 &b,
                           const glm::vec2 &c) const
    {
        float edge1 = edge_function(a, b, p);
        float edge2 = edge_function(b, c, p);
        float edge3 = edge_function(c, a, p);

        // Check if point is on the same side of all edges
        return (edge1 >= 0 && edge2 >= 0 && edge3 >= 0) ||
               (edge1 <= 0 && edge2 <= 0 && edge3 <= 0);
    }

    // Barycentric and normal calculation functions remain the same
    glm::vec3 calculate_barycentric(const glm::vec2 &p, const glm::vec2 &a, const glm::vec2 &b,
                                    const glm::vec2 &c) const
    {
        glm::vec2 v0 = b - a;
        glm::vec2 v1 = c - a;
        glm::vec2 v2 = p - a;

        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);

        float denom = 1.0f / (d00 * d11 - d01 * d01);
        float v = (d11 * d20 - d01 * d21) * denom;
        float w = (d00 * d21 - d01 * d20) * denom;
        float u = 1.0f - v - w;

        return glm::vec3(u, v, w);
    }
};