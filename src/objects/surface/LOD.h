#pragma once

#include "../base/GameObject.h"

class LOD : public GameObject
{
private:
    LOD *parent;
    LOD *child;
    glm::vec3 lod_resolution;
    bool is_visible = false;

public:
    LOD(std::vector<Vertex> vertices, std::vector<unsigned> indices) : GameObject(vertices, indices) {}
    ~LOD()
    {
        if (child != nullptr)
            delete child;
    }

    void set_parent(LOD *parent) { this->parent = parent; }
    void set_child(LOD *child) { this->child = child; }
    void set_lod_resolution(glm::vec3 lod_resolution) { this->lod_resolution = lod_resolution; }
    void set_visible(bool is_visible) { this->is_visible = is_visible; }
    glm::vec3 get_lod_resolution() { return lod_resolution; }
    bool get_visible() { return is_visible; }
    LOD *get_parent() { return parent; }
    LOD *get_child() { return child; }
    LOD *get_current(glm::vec3 pos)
    {
        auto cur = this;
        if (child == nullptr)
            return this;
        while (cur->child != nullptr)
        {
            if (glm::abs(pos - cur->get_position()) < lod_resolution)
                cur = cur->child;
            else
                break;
        }
        return cur;
    }
    void draw(const Camera &camera) const override
    {
    }
};