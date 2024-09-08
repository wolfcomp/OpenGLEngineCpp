#include "ColliderHandler.h"
#include "AABB.h"
#include "ConvexHull.h"
#include "SphereCollider.h"

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

template <>
static glm::vec3 ColliderHandler::get_collision_normal(AABB &a, AABB &b)
{
    glm::vec3 normal = b.center - a.center;
    float x_overlap = a.extent.x + b.extent.x - abs(normal.x);
    float y_overlap = a.extent.y + b.extent.y - abs(normal.y);
    float z_overlap = a.extent.z + b.extent.z - abs(normal.z);
    if (x_overlap < y_overlap && x_overlap < z_overlap)
    {
        normal.y = 0;
        normal.z = 0;
    }
    else if (y_overlap < x_overlap && y_overlap < z_overlap)
    {
        normal.x = 0;
        normal.z = 0;
    }
    else
    {
        normal.x = 0;
        normal.y = 0;
    }
    return glm::normalize(normal);
}

template <>
static glm::vec3 ColliderHandler::get_collision_normal(AABB &a, SphereCollider &b)
{
    glm::vec3 closest = glm::vec3(
        glm::clamp(b.center.x, a.center.x - a.extent.x, a.center.x + a.extent.x),
        glm::clamp(b.center.y, a.center.y - a.extent.y, a.center.y + a.extent.y),
        glm::clamp(b.center.z, a.center.z - a.extent.z, a.center.z + a.extent.z));
    glm::vec3 normal = b.center - closest;
    if (glm::length(normal) < b.radius)
    {
        return glm::normalize(normal);
    }
    return glm::vec3(0);
}

template <>
static glm::vec3 ColliderHandler::get_collision_normal(SphereCollider &a, AABB &b)
{
    return -get_collision_normal(b, a);
}

template <>
static glm::vec3 ColliderHandler::get_collision_normal(SphereCollider &a, SphereCollider &b)
{
    glm::vec3 normal = a.center - b.center;
    float distance = glm::length(normal);
    if (distance < a.radius + b.radius)
    {
        return glm::normalize(normal);
    }
    return glm::normalize(normal);
}

template <class TA, class TB>
static glm::vec3 ColliderHandler::get_collision_normal(Collider<TA> &a, Collider<TB> &b)
{
    auto sca = dynamic_cast<SphereCollider *>(&a);
    auto scb = dynamic_cast<SphereCollider *>(&b);
    auto aabba = dynamic_cast<AABB *>(&a);
    auto aabbb = dynamic_cast<AABB *>(&b);
    if (sca != nullptr && scb != nullptr)
    {
        return get_collision_normal(*sca, *scb);
    }
    else if (sca != nullptr && aabbb != nullptr)
    {
        return get_collision_normal(*sca, *aabbb);
    }
    else if (aabba != nullptr && scb != nullptr)
    {
        return get_collision_normal(*aabba, *scb);
    }
    else if (aabba != nullptr && aabbb != nullptr)
    {
        return get_collision_normal(*aabba, *aabbb);
    }
    auto at = type_name<decltype(a)>();
    auto bt = type_name<decltype(b)>();
    std::string message;
    sprintf(message.data(), "Not implemented for %s and %s", at.data(), bt.data());
    throw message;
}