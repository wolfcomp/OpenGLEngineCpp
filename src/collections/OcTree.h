#pragma once

#include <vector>
#include "../colliders/AABB.h"

template <typename T>
class OcTree
{
private:
    OcTree *northWestUpper;
    OcTree *northEastUpper;
    OcTree *southWestUpper;
    OcTree *southEastUpper;
    OcTree *northWestLower;
    OcTree *northEastLower;
    OcTree *southWestLower;
    OcTree *southEastLower;

    AABB boundary;

    const unsigned capacity = 8;

    std::vector<T> points;

    void subdivide()
    {
        glm::vec3 center = boundary.center;
        glm::vec3 extent = boundary.extent;
        glm::vec3 extentDivide = extent / 2.0f;

        glm::vec3 nwu_center = center + glm::vec3(-extent.x / 2, extent.y / 2, extent.z / 2);
        glm::vec3 neu_center = center + glm::vec3(extent.x / 2, extent.y / 2, extent.z / 2);
        glm::vec3 swu_center = center + glm::vec3(-extent.x / 2, extent.y / 2, -extent.z / 2);
        glm::vec3 seu_center = center + glm::vec3(extent.x / 2, extent.y / 2, -extent.z / 2);
        glm::vec3 nwl_center = center + glm::vec3(-extent.x / 2, -extent.y / 2, extent.z / 2);
        glm::vec3 nel_center = center + glm::vec3(extent.x / 2, -extent.y / 2, extent.z / 2);
        glm::vec3 swl_center = center + glm::vec3(-extent.x / 2, -extent.y / 2, -extent.z / 2);
        glm::vec3 sel_center = center + glm::vec3(extent.x / 2, -extent.y / 2, -extent.z / 2);

        northWestUpper = new OcTree<T>();
        northWestUpper->set_bounds(nwu_center, extentDivide);
        northEastUpper = new OcTree<T>();
        northEastUpper->set_bounds(neu_center, extentDivide);
        southWestUpper = new OcTree<T>();
        southWestUpper->set_bounds(swu_center, extentDivide);
        southEastUpper = new OcTree<T>();
        southEastUpper->set_bounds(seu_center, extentDivide);
        northWestLower = new OcTree<T>();
        northWestLower->set_bounds(nwl_center, extentDivide);
        northEastLower = new OcTree<T>();
        northEastLower->set_bounds(nel_center, extentDivide);
        southWestLower = new OcTree<T>();
        southWestLower->set_bounds(swl_center, extentDivide);
        southEastLower = new OcTree<T>();
        southEastLower->set_bounds(sel_center, extentDivide);
    }

public:
    OcTree() : northWestUpper(nullptr), northEastUpper(nullptr), southWestUpper(nullptr), southEastUpper(nullptr),
               northWestLower(nullptr), northEastLower(nullptr), southWestLower(nullptr), southEastLower(nullptr)
    {
        points.reserve(capacity);
    };
    ~OcTree()
    {
        delete northWestUpper;
        delete northEastUpper;
        delete southWestUpper;
        delete southEastUpper;
        delete northWestLower;
        delete northEastLower;
        delete southWestLower;
        delete southEastLower;
    };

    bool is_leaf() const { return northWestUpper == nullptr; }
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

        if (northWestUpper->insert(point))
            return true;
        if (northEastUpper->insert(point))
            return true;
        if (southWestUpper->insert(point))
            return true;
        if (southEastUpper->insert(point))
            return true;
        if (northWestLower->insert(point))
            return true;
        if (northEastLower->insert(point))
            return true;
        if (southWestLower->insert(point))
            return true;
        if (southEastLower->insert(point))
            return true;
        return false;
    };
    void query_range(const AABB &range, std::vector<T> &found)
    {
        if (!boundary.contains(range))
            return;

        for (const T &point : points)
        {
            if (range.contains(point))
                found.push_back(point);
        }

        if (is_leaf())
            return;

        northWestUpper->query_range(range, found);
        northEastUpper->query_range(range, found);
        southWestUpper->query_range(range, found);
        southEastUpper->query_range(range, found);
        northWestLower->query_range(range, found);
        northEastLower->query_range(range, found);
        southWestLower->query_range(range, found);
        southEastLower->query_range(range, found);
    };

    void draw_debug()
    {
        boundary.draw_debug();

        if (is_leaf())
            return;

        northWestUpper->draw_debug();
        northEastUpper->draw_debug();
        southWestUpper->draw_debug();
        southEastUpper->draw_debug();
        northWestLower->draw_debug();
        northEastLower->draw_debug();
        southWestLower->draw_debug();
        southEastLower->draw_debug();
    };
    AABB get_bounds() const { return boundary; }
    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
    {
        boundary.center = center;
        boundary.extent = extent;
    };
};