/// @ref core

#include "compute_vector_relational.hpp"

namespace glm
{
	namespace detail
	{
		template <typename T, qualifier Q, bool Aligned>
		struct compute_vec4_add
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
			}
		};

		template <typename T, qualifier Q, bool Aligned>
		struct compute_vec4_sub
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
			}
		};

		template <typename T, qualifier Q, bool Aligned>
		struct compute_vec4_mul
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
			}
		};

		template <typename T, qualifier Q, bool Aligned>
		struct compute_vec4_div
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
			}
		};

		template <typename T, qualifier Q, bool Aligned>
		struct compute_vec4_mod
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x % b.x, a.y % b.y, a.z % b.z, a.w % b.w);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_and
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x & b.x, a.y & b.y, a.z & b.z, a.w & b.w);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_or
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x | b.x, a.y | b.y, a.z | b.z, a.w | b.w);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_xor
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x ^ b.x, a.y ^ b.y, a.z ^ b.z, a.w ^ b.w);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_shift_left
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x << b.x, a.y << b.y, a.z << b.z, a.w << b.w);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_shift_right
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &a, vec<4, T, Q> const &b)
			{
				return vec<4, T, Q>(a.x >> b.x, a.y >> b.y, a.z >> b.z, a.w >> b.w);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_equal
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static bool call(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
			{
				return detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.x, v2.x) &&
					   detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.y, v2.y) &&
					   detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.z, v2.z) &&
					   detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.w, v2.w);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_nequal
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static bool call(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
			{
				return !compute_vec4_equal<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v1, v2);
			}
		};

		template <typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
		struct compute_vec4_bitwise_not
		{
			GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<4, T, Q> call(vec<4, T, Q> const &v)
			{
				return vec<4, T, Q>(~v.x, ~v.y, ~v.z, ~v.w);
			}
		};
	} // namespace detail

	// -- Implicit basic constructors --

#if GLM_CONFIG_DEFAULTED_FUNCTIONS == GLM_DISABLE
	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec()
#if GLM_CONFIG_CTOR_INIT != GLM_CTOR_INIT_DISABLE
		: x(0), y(0), z(0), w(0)
#endif
	{
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<4, T, Q> const &v)
		: x(v.x), y(v.y), z(v.z), w(v.w)
	{
	}
#endif

	template <typename T, qualifier Q>
	template <qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<4, T, P> const &v)
		: x(v.x), y(v.y), z(v.z), w(v.w)
	{
	}

	// -- Explicit basic constructors --

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(T scalar)
		: x(scalar), y(scalar), z(scalar), w(scalar)
	{
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(T _x, T _y, T _z, T _w)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	// -- Conversion scalar constructors --

	template <typename T, qualifier Q>
	template <typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, U, P> const &v)
		: x(static_cast<T>(v.x)), y(static_cast<T>(v.x)), z(static_cast<T>(v.x)), w(static_cast<T>(v.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(X _x, Y _y, Z _z, W _w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, Y _y, Z _z, W _w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(X _x, vec<1, Y, Q> const &_y, Z _z, W _w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_y, Z _z, W _w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(X _x, Y _y, vec<1, Z, Q> const &_z, W _w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, Y _y, vec<1, Z, Q> const &_z, W _w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(X _x, vec<1, Y, Q> const &_y, vec<1, Z, Q> const &_z, W _w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_y, vec<1, Z, Q> const &_z, W _w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, Y _y, Z _z, vec<1, W, Q> const &_w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(X _x, vec<1, Y, Q> const &_y, Z _z, vec<1, W, Q> const &_w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_y, Z _z, vec<1, W, Q> const &_w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(X _x, Y _y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, Y _y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(X _x, vec<1, Y, Q> const &_y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename X, typename Y, typename Z, typename W>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, X, Q> const &_x, vec<1, Y, Q> const &_y, vec<1, Z, Q> const &_z, vec<1, W, Q> const &_w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w.x))
	{
	}

	// -- Conversion vector constructors --

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<2, A, P> const &_xy, B _z, C _w)
		: x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_z)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<2, A, P> const &_xy, vec<1, B, P> const &_z, C _w)
		: x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<2, A, P> const &_xy, B _z, vec<1, C, P> const &_w)
		: x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_z)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<2, A, P> const &_xy, vec<1, B, P> const &_z, vec<1, C, P> const &_w)
		: x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_z.x)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(A _x, vec<2, B, P> const &_yz, C _w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_yz.x)), z(static_cast<T>(_yz.y)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, A, P> const &_x, vec<2, B, P> const &_yz, C _w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_yz.x)), z(static_cast<T>(_yz.y)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(A _x, vec<2, B, P> const &_yz, vec<1, C, P> const &_w)
		: x(static_cast<T>(_x)), y(static_cast<T>(_yz.x)), z(static_cast<T>(_yz.y)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, A, P> const &_x, vec<2, B, P> const &_yz, vec<1, C, P> const &_w)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_yz.x)), z(static_cast<T>(_yz.y)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(A _x, B _y, vec<2, C, P> const &_zw)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_zw.x)), w(static_cast<T>(_zw.y))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, A, P> const &_x, B _y, vec<2, C, P> const &_zw)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y)), z(static_cast<T>(_zw.x)), w(static_cast<T>(_zw.y))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(A _x, vec<1, B, P> const &_y, vec<2, C, P> const &_zw)
		: x(static_cast<T>(_x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_zw.x)), w(static_cast<T>(_zw.y))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, typename C, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, A, P> const &_x, vec<1, B, P> const &_y, vec<2, C, P> const &_zw)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_y.x)), z(static_cast<T>(_zw.x)), w(static_cast<T>(_zw.y))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<3, A, P> const &_xyz, B _w)
		: x(static_cast<T>(_xyz.x)), y(static_cast<T>(_xyz.y)), z(static_cast<T>(_xyz.z)), w(static_cast<T>(_w))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<3, A, P> const &_xyz, vec<1, B, P> const &_w)
		: x(static_cast<T>(_xyz.x)), y(static_cast<T>(_xyz.y)), z(static_cast<T>(_xyz.z)), w(static_cast<T>(_w.x))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(A _x, vec<3, B, P> const &_yzw)
		: x(static_cast<T>(_x)), y(static_cast<T>(_yzw.x)), z(static_cast<T>(_yzw.y)), w(static_cast<T>(_yzw.z))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<1, A, P> const &_x, vec<3, B, P> const &_yzw)
		: x(static_cast<T>(_x.x)), y(static_cast<T>(_yzw.x)), z(static_cast<T>(_yzw.y)), w(static_cast<T>(_yzw.z))
	{
	}

	template <typename T, qualifier Q>
	template <typename A, typename B, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<2, A, P> const &_xy, vec<2, B, P> const &_zw)
		: x(static_cast<T>(_xy.x)), y(static_cast<T>(_xy.y)), z(static_cast<T>(_zw.x)), w(static_cast<T>(_zw.y))
	{
	}

	template <typename T, qualifier Q>
	template <typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q>::vec(vec<4, U, P> const &v)
		: x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w))
	{
	}

	// -- Component accesses --

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T &vec<4, T, Q>::operator[](typename vec<4, T, Q>::length_type i)
	{
		assert(i >= 0 && i < this->length());
		switch (i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		}
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T const &vec<4, T, Q>::operator[](typename vec<4, T, Q>::length_type i) const
	{
		assert(i >= 0 && i < this->length());
		switch (i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		}
	}

	// -- Unary arithmetic operators --

#if GLM_CONFIG_DEFAULTED_FUNCTIONS == GLM_DISABLE
	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator=(vec<4, T, Q> const &v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;
		return *this;
	}
#endif

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator=(vec<4, U, Q> const &v)
	{
		this->x = static_cast<T>(v.x);
		this->y = static_cast<T>(v.y);
		this->z = static_cast<T>(v.z);
		this->w = static_cast<T>(v.w);
		return *this;
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator+=(U scalar)
	{
		return (*this = detail::compute_vec4_add<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator+=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_add<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v.x)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator+=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_add<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator-=(U scalar)
	{
		return (*this = detail::compute_vec4_sub<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator-=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_sub<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v.x)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator-=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_sub<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator*=(U scalar)
	{
		return (*this = detail::compute_vec4_mul<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator*=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_mul<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v.x)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator*=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_mul<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator/=(U scalar)
	{
		return (*this = detail::compute_vec4_div<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator/=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_div<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v.x)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator/=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_div<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	// -- Increment and decrement operators --

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator++()
	{
		++this->x;
		++this->y;
		++this->z;
		++this->w;
		return *this;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator--()
	{
		--this->x;
		--this->y;
		--this->z;
		--this->w;
		return *this;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> vec<4, T, Q>::operator++(int)
	{
		vec<4, T, Q> Result(*this);
		++*this;
		return Result;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> vec<4, T, Q>::operator--(int)
	{
		vec<4, T, Q> Result(*this);
		--*this;
		return Result;
	}

	// -- Unary bit operators --

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator%=(U scalar)
	{
		return (*this = detail::compute_vec4_mod<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator%=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_mod<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator%=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_mod<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator&=(U scalar)
	{
		return (*this = detail::compute_vec4_and<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator&=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_and<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator&=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_and<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator|=(U scalar)
	{
		return (*this = detail::compute_vec4_or<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator|=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_or<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator|=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_or<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator^=(U scalar)
	{
		return (*this = detail::compute_vec4_xor<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator^=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_xor<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator^=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_xor<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator<<=(U scalar)
	{
		return (*this = detail::compute_vec4_shift_left<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator<<=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_shift_left<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator<<=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_shift_left<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator>>=(U scalar)
	{
		return (*this = detail::compute_vec4_shift_right<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(scalar)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator>>=(vec<1, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_shift_right<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	template <typename T, qualifier Q>
	template <typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> &vec<4, T, Q>::operator>>=(vec<4, U, Q> const &v)
	{
		return (*this = detail::compute_vec4_shift_right<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<4, T, Q>(v)));
	}

	// -- Unary constant operators --

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v)
	{
		return v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(0) -= v;
	}

	// -- Binary arithmetic operators --

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v, T const &scalar)
	{
		return vec<4, T, Q>(v) += scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) += v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator+(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(v) += scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator+(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v2) += v1;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator+(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) += v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v, T const &scalar)
	{
		return vec<4, T, Q>(v) -= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) -= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator-(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) -= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator-(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1.x) -= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator-(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) -= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const &v, T const &scalar)
	{
		return vec<4, T, Q>(v) *= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) *= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator*(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(v) *= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator*(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v2) *= v1;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator*(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) *= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const &v, T const &scalar)
	{
		return vec<4, T, Q>(v) /= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) /= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator/(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) /= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator/(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1.x) /= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator/(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) /= v2;
	}

	// -- Binary bit operators --

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const &v, T scalar)
	{
		return vec<4, T, Q>(v) %= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) %= v2.x;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator%(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) %= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator%(vec<1, T, Q> const &scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar.x) %= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator%(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) %= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const &v, T scalar)
	{
		return vec<4, T, Q>(v) &= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const &v, vec<1, T, Q> const &scalar)
	{
		return vec<4, T, Q>(v) &= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator&(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) &= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator&(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1.x) &= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator&(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) &= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const &v, T scalar)
	{
		return vec<4, T, Q>(v) |= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) |= v2.x;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator|(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) |= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator|(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1.x) |= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator|(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) |= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const &v, T scalar)
	{
		return vec<4, T, Q>(v) ^= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) ^= v2.x;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator^(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) ^= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator^(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1.x) ^= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator^(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) ^= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const &v, T scalar)
	{
		return vec<4, T, Q>(v) <<= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) <<= v2.x;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator<<(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) <<= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1.x) <<= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator<<(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) <<= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const &v, T scalar)
	{
		return vec<4, T, Q>(v) >>= scalar;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const &v1, vec<1, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) >>= v2.x;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator>>(T scalar, vec<4, T, Q> const &v)
	{
		return vec<4, T, Q>(scalar) >>= v;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<1, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1.x) >>= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator>>(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return vec<4, T, Q>(v1) >>= v2;
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, T, Q> operator~(vec<4, T, Q> const &v)
	{
		return detail::compute_vec4_bitwise_not<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v);
	}

	// -- Boolean operators --

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool operator==(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return detail::compute_vec4_equal<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v1, v2);
	}

	template <typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool operator!=(vec<4, T, Q> const &v1, vec<4, T, Q> const &v2)
	{
		return detail::compute_vec4_nequal<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v1, v2);
	}

	template <qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, bool, Q> operator&&(vec<4, bool, Q> const &v1, vec<4, bool, Q> const &v2)
	{
		return vec<4, bool, Q>(v1.x && v2.x, v1.y && v2.y, v1.z && v2.z, v1.w && v2.w);
	}

	template <qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, bool, Q> operator||(vec<4, bool, Q> const &v1, vec<4, bool, Q> const &v2)
	{
		return vec<4, bool, Q>(v1.x || v2.x, v1.y || v2.y, v1.z || v2.z, v1.w || v2.w);
	}

	// -- Conversion functions --
	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xxx() const
	{
		return vec<3, T, Q>(x, x, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xxy() const
	{
		return vec<3, T, Q>(x, x, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xxz() const
	{
		return vec<3, T, Q>(x, x, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xxw() const
	{
		return vec<3, T, Q>(x, x, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xyx() const
	{
		return vec<3, T, Q>(x, y, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xyy() const
	{
		return vec<3, T, Q>(x, y, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xyz() const
	{
		return vec<3, T, Q>(x, y, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xyw() const
	{
		return vec<3, T, Q>(x, y, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xzx() const
	{
		return vec<3, T, Q>(x, z, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xzy() const
	{
		return vec<3, T, Q>(x, z, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xzz() const
	{
		return vec<3, T, Q>(x, z, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xzw() const
	{
		return vec<3, T, Q>(x, z, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xwx() const
	{
		return vec<3, T, Q>(x, w, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xwy() const
	{
		return vec<3, T, Q>(x, w, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xwz() const
	{
		return vec<3, T, Q>(x, w, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::xww() const
	{
		return vec<3, T, Q>(x, w, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yxx() const
	{
		return vec<3, T, Q>(y, x, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yxy() const
	{
		return vec<3, T, Q>(y, x, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yxz() const
	{
		return vec<3, T, Q>(y, x, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yxw() const
	{
		return vec<3, T, Q>(y, x, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yyx() const
	{
		return vec<3, T, Q>(y, y, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yyy() const
	{
		return vec<3, T, Q>(y, y, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yyz() const
	{
		return vec<3, T, Q>(y, y, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yyw() const
	{
		return vec<3, T, Q>(y, y, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yzx() const
	{
		return vec<3, T, Q>(y, z, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yzy() const
	{
		return vec<3, T, Q>(y, z, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yzz() const
	{
		return vec<3, T, Q>(y, z, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yzw() const
	{
		return vec<3, T, Q>(y, z, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::ywx() const
	{
		return vec<3, T, Q>(y, w, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::ywy() const
	{
		return vec<3, T, Q>(y, w, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::ywz() const
	{
		return vec<3, T, Q>(y, w, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::yww() const
	{
		return vec<3, T, Q>(y, w, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zxx() const
	{
		return vec<3, T, Q>(z, x, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zxy() const
	{
		return vec<3, T, Q>(z, x, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zxz() const
	{
		return vec<3, T, Q>(z, x, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zxw() const
	{
		return vec<3, T, Q>(z, x, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zyx() const
	{
		return vec<3, T, Q>(z, y, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zyy() const
	{
		return vec<3, T, Q>(z, y, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zyz() const
	{
		return vec<3, T, Q>(z, y, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zyw() const
	{
		return vec<3, T, Q>(z, y, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zzx() const
	{
		return vec<3, T, Q>(z, z, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zzy() const
	{
		return vec<3, T, Q>(z, z, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zzz() const
	{
		return vec<3, T, Q>(z, z, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zzw() const
	{
		return vec<3, T, Q>(z, z, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zwx() const
	{
		return vec<3, T, Q>(z, w, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zwy() const
	{
		return vec<3, T, Q>(z, w, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zwz() const
	{
		return vec<3, T, Q>(z, w, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::zww() const
	{
		return vec<3, T, Q>(z, w, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wxx() const
	{
		return vec<3, T, Q>(w, x, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wxy() const
	{
		return vec<3, T, Q>(w, x, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wxz() const
	{
		return vec<3, T, Q>(w, x, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wxw() const
	{
		return vec<3, T, Q>(w, x, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wyx() const
	{
		return vec<3, T, Q>(w, y, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wyy() const
	{
		return vec<3, T, Q>(w, y, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wyz() const
	{
		return vec<3, T, Q>(w, y, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wyw() const
	{
		return vec<3, T, Q>(w, y, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wzx() const
	{
		return vec<3, T, Q>(w, z, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wzy() const
	{
		return vec<3, T, Q>(w, z, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wzz() const
	{
		return vec<3, T, Q>(w, z, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wzw() const
	{
		return vec<3, T, Q>(w, z, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wwx() const
	{
		return vec<3, T, Q>(w, w, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wwy() const
	{
		return vec<3, T, Q>(w, w, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::wwz() const
	{
		return vec<3, T, Q>(w, w, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<3, T, Q> vec<4, T, Q>::www() const
	{
		return vec<3, T, Q>(w, w, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::xx() const
	{
		return vec<2, T, Q>(x, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::xy() const
	{
		return vec<2, T, Q>(x, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::xz() const
	{
		return vec<2, T, Q>(x, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::xw() const
	{
		return vec<2, T, Q>(x, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::yx() const
	{
		return vec<2, T, Q>(y, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::yy() const
	{
		return vec<2, T, Q>(y, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::yz() const
	{
		return vec<2, T, Q>(y, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::yw() const
	{
		return vec<2, T, Q>(y, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::zx() const
	{
		return vec<2, T, Q>(z, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::zy() const
	{
		return vec<2, T, Q>(z, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::zz() const
	{
		return vec<2, T, Q>(z, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::zw() const
	{
		return vec<2, T, Q>(z, w);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::wx() const
	{
		return vec<2, T, Q>(w, x);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::wy() const
	{
		return vec<2, T, Q>(w, y);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::wz() const
	{
		return vec<2, T, Q>(w, z);
	}

	template <typename T, qualifier Q>
	GLM_CONSTEXPR vec<2, T, Q> vec<4, T, Q>::ww() const
	{
		return vec<2, T, Q>(w, w);
	}

} // namespace glm

#if GLM_CONFIG_SIMD == GLM_ENABLE
#include "type_vec4_simd.inl"
#endif
