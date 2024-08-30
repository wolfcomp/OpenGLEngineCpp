#include "QuadTree.h"

template <typename T>
QuadTree<T>::QuadTree()
{
    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;
}

template <typename T>
QuadTree<T>::~QuadTree()
{
    delete northWest;
    delete northEast;
    delete southWest;
    delete southEast;
}

template <typename T>
void QuadTree<T>::insert(const T &point)
{
    if (!boundary.contains(point))
        return;

    if (points.size() < capacity && is_leaf())
    {
        points.push_back(point);
        return;
    }

    if (is_leaf())
        subdivide();

    northWest->insert(point);
    northEast->insert(point);
    southWest->insert(point);
    southEast->insert(point);
}

template <typename T>
void QuadTree<T>::subdivide()
{
    glm::vec3 center = boundary.center;
    glm::vec3 extent = boundary.extent;

    glm::vec3 nw_center = center + glm::vec3(-extent.x / 2, extent.y / 2, 0.0f);
    glm::vec3 ne_center = center + glm::vec3(extent.x / 2, extent.y / 2, 0.0f);
    glm::vec3 sw_center = center + glm::vec3(-extent.x / 2, -extent.y / 2, 0.0f);
    glm::vec3 se_center = center + glm::vec3(extent.x / 2, -extent.y / 2, 0.0f);

    northWest = new QuadTree<T>(AABB(nw_center, extent / 2.0f));
    northEast = new QuadTree<T>(AABB(ne_center, extent / 2.0f));
    southWest = new QuadTree<T>(AABB(sw_center, extent / 2.0f));
    southEast = new QuadTree<T>(AABB(se_center, extent / 2.0f));

    for (const T &point : points)
    {
        northWest->insert(point);
        northEast->insert(point);
        southWest->insert(point);
        southEast->insert(point);
    }

    points.clear();
}

template <typename T>
void QuadTree<T>::query_range(const AABB &range, std::vector<T> &found)
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
}

template <typename T>
void QuadTree<T>::draw_debug()
{
    boundary.draw();

    if (northWest == nullptr)
        return;

    northWest->draw_debug();
    northEast->draw_debug();
    southWest->draw_debug();
    southEast->draw_debug();
}