#pragma once

#include <vector>
#include "../colliders/AABB.h"
#include "Node.h"

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
    OcTree *parent;

    AABB boundary;
    bool recalculated = false;

    Node<T> *node = nullptr;

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
        northWestUpper->parent = this;
        northEastUpper->parent = this;
        southWestUpper->parent = this;
        southEastUpper->parent = this;
        northWestLower->parent = this;
        northEastLower->parent = this;
        southWestLower->parent = this;
        southEastLower->parent = this;

        if (northWestUpper->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (northEastUpper->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (southWestUpper->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (southEastUpper->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (northWestLower->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (northEastLower->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        if (southWestLower->insert(node->data))
        {
            delete node;
            node = nullptr;
            return;
        }
        southEastLower->insert(node->data);
        delete node;
        node = nullptr;
    }

    bool unsubdivide()
    {
        if (northWestUpper != nullptr)
        {
            northWestUpper->unsubdivide();
            northEastUpper->unsubdivide();
            southWestUpper->unsubdivide();
            southEastUpper->unsubdivide();
            northWestLower->unsubdivide();
            northEastLower->unsubdivide();
            southWestLower->unsubdivide();
            southEastLower->unsubdivide();
            if (northWestUpper->node == nullptr && northEastUpper->node == nullptr && southWestUpper->node == nullptr && southEastUpper->node == nullptr &&
                northWestLower->node == nullptr && northEastLower->node == nullptr && southWestLower->node == nullptr && southEastLower->node == nullptr)
            {
                delete northWestUpper;
                delete northEastUpper;
                delete southWestUpper;
                delete southEastUpper;
                delete northWestLower;
                delete northEastLower;
                delete southWestLower;
                delete southEastLower;
                northWestUpper = nullptr;
                northEastUpper = nullptr;
                southWestUpper = nullptr;
                southEastUpper = nullptr;
                northWestLower = nullptr;
                northEastLower = nullptr;
                southWestLower = nullptr;
                southEastLower = nullptr;
                return true;
            }
        }
        return false;
    }

public:
    OcTree() : northWestUpper(nullptr), northEastUpper(nullptr), southWestUpper(nullptr), southEastUpper(nullptr),
               northWestLower(nullptr), northEastLower(nullptr), southWestLower(nullptr), southEastLower(nullptr) {
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
        if (northWestUpper == nullptr)
            return result;
        result = northWestUpper->pop(point);
        if (result != nullptr)
            return result;
        result = northEastUpper->pop(point);
        if (result != nullptr)
            return result;
        result = southWestUpper->pop(point);
        if (result != nullptr)
            return result;
        result = southEastUpper->pop(point);
        if (result != nullptr)
            return result;
        result = northWestLower->pop(point);
        if (result != nullptr)
            return result;
        result = northEastLower->pop(point);
        if (result != nullptr)
            return result;
        result = southWestLower->pop(point);
        if (result != nullptr)
            return result;
        result = southEastLower->pop(point);
        return result;
    }
    void query_range(const AABB &range, std::vector<T> &found)
    {
        if (!boundary.contains(range))
            return;

        if (node != nullptr && range.contains(node->data))
            found.push_back(node->data);

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

    void query_range(const AABB &range, std::vector<T> &found, std::function<bool(const T &)> filter)
    {
        if (!boundary.contains(range))
            return;

        if (node != nullptr && range.contains(node->data) && filter(node->data))
            found.push_back(node->data);

        if (is_leaf())
            return;

        northWestUpper->query_range(range, found, filter);
        northEastUpper->query_range(range, found, filter);
        southWestUpper->query_range(range, found, filter);
        southEastUpper->query_range(range, found, filter);
        northWestLower->query_range(range, found, filter);
        northEastLower->query_range(range, found, filter);
        southWestLower->query_range(range, found, filter);
        southEastLower->query_range(range, found, filter);
    };

    void draw_debug(Line *line)
    {
        boundary.draw_debug(line);

        if (is_leaf())
            return;

        northWestUpper->draw_debug(line);
        northEastUpper->draw_debug(line);
        southWestUpper->draw_debug(line);
        southEastUpper->draw_debug(line);
        northWestLower->draw_debug(line);
        northEastLower->draw_debug(line);
        southWestLower->draw_debug(line);
        southEastLower->draw_debug(line);
    };
    AABB get_bounds() const { return boundary; }
    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent)
    {
        boundary.center = center;
        boundary.extent = extent;
    };
    void recalculate();
};