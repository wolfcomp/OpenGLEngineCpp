#include "QuadTree.h"
#include "../objects/base/SceneObject.h"
#include "../objects/base/SceneUpdatableObject.h"
#include <vector>

template <typename T>
void QuadTree<T>::recalculate()
{
    throw "Cannot recalculate QuadTree of unknown type";
}

template <>
void QuadTree<SceneUpdatableObject *>::recalculate()
{
    std::vector<SceneUpdatableObject *> all_objects;
    query_range(boundary, all_objects, [](const SceneUpdatableObject *object)
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

template <>
void QuadTree<SceneObject *>::recalculate()
{
    std::vector<SceneObject *> all_objects;
    query_range(boundary, all_objects);

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