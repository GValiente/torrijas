//
// Copyright (c) 2015 Gustavo Valiente gustavovalient@gmail.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef TRJ_COMMON_H
#define TRJ_COMMON_H

#include "trjconfig.h"

namespace trj
{
    template<typename Type>
    constexpr bool isPositiveZero(Type value) noexcept
    {
        return value < kEpsilon;
    }

    template<typename Type>
    constexpr bool isNegativeZero(Type value) noexcept
    {
        return -value < kEpsilon;
    }

    template<typename Type>
    constexpr bool isZero(Type value) noexcept
    {
        return isPositiveZero(value) && isNegativeZero(value);
    }

    template<typename Type>
    constexpr bool isPositive(Type value) noexcept
    {
        return value >= kEpsilon;
    }

    template<typename Type>
    constexpr bool isNegative(Type value) noexcept
    {
        return value <= -kEpsilon;
    }

    template<typename Type1, typename Type2>
    constexpr bool areEquals(Type1 a, Type2 b) noexcept
    {
        return isZero(b - a);
    }

    constexpr float kPi = 3.14159265f;

    constexpr float toDegrees(float radians) noexcept
    {
        return (radians * 180.0f) / kPi;
    }

    constexpr float toRadians(float degrees) noexcept
    {
        return (degrees * kPi) / 180.0f;
    }
}

#endif
