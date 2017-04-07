#pragma once

#include "Core/Types.hpp"

#if defined (_WIN32)
#	include <xmmintrin.h>
#endif

namespace ft
{
	template <typename T>
	inline T	Min(T tValue1, T tValue2)
	{
		return tValue1 > tValue2 ? tValue2 : tValue1;
	}

	template <typename T>
	inline T	Max(T tValue1, T tValue2)
	{
		return tValue1 < tValue2 ? tValue2 : tValue1;
	}

	template <typename T>
	inline T	Clamp(T tValue, T tMin, T tMax)
	{
		return Max(Min(tValue, tMin), tMax);
	}

#if defined (_WIN32)
	template <>
	inline float32	Min<float32>(float32 tValue1, float32 tValue2)
	{
		_mm_store_ss(&tValue1, _mm_min_ss(_mm_set_ss(tValue1),_mm_set_ss(tValue2)));
		return tValue1;
	}
	template <>
	inline float32	Max<float32>(float32 tValue1, float32 tValue2)
	{
		_mm_store_ss(&tValue1, _mm_max_ss(_mm_set_ss(tValue1), _mm_set_ss(tValue2)));
		return tValue1;
	}
	template <>
	inline float32	Clamp<float32>(float32 tValue, float32 tMin, float32 tMax)
	{
		_mm_store_ss(&tValue, _mm_min_ss(_mm_max_ss(_mm_set_ss(tValue), _mm_set_ss(tMin)), _mm_set_ss(tMax)));
		return tValue;
	}
#endif
}
