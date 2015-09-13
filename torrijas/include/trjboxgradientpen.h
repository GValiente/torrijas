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

#ifndef TRJ_BOX_GRADIENT_PEN_H
#define TRJ_BOX_GRADIENT_PEN_H

#include "trjpen.h"

namespace trj
{

class BoxGradientPen : public Pen
{

protected:
    BoxGradientPen(const BoxGradientInfo& info, bool stroke, float strokeWidth) noexcept :
        Pen(info, stroke, strokeWidth)
    {
    }

public:
    BoxGradientPen(const Rect& rect, float cornerRadius, float cornerBlur, const Color& innerColor,
            const Color& outerColor) noexcept :
        BoxGradientPen(BoxGradientInfo{rect, innerColor, outerColor, cornerRadius, cornerBlur}, false, 0)
    {
    }

    BoxGradientPen(float positionX, float positionY, float width, float height, float cornerRadius,
            float cornerBlur, const Color& innerColor, const Color& outerColor) noexcept :
        BoxGradientPen(Rect(positionX, positionY, width, height), cornerRadius, cornerBlur,
                innerColor, outerColor)
    {
    }

    const Rect& getRect() const noexcept
    {
        return mBoxGradientInfo.rect;
    }

    float getCornerRadius() const noexcept
    {
        return mBoxGradientInfo.cornerRadius;
    }

    float getCornerBlur() const noexcept
    {
        return mBoxGradientInfo.cornerBlur;
    }

    const Color& getInnerColor() const noexcept
    {
        return mBoxGradientInfo.innerColor;
    }

    const Color& getOuterColor() const noexcept
    {
        return mBoxGradientInfo.outerColor;
    }
};


class StrokeBoxGradientPen : public BoxGradientPen
{

public:
    StrokeBoxGradientPen(const Rect& rect, float cornerRadius, float cornerBlur,
            const Color& innerColor, const Color& outerColor, float strokeWidth = 1) noexcept :
        BoxGradientPen(BoxGradientInfo{rect, innerColor, outerColor, cornerRadius, cornerBlur},
                true, strokeWidth)
    {
    }

    StrokeBoxGradientPen(float positionX, float positionY, float width, float height,
            float cornerRadius, float cornerBlur, const Color& innerColor,
            const Color& outerColor, float strokeWidth = 1) noexcept :
        StrokeBoxGradientPen(Rect(positionX, positionY, width, height), cornerRadius, cornerBlur,
                innerColor, outerColor, strokeWidth)
    {
    }
};

}

#endif
