#pragma once

#include "../uuid.h"
#include "components/base.h"

template <typename T>
struct ECSValuePair
{
    UUID id;
    T value;
};

struct typename_info
{
    const char *name;

    typename_info() : name("") {}
    typename_info(const char *name) : name(name) {}

    bool operator==(const char *other)
    {
        return strcmp(name, other) == 0;
    }

    bool operator==(const typename_info &other)
    {
        return strcmp(name, other.name) == 0;
    }

    typename_info &operator=(const char *other)
    {
        name = other;
        return *this;
    }

    typename_info &operator=(const typename_info &other)
    {
        name = other.name;
        return *this;
    }
};

template <typename T>
struct ECSMap
{
private:
    ECSValuePair<T> *data;
    int size;
    int capacity;

    void expand()
    {
        capacity *= 2;
        ECSValuePair<T> *new_data = new ECSValuePair<T>[capacity];
        for (int i = 0; i < size; i++)
        {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }

public:
    ECSMap() : ECSMap(1) {}

    ECSMap(int capacity)
    {
        this->capacity = capacity;
        this->size = 0;
        this->data = new ECSValuePair<T>[capacity];
    }

    ~ECSMap()
    {
        delete[] data;
    }

    void insert(UUID id, T value)
    {
        if (size >= capacity)
        {
            expand();
        }
        data[size++] = {id, value};
    }

    T *get(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].id == id)
            {
                return &data[i].value;
            }
        }
        return nullptr;
    }

    template <typename U>
    U *get(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].id == id && dynamic_cast<U *>(&data[i].value) != nullptr)
            {
                return dynamic_cast<U *>(&data[i].value);
            }
        }
        return nullptr;
    }

    void remove(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].id == id)
            {
                for (int j = i; j < size - 1; j++)
                {
                    data[j] = data[j + 1];
                }
                size--;
                return;
            }
        }
    }

    int get_size()
    {
        return size;
    }

    T *operator[](UUID id)
    {
        return get(id);
    }

    ECSMap<T> &operator=(const ECSMap<T> &other)
    {
        if (this == &other)
        {
            return *this;
        }
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new ECSValuePair<T>[capacity];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    bool operator==(const ECSMap<T> &other)
    {
        if (size != other.size)
        {
            return false;
        }
        for (int i = 0; i < size; i++)
        {
            if (data[i].id != other.data[i].id || data[i].value != other.data[i].value)
            {
                return false;
            }
        }
        return true;
    }

    bool operator==(const type_info &other)
    {
        return typeid(T) == other;
    }
};

struct ECSGlobalMap
{
private:
    ECSMap<BaseComponent *> *data;
    typename_info *types;
    int size;
    int capacity;

    void expand()
    {
        capacity *= 2;
        ECSMap<BaseComponent *> *new_data = new ECSMap<BaseComponent *>[capacity];
        typename_info *new_types = new typename_info[capacity];
        for (int i = 0; i < size; i++)
        {
            new_data[i] = data[i];
            new_types[i] = types[i];
        }
        delete[] data;
        data = new_data;
    }

public:
    ECSGlobalMap() : ECSGlobalMap(1) {}

    ECSGlobalMap(int capacity)
    {
        this->capacity = capacity;
        this->size = 0;
        this->data = new ECSMap<BaseComponent *>[capacity];
    }

    ~ECSGlobalMap()
    {
        delete[] data;
    }

    template <typename T>
    void insert(UUID id, T value)
    {
        for (int i = 0; i < size; i++)
        {
            if (types[i] == typeid(T).name())
            {
                data[i].insert(id, value);
                return;
            }
        }
        if (size >= capacity)
        {
            expand();
        }
        types[size] = typeid(T).name();
        data[size++].insert(id, value);
    }

    template <typename T>
    T *get(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (types[i] != typeid(T).name())
                continue;

            return data[i].get(id);
        }
        return nullptr;
    }

    template <typename T>
    void remove(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].get(id))
            {
                data[i].remove(id);
                return;
            }
        }
    }

    int get_size()
    {
        return size;
    }
};