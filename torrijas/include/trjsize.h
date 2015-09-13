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

#ifndef TRJ_SIZE_H
#define TRJ_SIZE_H

#include "trjcommon.h"

namespace trj
{

class Size
{

protected:
    float mWidth;
    float mHeight;

    void checkEmpty() noexcept;

public:
    Size() noexcept :
        mWidth(0),
        mHeight(0)
    {
    }

    Size(float width, float height) noexcept;

    float getWidth() const noexcept
    {
        return mWidth;
    }

    void setWidth(float width) noexcept;

    float getHeight() const noexcept
    {
        return mHeight;
    }

    void setHeight(float height) noexcept;

    bool isEmpty() const noexcept
    {
        return mWidth == 0;
    }

    void scale(float scaleFactor) noexcept;

    Size getScaled(float scaleFactor) const noexcept;

    Size& operator*=(float scaleFactor) noexcept
    {
        scale(scaleFactor);
        return *this;
    }

    bool operator==(const Size& other) noexcept
    {
        return areEquals(mWidth, other.mWidth) && areEquals(mHeight, other.mHeight);
    }

    bool operator!=(const Size& other) noexcept
    {
        return ! (*this == other);
    }
};

Size operator*(const Size& size, float scaleFactor);

Size operator*(float scaleFactor, const Size& size);

}

#endif
