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

#ifndef TRJ_RADIAL_GRADIENT_PEN_H
#define TRJ_RADIAL_GRADIENT_PEN_H

#include "trjpen.h"

namespace trj
{

class RadialGradientPen : public Pen
{

protected:
    RadialGradientPen(const RadialGradientInfo& info, bool stroke, float strokeWidth) noexcept :
        Pen(info, stroke, strokeWidth)
    {
    }

public:
    RadialGradientPen(const Point& position, float innerRadius, float outerRadius,
            const Color& innerColor, const Color& outerColor) noexcept :
        RadialGradientPen(RadialGradientInfo{position, innerColor, outerColor, innerRadius,
                outerRadius}, false, 0)
    {
    }

    RadialGradientPen(float positionX, float positionY, float innerRadius,
            float outerRadius, const Color& innerColor, const Color& outerColor) noexcept :
        RadialGradientPen(Point(positionX, positionY), innerRadius, outerRadius, innerColor,
                outerColor)
    {
    }

    const Point& getPosition() const noexcept
    {
        return mRadialGradientInfo.position;
    }

    float getInnerRadius() const noexcept
    {
        return mRadialGradientInfo.innerRadius;
    }

    float getOuterRadius() const noexcept
    {
        return mRadialGradientInfo.outerRadius;
    }

    const Color& getInnerColor() const noexcept
    {
        return mRadialGradientInfo.innerColor;
    }

    const Color& getOuterColor() const noexcept
    {
        return mRadialGradientInfo.outerColor;
    }
};


class StrokeRadialGradientPen : public RadialGradientPen
{

public:
    StrokeRadialGradientPen(const Point& position, float innerRadius, float outerRadius,
            const Color& innerColor, const Color& outerColor, float strokeWidth = 1) noexcept :
        RadialGradientPen(RadialGradientInfo{position, innerColor, outerColor, innerRadius,
                outerRadius}, true, strokeWidth)
    {
    }

    StrokeRadialGradientPen(float positionX, float positionY, float innerRadius,
            float outerRadius, const Color& innerColor, const Color& outerColor,
            float strokeWidth = 1) noexcept :
        StrokeRadialGradientPen(Point(positionX, positionY), innerRadius, outerRadius, innerColor,
                outerColor, strokeWidth)
    {
    }
};

}

#endif
