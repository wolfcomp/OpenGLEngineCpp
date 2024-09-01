#pragma once

#include <vector>
#include "../colliders/AABB.h"

template <typename T>
class QuadTree
{
private:
    QuadTree *northWest;
    QuadTree *northEast;
    QuadTree *southWest;
    QuadTree *southEast;

    AABB boundary;

    const unsigned capacity = 4;

    std::vector<T> points;

    void subdivide()
    {
        glm::vec3 center = boundary.center;
        glm::vec3 extent = boundary.extent;
        glm::vec3 extentDivide = extent / 2.0f;
        extentDivide.y = extent.y;

        glm::vec3 nw_center = center + glm::vec3(-extent.x / 2, 0, extent.z / 2);
        glm::vec3 ne_center = center + glm::vec3(extent.x / 2, 0, extent.z / 2);
        glm::vec3 sw_center = center + glm::vec3(-extent.x / 2, 0, -extent.z / 2);
        glm::vec3 se_center = center + glm::vec3(extent.x / 2, 0, -extent.z / 2);

        northWest = new QuadTree<T>();
        northWest->set_bounds(nw_center, extentDivide);
        northEast = new QuadTree<T>();
        northEast->set_bounds(ne_center, extentDivide);
        southWest = new QuadTree<T>();
        southWest->set_bounds(sw_center, extentDivide);
        southEast = new QuadTree<T>();
        southEast->set_bounds(se_center, extentDivide);

        for (const T &point : points)
        {
            if (northWest->insert(point))
                continue;
            if (northEast->insert(point))
                continue;
            if (southWest->insert(point))
                continue;
            if (southEast->insert(point))
                continue;
        }

        points.clear();
    };

public:
    QuadTree() : northWest(nullptr), northEast(nullptr), southWest(nullptr), southEast(nullptr)
    {
        points.reserve(capacity);
    };
    ~QuadTree()
    {
        delete northWest;
        delete northEast;
        delete southWest;
        delete southEast;
    };

    bool is_leaf() const { return northWest == nullptr; }
    bool insert(const T &point)
    {
        if (!boundary.contains(point))
            return false;

        if (points.size() < capacity && is_leaf())
        {
            points.push_back(point);
            return true;
        }

        if (is_leaf())
            subdivide();

        if (northWest->insert(point))
            return true;
        if (northEast->insert(point))
            return true;
        if (southWest->insert(point))
            return true;
        if (southEast->insert(point))
            return true;
        return false;
    };

    void query_range(const AABB &range, std::vector<T> &found)
    {
        if (!boundary.intersects(range))
            return;

        for (const T &point : points)
        {
            if (range.contains(point))
                found.push_back(point);
        }

        if (northWest == nullptr)
            return;

        northWest->query_range(range, found);
        northEast->query_range(range, found);
        southWest->query_range(range, found);
        southEast->query_range(range, found);
    };
    void draw_debug()
    {
        boundary.draw_debug();

        if (northWest == nullptr)
            return;

        northWest->draw_debug();
        northEast->draw_debug();
        southWest->draw_debug();
        southEast->draw_debug();
    };
    AABB get_bounds() const { return boundary; }
    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
    {
        boundary.center = center;
        boundary.extent = extent;
    }
};