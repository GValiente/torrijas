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

#ifndef TRJ_LINEAR_GRADIENT_PEN_H
#define TRJ_LINEAR_GRADIENT_PEN_H

#include "trjpen.h"

namespace trj
{

class LinearGradientPen : public Pen
{

protected:
    LinearGradientPen(const LinearGradientInfo& info, bool stroke, float strokeWidth) noexcept :
        Pen(info, stroke, strokeWidth)
    {
    }

public:
    LinearGradientPen(const Point& startPosition, const Point& endPosition,
            const Color& innerColor, const Color& outerColor) noexcept :
        LinearGradientPen(LinearGradientInfo{startPosition, endPosition, innerColor, outerColor},
                false, 0)
    {
    }

    const Point& getStartPosition() const noexcept
    {
        return mLinearGradientInfo.startPosition;
    }

    const Point& getEndPosition() const noexcept
    {
        return mLinearGradientInfo.endPosition;
    }

    const Color& getInnerColor() const noexcept
    {
        return mLinearGradientInfo.innerColor;
    }

    const Color& getOuterColor() const noexcept
    {
        return mLinearGradientInfo.outerColor;
    }
};


class StrokeLinearGradientPen : public LinearGradientPen
{

public:
    StrokeLinearGradientPen(const Point& startPosition, const Point& endPosition,
            const Color& innerColor, const Color& outerColor, float strokeWidth = 1) noexcept :
        LinearGradientPen(LinearGradientInfo{startPosition, endPosition, innerColor, outerColor},
                true, strokeWidth)
    {
    }

    StrokeLinearGradientPen(float startPositionX, float startPositionY,
            float endPositionX, float endPositionY, const Color& innerColor,
            const Color& outerColor, float strokeWidth = 1) noexcept :
        StrokeLinearGradientPen(Point(startPositionX, startPositionY),
                Point(endPositionX, endPositionY), innerColor, outerColor, strokeWidth)
    {
    }
};

}

#endif
