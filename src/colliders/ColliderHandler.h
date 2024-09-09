#pragma once

#include <glm/vec3.hpp>
#include "Collider.h"

#include <string_view>

template <typename T>
constexpr std::string_view type_name();

template <>
constexpr std::string_view type_name<void>()
{
    return "void";
}

namespace detail
{

    using type_name_prober = void;

    template <typename T>
    constexpr std::string_view wrapped_type_name()
    {
        return __FUNCSIG__;
    }

    constexpr std::size_t wrapped_type_name_prefix_length()
    {
        return wrapped_type_name<type_name_prober>()
            .find(type_name<type_name_prober>());
    }

    constexpr std::size_t wrapped_type_name_suffix_length()
    {
        return wrapped_type_name<type_name_prober>().length() - wrapped_type_name_prefix_length() - type_name<type_name_prober>().length();
    }

} // namespace detail

template <typename T>
constexpr std::string_view type_name()
{
    constexpr auto wrapped_name = detail::wrapped_type_name<T>();
    constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();
    constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();
    constexpr auto type_name_length =
        wrapped_name.length() - prefix_length - suffix_length;
    return wrapped_name.substr(prefix_length, type_name_length);
}

namespace ColliderHandler
{
    template <class TA, class TB>
    bool contains(const Collider<TA> &a, const Collider<TB> &b)
    {
        auto t = a.contains(b);
        if (t)
        {
            for (auto &callback : a.onCollisionCallbacks)
            {
                callback(b);
            }
            for (auto &callback : b.onCollisionCallbacks)
            {
                callback(a);
            }
        }
        return t;
    }

    /// @brief Gets the collision normal between two colliders
    /// @param a The collider to get the normal of
    /// @param b the collider that is colliding with a
    /// @return
    glm::vec3 get_collision_normal(ColliderBase *a, ColliderBase *b);

    template <class TA, class TB>
    bool intersects(const Collider<TA> &a, const Collider<TB> &b)
    {
        return contains(a, b);
    }
};