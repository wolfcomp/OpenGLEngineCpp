#pragma once

#include <vector>
#include "../colliders/AABB.h"
#include <functional>
#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node *next;
};

template <typename T>
class QuadTree
{
private:
    QuadTree *northWest;
    QuadTree *northEast;
    QuadTree *southWest;
    QuadTree *southEast;
    QuadTree *parent;

    AABB boundary;
    bool recalculated = false;

    Node<T> *node = nullptr;

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
        northWest->parent = this;
        northEast->parent = this;
        southWest->parent = this;
        southEast->parent = this;

        if (northWest->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (northEast->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (southWest->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        southEast->insert(node->data);
        delete node;
        node = nullptr;
    };

    bool unsubdivide()
    {
        if (northWest != nullptr)
        {
            if (northWest->node == nullptr && northEast->node == nullptr && southWest->node == nullptr && southEast->node == nullptr)
            {
                delete northWest;
                delete northEast;
                delete southWest;
                delete southEast;
                northWest = nullptr;
                northEast = nullptr;
                southWest = nullptr;
                southEast = nullptr;
                return true;
            }
            else
            {
                northWest->unsubdivide();
                northEast->unsubdivide();
                southWest->unsubdivide();
                southEast->unsubdivide();
            }
            if (northWest->node == nullptr && northEast->node == nullptr && southWest->node == nullptr && southEast->node == nullptr)
            {
                delete northWest;
                delete northEast;
                delete southWest;
                delete southEast;
                northWest = nullptr;
                northEast = nullptr;
                southWest = nullptr;
                southEast = nullptr;
                return true;
            }
        }
        return false;
    }

public:
    QuadTree() : northWest(nullptr), northEast(nullptr), southWest(nullptr), southEast(nullptr) {
                 };
    ~QuadTree()
    {
        delete northWest;
        delete northEast;
        delete southWest;
        delete southEast;
    };

    bool is_leaf() const { return northWest == nullptr; }
    bool insert(T point)
    {
        if (!boundary.contains(point))
            return false;

        if (node == nullptr && is_leaf())
        {
            node = new Node<T>();
            node->data = point;
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

    T pop(T point)
    {
        T result = nullptr;
        if (node != nullptr && node->data == point && is_leaf())
        {
            result = node->data;
            delete node;
            node = nullptr;
            return result;
        }

        if (northWest == nullptr)
            return result;

        result = northWest->pop(point);
        if (result != nullptr)
            return result;
        result = northEast->pop(point);
        if (result != nullptr)
            return result;
        result = southWest->pop(point);
        if (result != nullptr)
            return result;
        result = southEast->pop(point);
        if (result != nullptr)
            return result;

        return result;
    };

    void query_range(const AABB &range, std::vector<T> &found)
    {
        if (!boundary.contains(range))
            return;

        if (node != nullptr && range.contains(node->data))
            found.push_back(node->data);

        if (northWest == nullptr)
            return;

        northWest->query_range(range, found);
        northEast->query_range(range, found);
        southWest->query_range(range, found);
        southEast->query_range(range, found);
    };
    void query_range(const AABB &range, std::vector<T> &found, std::function<bool(const T &)> filter)
    {
        if (!boundary.contains(range))
            return;

        if (node != nullptr && range.contains(node->data) && filter(node->data))
            found.push_back(node->data);

        if (northWest == nullptr)
            return;

        northWest->query_range(range, found, filter);
        northEast->query_range(range, found, filter);
        southWest->query_range(range, found, filter);
        southEast->query_range(range, found, filter);
    };
    void draw_debug(Line *line)
    {
        boundary.draw_debug(line);

        if (northWest == nullptr)
            return;

        northWest->draw_debug(line);
        northEast->draw_debug(line);
        southWest->draw_debug(line);
        southEast->draw_debug(line);
    };
    AABB get_bounds() const { return boundary; }
    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
    {
        boundary.center = center;
        boundary.extent = extent;
    }
    void recalculate();
};