#include "physcis.h"

template <>
void ECSGlobalMap::insert<PhysicsComponent>(UUID id, PhysicsComponent *value)
{
    for (int i = 0; i < size; i++)
    {
        if (types[i] == typeid(PhysicsComponent).name())
        {
            data[i].insert(id, value);
            return;
        }
    }
    if (size >= capacity)
    {
        expand();
    }
    types[size] = typeid(PhysicsComponent).name();
    data[size++].insert(id, value);
}