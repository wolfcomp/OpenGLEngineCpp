/// @ref core
/// @file glm/detail/type_vec4.hpp

#pragma once

#include "qualifier.hpp"
#if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
#include "_swizzle.hpp"
#elif GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
#include "_swizzle_func.hpp"
#endif
#include <cstddef>

namespace glm
{
	template <typename T, qualifier Q>
	struct vec<4, T, Q>
	{
		// -- Implementation detail --

		typedef T value_type;
		typedef vec<4, T, Q> type;
		typedef vec<4, bool, Q> bool_type;

		// -- Data --

#if GLM_SILENT_WARNINGS == GLM_ENABLE
#if GLM_COMPILER & GLM_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif GLM_COMPILER & GLM_COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#elif GLM_COMPILER & GLM_COMPILER_VC
#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#endif
#endif

#if GLM_CONFIG_XYZW_ONLY
		T x, y, z, w;
#elif GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
		union
		{
			struct
			{
				T x, y, z, w;
			};
			struct
			{
				T r, g, b, a;
			};
			struct
			{
				T s, t, p, q;
			};

			typename detail::storage<4, T, detail::is_aligned<Q>::value>::type data;

#if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
			GLM_SWIZZLE4_2_MEMBERS(T, Q, x, y, z, w)
			GLM_SWIZZLE4_2_MEMBERS(T, Q, r, g, b, a)
			GLM_SWIZZLE4_2_MEMBERS(T, Q, s, t, p, q)
			GLM_SWIZZLE4_3_MEMBERS(T, Q, x, y, z, w)
			GLM_SWIZZLE4_3_MEMBERS(T, Q, r, g, b, a)
			GLM_SWIZZLE4_3_MEMBERS(T, Q, s, t, p, q)
			GLM_SWIZZLE4_4_MEMBERS(T, Q, x, y, z, w)
			GLM_SWIZZLE4_4_MEMBERS(T, Q, r, g, b, a)
			GLM_SWIZZLE4_4_MEMBERS(T, Q, s, t, p, q)
#endif
		};
#else
		union
		{
			T x, r, s;
		};
		union
		{
			T y, g, t;
		};
		union
		{
			T z, b, p;
		};
		union
		{
			T w, a, q;
		};

#if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
		GLM_SWIZZLE_GEN_VEC_FROM_VEC4(T, Q)
#endif
#endif

#if GLM_SILENT_WARNINGS == GLM_ENABLE
#if GLM_COMPILER & GLM_COMPILER_CLANG
#pragma clang diagnostic pop
#elif GLM_COMPILER & GLM_COMPILER_GCC
#pragma GCC diagnostic pop
#elif GLM_COMPILER & GLM_COMPILER_VC
#pragma warning(pop)
#endif
#endif

		// -- Component accesses --

		typedef length_t length_type;

		/// Return the count of components of the vector
		GLM_FUNC_DECL static GLM_CONSTEXPR length_type length() { return 4; }

		GLM_FUNC_DECL GLM_CONSTEXPR T &operator[](length_type i);
		GLM_FUNC_DECL GLM_CONSTEXPR T const &operator[](length_type i) const;

		// -- Implicit basic constructors --

		GLM_FUNC_DECL GLM_CONSTEXPR vec() GLM_DEFAULT;
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<4, T, Q> const &v) GLM_DEFAULT;
		template <qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<4, T, P> const &v);

		// -- Explicit basic constructors --

		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(T scalar);
		GLM_FUNC_DECL GLM_CONSTEXPR vec(T x, T y, T z, T w);

		// -- Conversion scalar constructors --

		template <typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR explicit vec(vec<1, U, P> const &v);

		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, Y _y, Z _z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, Y _y, Z _z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const &_y, Z _z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_y, Z _z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, Y _y, vec<1, Z, Q> const &_z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, Y _y, vec<1, Z, Q> const &_z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const &_y, vec<1, Z, Q> const &_z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_y, vec<1, Z, Q> const &_z, W _w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, Y _y, Z _z, vec<1, W, Q> const &_w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const &_y, Z _z, vec<1, W, Q> const &_w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_y, Z _z, vec<1, W, Q> const &_w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, Y _y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, Y _y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(X _x, vec<1, Y, Q> const &_y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w);
		template <typename X, typename Y, typename Z, typename W>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_Y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w);

		// -- Conversion vector constructors --

		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const &_xy, B _z, C _w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const &_xy, vec<1, B, P> const &_z, C _w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const &_xy, B _z, vec<1, C, P> const &_w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const &_xy, vec<1, B, P> const &_z, vec<1, C, P> const &_w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<2, B, P> const &_yz, C _w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const &_x, vec<2, B, P> const &_yz, C _w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<2, B, P> const &_yz, vec<1, C, P> const &_w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const &_x, vec<2, B, P> const &_yz, vec<1, C, P> const &_w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, B _y, vec<2, C, P> const &_zw);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const &_x, B _y, vec<2, C, P> const &_zw);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<1, B, P> const &_y, vec<2, C, P> const &_zw);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, typename C, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const &_x, vec<1, B, P> const &_y, vec<2, C, P> const &_zw);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<3, A, P> const &_xyz, B _w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<3, A, P> const &_xyz, vec<1, B, P> const &_w);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(A _x, vec<3, B, P> const &_yzw);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<1, A, P> const &_x, vec<3, B, P> const &_yzw);
		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename A, typename B, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(vec<2, A, P> const &_xy, vec<2, B, P> const &_zw);

		/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
		template <typename U, qualifier P>
		GLM_FUNC_DECL GLM_CONSTEXPR GLM_EXPLICIT vec(vec<4, U, P> const &v);

		// -- Swizzle constructors --
#if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
		template <int E0, int E1, int E2, int E3>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<4, T, Q, E0, E1, E2, E3> const &that)
		{
			*this = that();
		}

		template <int E0, int E1, int F0, int F1>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<2, T, Q, E0, E1, -1, -2> const &v, detail::_swizzle<2, T, Q, F0, F1, -1, -2> const &u)
		{
			*this = vec<4, T, Q>(v(), u());
		}

		template <int E0, int E1>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(T const &x, T const &y, detail::_swizzle<2, T, Q, E0, E1, -1, -2> const &v)
		{
			*this = vec<4, T, Q>(x, y, v());
		}

		template <int E0, int E1>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(T const &x, detail::_swizzle<2, T, Q, E0, E1, -1, -2> const &v, T const &w)
		{
			*this = vec<4, T, Q>(x, v(), w);
		}

		template <int E0, int E1>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<2, T, Q, E0, E1, -1, -2> const &v, T const &z, T const &w)
		{
			*this = vec<4, T, Q>(v(), z, w);
		}

		template <int E0, int E1, int E2>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(detail::_swizzle<3, T, Q, E0, E1, E2, -1> const &v, T const &w)
		{
			*this = vec<4, T, Q>(v(), w);
		}

		template <int E0, int E1, int E2>
		GLM_FUNC_DECL GLM_CONSTEXPR vec(T const &x, detail::_swizzle<3, T, Q, E0, E1, E2, -1> const &v)
		{
			*this = vec<4, T, Q>(x, v());
		}
#endif // GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR

		// -- Unary arithmetic operators --

		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator=(vec<4, T, Q> const &v) GLM_DEFAULT;

		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator+=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator+=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator+=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator-=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator-=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator-=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator*=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator*=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator*=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator/=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator/=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator/=(vec<4, U, Q> const &v);

		// -- Increment and decrement operators --

		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator++();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator--();
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator++(int);
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator--(int);

		// -- Unary bit operators --

		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator%=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator%=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator%=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator&=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator&=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator&=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator|=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator|=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator|=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator^=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator^=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator^=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator<<=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator<<=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator<<=(vec<4, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator>>=(U scalar);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator>>=(vec<1, U, Q> const &v);
		template <typename U>
		GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> &operator>>=(vec<4, U, Q> const &v);

		// -- Conversion functions --
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xxx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xxy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xxz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xxw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xyx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xyy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xyz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xyw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xzx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xzy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xzz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xzw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xwx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xwy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xwz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> xww() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yxx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yxy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yxz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yxw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yyx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yyy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yyz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yyw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yzx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yzy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yzz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yzw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> ywx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> ywy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> ywz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> yww() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zxx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zxy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zxz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zxw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zyx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zyy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zyz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zyw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zzx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zzy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zzz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zzw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zwx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zwy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zwz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> zww() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wxx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wxy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wxz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wxw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wyx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wyy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wyz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wyw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wzx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wzy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wzz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wzw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wwx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wwy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> wwz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<3, T, Q> www() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> xx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> xy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> xz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> xw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> yx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> yy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> yz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> yw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> zx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> zy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> zz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> zw() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> wx() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> wy() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> wz() const;
		GLM_FUNC_DECL GLM_CONSTEXPR vec<2, T, Q> ww() const;
	};

	// -- Unary operators --

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v);

	// -- Binary operators --

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v, T const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v, T const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const &v, T const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const &v, T const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const &v, T scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const &v, vec<1, T, Q> const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<1, T, Q> const &scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const &v, T scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const &v, vec<1, T, Q> const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<1, T, Q> const &scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const &v, T scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const &v, vec<1, T, Q> const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<1, T, Q> const &scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const &v, T scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const &v, vec<1, T, Q> const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<1, T, Q> const &scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const &v, T scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const &v, vec<1, T, Q> const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<1, T, Q> const &scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const &v, T scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const &v, vec<1, T, Q> const &scalar);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(T scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<1, T, Q> const &scalar, vec<4, T, Q> const &v);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, T, Q> operator~(vec<4, T, Q> const &v);

	// -- Boolean operators --

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator==(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR bool operator!=(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2);

	template <qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, bool, Q> operator&&(vec<4, bool, Q> const &v1, vec<4, bool, Q> const &v2);

	template <qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<4, bool, Q> operator||(vec<4, bool, Q> const &v1, vec<4, bool, Q> const &v2);
} // namespace glm

#ifndef GLM_EXTERNAL_TEMPLATE
#include "type_vec4.inl"
#endif // GLM_EXTERNAL_TEMPLATE
