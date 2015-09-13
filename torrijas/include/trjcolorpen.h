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

#ifndef TRJ_COLOR_PEN_H
#define TRJ_COLOR_PEN_H

#include "trjpen.h"

namespace trj
{

class ColorPen : public Pen
{

protected:
    ColorPen(const ColorInfo& info, bool stroke, float strokeWidth) noexcept :
        Pen(info, stroke, strokeWidth)
    {
    }

public:
    ColorPen(const Color& color) noexcept :
        ColorPen(ColorInfo({color}), false, 0)
    {
    }

    ColorPen(float red, float green, float blue, float alpha = 1) noexcept :
        ColorPen(Color(red, green, blue, alpha))
    {
    }

    const Color& getColor() const noexcept
    {
        return mColorInfo.color;
    }
};


class StrokeColorPen : public ColorPen
{

public:
    StrokeColorPen(const Color& color, float strokeWidth = 1) noexcept :
        ColorPen(ColorInfo({color}), true, strokeWidth)
    {
    }

    StrokeColorPen(float red, float green, float blue, float alpha = 1,
            float strokeWidth = 1) noexcept :
        StrokeColorPen(Color(red, green, blue, alpha), strokeWidth)
    {
    }
};

}

#endif
