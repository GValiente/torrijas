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

#include "trjsize.h"

#include "trjdebug.h"

namespace trj
{

void Size::checkEmpty() noexcept
{
    if(isZero(mWidth) || isZero(mHeight))
    {
        mWidth = 0;
        mHeight = 0;
    }
}

Size::Size(float width, float height) noexcept :
    mWidth(width),
    mHeight(height)
{
    TRJ_ASSERT(! isNegative(width), "Invalid width");
    TRJ_ASSERT(! isNegative(height), "Invalid height");

    checkEmpty();
}

void Size::setWidth(float width) noexcept
{
    TRJ_ASSERT(! isNegative(width), "Invalid width");

    mWidth = width;
    checkEmpty();
}

void Size::setHeight(float height) noexcept
{
    TRJ_ASSERT(! isNegative(height), "Invalid height");

    mHeight = height;
    checkEmpty();
}

void Size::scale(float scaleFactor) noexcept
{
    TRJ_ASSERT(isPositive(scaleFactor), "Invalid scale factor");

    if(! isEmpty())
    {
        mWidth *= scaleFactor;
        mHeight *= scaleFactor;
    }
}

Size Size::getScaled(float scaleFactor) const noexcept
{
    Size scaled = *this;
    scaled.scale(scaleFactor);

    return scaled;
}

Size operator*(const Size& size, float scaleFactor)
{
    return size.getScaled(scaleFactor);
}

Size operator*(float scaleFactor, const Size& size)
{
    return size.getScaled(scaleFactor);
}

}
