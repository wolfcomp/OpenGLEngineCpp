#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include <array>
#include <map>
#include <initializer_list>
#include "Collider.h"

struct Simplex
{
private:
    std::array<glm::vec3, 4> points;
    int m_size;

public:
    Simplex() : m_size(0) {}

    Simplex &operator=(std::initializer_list<glm::vec3> list)
    {
        m_size = 0;
        for (auto p : list)
            points[m_size++] = p;

        return *this;
    }

    void push_front(glm::vec3 point)
    {
        points = {point, points[0], points[1], points[2]};
        m_size = std::min(m_size + 1, 4);
    }

    glm::vec3 &operator[](int i) { return points[i]; }
    size_t size() const { return m_size; }

    auto begin() const { return points.begin(); }
    auto end() const { return points.end() - (4 - m_size); }
};

struct GJK
{
    static bool line(Simplex &simplex, glm::vec3 &direction);
    static bool triangle(Simplex &simplex, glm::vec3 &direction);
    static bool tetrahedron(Simplex &simplex, glm::vec3 &direction);
    static bool next_simplex(Simplex &simplex, glm::vec3 &direction);
    static bool same_direction(glm::vec3 a, glm::vec3 b);
    static bool gjk(const ColliderBase &a, const ColliderBase &b);
    static bool gjk(const ColliderBase &a, const std::vector<glm::vec3> &vertices);
};