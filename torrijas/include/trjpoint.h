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

#ifndef TRJ_POINT_H
#define TRJ_POINT_H

#include <array>
#include "trjcommon.h"

namespace trj
{

class Point
{

protected:
    float mX;
    float mY;

public:
    constexpr Point(float x, float y) noexcept :
        mX(x),
        mY(y)
    {
    }

    constexpr Point() noexcept :
        mX(0),
        mY(0)
    {
    }

    constexpr float getX() const noexcept
    {
        return mX;
    }

    void setX(float x) noexcept
    {
        mX = x;
    }

    constexpr float getY() const noexcept
    {
        return mY;
    }

    void setY(float y) noexcept
    {
        mY = y;
    }

    void transform(const std::array<float, 6>& transformationMatrix);

    Point getTransformed(const std::array<float, 6>& transformationMatrix) const;

    bool operator==(const Point& other) const
    {
        return areEquals(mX, other.mX) && areEquals(mY, other.mY);
    }

    bool operator!=(const Point& other) const
    {
        return ! (operator==(other));
    }
};

}

#endif
