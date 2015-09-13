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

#ifndef TRJ_ARC_SHAPE_H
#define TRJ_ARC_SHAPE_H

#include "trjshape.h"

namespace trj
{

class ArcShape : public Shape
{

public:
    ArcShape(const Point& position, float radius, float startAngle, float endAngle,
            bool clockWise = true) noexcept :
        Shape(ArcInfo({position, radius, startAngle, endAngle, clockWise}))
    {
    }

    ArcShape(float positionX, float positionY, float radius, float startAngle,
            float endAngle, bool clockWise = true) noexcept :
        ArcShape(Point(positionX, positionY), radius, startAngle, endAngle, clockWise)
    {
    }

    const Point& getPosition() const noexcept
    {
        return mArcInfo.position;
    }

    float getRadius() const noexcept
    {
        return mArcInfo.radius;
    }

    float getStartAngle() const noexcept
    {
        return mArcInfo.startAngle;
    }

    float getEndAngle() const noexcept
    {
        return mArcInfo.endAngle;
    }

    bool isClockWise() const noexcept
    {
        return mArcInfo.clockWise;
    }
};

}

#endif
