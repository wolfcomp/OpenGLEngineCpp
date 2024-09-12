#include "OcTree.h"
#include "../objects/base/SceneObject.h"
#include "../objects/base/SceneUpdatableObject.h"
#include <vector>

template <typename T>
void OcTree<T>::recalculate()
{
    throw "Cannot recalculate OcTree of unknown type";
}

template <>
void OcTree<SceneUpdatableObject *>::recalculate()
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
void OcTree<SceneObject *>::recalculate()
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