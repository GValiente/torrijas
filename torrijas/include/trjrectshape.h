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

#ifndef TRJ_RECT_SHAPE_H
#define TRJ_RECT_SHAPE_H

#include "trjshape.h"

namespace trj
{

class RectShape : public Shape
{

public:
    RectShape(const Rect& rect, float cornerRadius = 0) noexcept :
        Shape(RectInfo({rect, cornerRadius, isPositive(cornerRadius)}))
    {
    }

    RectShape(float positionX, float positionY, float width, float height,
            float cornerRadius = 0) noexcept :
        RectShape(Rect(positionX, positionY, width, height), cornerRadius)
    {
    }

    const Rect& getRect() const noexcept
    {
        return mRectInfo.rect;
    }

    float getCornerRadius() const noexcept
    {
        return mRectInfo.cornerRadius;
    }

    bool isRounded() const noexcept
    {
        return mRectInfo.rounded;
    }
};

}

#endif
