#pragma once

class BaseSystem
{
public:
    virtual ~BaseSystem() = default;
    virtual void update(float delta_time) = 0;
};