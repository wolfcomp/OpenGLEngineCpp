#pragma once

#include "../uuid.h"
#include "components/base.h"

template <typename T>
struct ECSValuePair
{
    UUID id;
    T *value;
    ECSValuePair() : id(UUID::empty()), value(nullptr) {}
    ECSValuePair(UUID id, T *value) : id(id), value(value) {}
    virtual bool operator==(const ECSValuePair<T> &other)
    {
        return id == other.id && value == other.value;
    }
    virtual bool operator==(const type_info &other)
    {
        return typeid(T) == other;
    }
    virtual bool operator!=(const ECSValuePair<T> &other)
    {
        return !(*this == other);
    }
    virtual bool operator!=(const type_info &other)
    {
        return !(*this == other);
    }
};

template <typename T>
struct ECSMap
{
private:
    ECSValuePair<T> *data;
    int size;
    int capacity;

    inline void expand()
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

    inline void insert(UUID id, T *value)
    {
        if (size >= capacity)
        {
            expand();
        }
        data[size++] = ECSValuePair{id, value};
    }

    inline T *get(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].id == id)
            {
                return data[i].value;
            }
        }
        return nullptr;
    }

    inline T *get(int index)
    {
        if (index < 0 || index >= size)
        {
            return nullptr;
        }
        return data[index].value;
    }

    template <typename U>
    inline U *get(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].id == id && dynamic_cast<U *>(&data[i].value) != nullptr)
            {
                return dynamic_cast<U *>(data[i].value);
            }
        }
        return nullptr;
    }

    template <typename U>
    inline ECSValuePair<U> *get_value_pair(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].id == id && dynamic_cast<U *>(&data[i].value) != nullptr)
            {
                return dynamic_cast<ECSValuePair<U> *>(&data[i]);
            }
        }
        return nullptr;
    }

    template <typename U>
    inline ECSValuePair<U> *get_value_pair(int i)
    {
        if (i < 0 || i >= size)
        {
            return nullptr;
        }
        if (dynamic_cast<U *>(data[i].value) != nullptr)
        {
            return new ECSValuePair<U>(data[i].id, dynamic_cast<U *>(data[i].value));
        }
        return nullptr;
    }

    inline void remove(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i].id == id)
            {
                delete data[i].value;
                for (int j = i; j < size - 1; j++)
                {
                    data[j] = data[j + 1];
                }
                size--;
                return;
            }
        }
    }

    inline int get_size()
    {
        return size;
    }

    inline T *operator[](UUID id)
    {
        return get(id);
    }

    inline ECSMap<T> &operator=(const ECSMap<T> &other)
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

    inline bool operator==(const ECSMap<T> &other)
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

    inline bool operator==(const type_info &other)
    {
        return typeid(T) == other;
    }
};

struct ECSGlobalMap
{
private:
    ECSMap<BaseComponent> *data;
    std::string *types;
    int size;
    int capacity;

    inline void expand()
    {
        capacity *= 2;
        ECSMap<BaseComponent> *new_data = new ECSMap<BaseComponent>[capacity];
        std::string *new_types = new std::string[capacity];
        for (int i = 0; i < size; i++)
        {
            new_data[i] = data[i];
            new_types[i] = types[i];
        }
        delete[] data;
        data = new_data;
        delete[] types;
        types = new_types;
    }

public:
    ECSGlobalMap() : ECSGlobalMap(1) {}

    ECSGlobalMap(int capacity)
    {
        this->capacity = capacity;
        this->size = 0;
        this->data = new ECSMap<BaseComponent>[capacity];
        this->types = new std::string[capacity];
    }

    ~ECSGlobalMap()
    {
        delete[] data;
        delete[] types;
    }

    template <typename T>
    inline void insert(UUID id, T *value)
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
    inline T *get(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (types[i] != typeid(T).name())
                continue;

            return dynamic_cast<T *>(data[i].get(id));
        }
        return nullptr;
    }

    template <typename T>
    inline void remove(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            if (types[i] == typeid(T).name())
            {
                data[i].remove(id);
                return;
            }
        }
    }

    inline void remove_all(UUID id)
    {
        for (int i = 0; i < size; i++)
        {
            data[i].remove(id);
        }
    }

    template <typename T>
    inline ECSMap<BaseComponent> *get()
    {
        for (int i = 0; i < size; i++)
        {
            if (types[i] == typeid(T).name())
            {
                return &data[i];
            }
        }
        return nullptr;
    }

    inline int get_size()
    {
        return size;
    }
};