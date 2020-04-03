#pragma once

#include <cstddef>

namespace math {
    template <size_t N, typename T> //requires (std::is_integral_v<T> || std::is_floating_point_v<T>)
    struct vec;

    template <typename T>
    struct vec<2, T> {
        T x, y;

        explicit constexpr vec() noexcept : x(0), y(0) {}
        explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar) {}
        explicit constexpr vec(T x, T y, T z) noexcept : x(x), y(y) {}
    };

    template <typename T>
    struct vec<3, T> {
        T x, y, z;

        explicit constexpr vec() noexcept : x(0), y(0), z(0) {}
        explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
        explicit constexpr vec(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
    };

    template <typename T>
    struct vec<4, T> {
        T x, y, z, w;

        explicit constexpr vec() noexcept : x(0), y(0), z(0), w(0) {}
        explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
        explicit constexpr vec(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}
    };

    using vec2 = vec<2, float>;
    using vec3 = vec<3, float>;
    using vec4 = vec<4, float>;
    using ivec2 = vec<2, int>;
    using ivec3 = vec<3, int>;
    using ivec4 = vec<4, int>;
    using bvec2 = vec<2, bool>;
    using bvec3 = vec<3, bool>;
    using bvec4 = vec<4, bool>;
}
