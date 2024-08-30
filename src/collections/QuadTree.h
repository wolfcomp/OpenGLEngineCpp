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

public:
    QuadTree();
    ~QuadTree();

    bool is_leaf() const { return northWest == nullptr; }
    void insert(const T &point);
    void subdivide();
    void query_range(const AABB &range, std::vector<T> &found);
    void draw_debug();
    AABB get_bounds() const { return boundary; }
};