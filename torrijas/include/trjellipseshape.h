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

#ifndef TRJ_ELLIPSE_SHAPE_H
#define TRJ_ELLIPSE_SHAPE_H

#include "trjshape.h"

namespace trj
{

class EllipseShape : public Shape
{

public:
    EllipseShape(const Point& position, float horizontalRadius, float verticalRadius) noexcept :
        Shape(EllipseInfo({position, horizontalRadius, verticalRadius}))
    {
    }

    EllipseShape(float x, float y, float horizontalRadius, float verticalRadius) noexcept :
        EllipseShape(Point(x, y), horizontalRadius, verticalRadius)
    {
    }

    EllipseShape(const Point& position, float radius) noexcept :
        EllipseShape(position, radius, radius)
    {
    }

    EllipseShape(float x, float y, float radius) noexcept :
        EllipseShape(Point(x, y), radius, radius)
    {
    }

    const Point& getPosition() const noexcept
    {
        return mEllipseInfo.position;
    }

    float getHorizontalRadius() const noexcept
    {
        return mEllipseInfo.horizontalRadius;
    }

    float getVerticalRadius() const noexcept
    {
        return mEllipseInfo.verticalRadius;
    }
};

}

#endif
