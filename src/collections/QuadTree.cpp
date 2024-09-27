#include "QuadTree.h"
#include "../objects/base/GameObject.h"
#include "../objects/base/GameObject.h"
#include <vector>

template <typename T>
void QuadTree<T>::recalculate()
{
    throw "Cannot recalculate QuadTree of unknown type";
}

template <>
void QuadTree<GameObject *>::recalculate()
{
    std::vector<GameObject *> all_objects;
    query_range(boundary, all_objects, [](const GameObject *object)
                { return object->get_has_updated(); });

    for (auto &object : all_objects)
    {
        pop(object);
    }
    unsubdivide();
    for (auto &object : all_objects)
    {
        insert(object);
    }
}