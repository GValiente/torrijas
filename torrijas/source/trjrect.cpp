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

#include "trjrect.h"

#include <algorithm>
#include "trjdebug.h"

namespace trj
{

namespace
{
    void minMax(float value, float& min, float& max)
    {
        if(value < min)
        {
            min = value;
        }
        else if(value > max)
        {
            max = value;
        }
    }
}

Rect::Rect(const Point& position1, const Point& position2) noexcept
{
    float minX = position1.getX();
    float maxX = minX;
    float minY = position1.getY();
    float maxY = minY;

    minMax(position2.getX(), minX, maxX);
    minMax(position2.getY(), minY, maxY);

    mPosition = Point(minX, minY);
    mSize = Size(maxX - minX, maxY - minY);
}

Rect::Rect(const Point& position1, const Point& position2, const Point& position3) noexcept
{
    float minX = position1.getX();
    float maxX = minX;
    float minY = position1.getY();
    float maxY = minY;

    minMax(position2.getX(), minX, maxX);
    minMax(position2.getY(), minY, maxY);

    minMax(position3.getX(), minX, maxX);
    minMax(position3.getY(), minY, maxY);

    mPosition = Point(minX, minY);
    mSize = Size(maxX - minX, maxY - minY);
}

Rect::Rect(const Point& position1, const Point& position2, const Point& position3,
        const Point& position4) noexcept
{
    float minX = position1.getX();
    float maxX = minX;
    float minY = position1.getY();
    float maxY = minY;

    minMax(position2.getX(), minX, maxX);
    minMax(position2.getY(), minY, maxY);

    minMax(position3.getX(), minX, maxX);
    minMax(position3.getY(), minY, maxY);

    minMax(position4.getX(), minX, maxX);
    minMax(position4.getY(), minY, maxY);

    mPosition = Point(minX, minY);
    mSize = Size(maxX - minX, maxY - minY);
}

bool Rect::isIntersecting(const Rect& other) const noexcept
{
    if(isEmpty())
    {
        return false;
    }

    float x = std::max(getX(), other.getX());
    float width = std::min(getX() + getWidth(), other.getX() + other.getWidth()) - x;
    if(! isPositive(width))
    {
        return false;
    }

    float y = std::max(getY(), other.getY());
    float height = std::min(getY() + getHeight(), other.getY() + other.getHeight()) - y;
    return isPositive(height);
}

void Rect::scale(float scaleFactor) noexcept
{
    TRJ_ASSERT(isPositive(scaleFactor), "Invalid scale factor");

    Size oldSize = mSize;
    mSize.scale(scaleFactor);

    float widthDelta = mSize.getWidth() - oldSize.getWidth();
    float heightDelta = mSize.getHeight() - oldSize.getHeight();
    mPosition = Point(getX() - (widthDelta * 0.5f), getY() - (heightDelta * 0.5f));
}

Rect Rect::getScaled(float scaleFactor) const noexcept
{
    Rect scaled = *this;
    scaled.scale(scaleFactor);

    return scaled;
}

void Rect::join(const Rect& other) noexcept
{
    if(isEmpty())
    {
        *this = other;
    }
    else
    {
        float x = std::min(getX(), other.getX());
        float y = std::min(getY(), other.getY());
        float width = std::max(getX() + getWidth(), other.getX() + other.getWidth()) - x;
        float height = std::max(getY() + getHeight(), other.getY() + other.getHeight()) - y;
        mPosition = Point(x, y);
        mSize = Size(width, height);
    }
}

Rect Rect::getJoined(const Rect& other) const noexcept
{
    Rect joined = *this;
    joined.join(other);

    return joined;
}

void Rect::intersect(const Rect& other) noexcept
{
    if(isEmpty())
    {
        return;
    }

    float x = std::max(getX(), other.getX());
    float width = std::min(getX() + getWidth(), other.getX() + other.getWidth()) - x;
    if(isPositive(width))
    {
        float y = std::max(getY(), other.getY());
        float height = std::min(getY() + getHeight(), other.getY() + other.getHeight()) - y;
        if(isPositive(height))
        {
            mPosition = Point(x, y);
            mSize = Size(width, height);
        }
        else
        {
            mPosition = Point();
            mSize = Size();
        }
    }
    else
    {
        mPosition = Point();
        mSize = Size();
    }
}

Rect Rect::getIntersected(const Rect& other) const noexcept
{
    Rect intersected = *this;
    intersected.intersect(other);

    return intersected;
}

Rect Rect::getTransformed(const std::array<float, 6>& transformationMatrix) const
{
    Point topLeftPosition = getTopLeft().getTransformed(transformationMatrix);
    Point topRightPosition = getTopRight().getTransformed(transformationMatrix);
    Point bottomLeftPosition = getBottomLeft().getTransformed(transformationMatrix);
    Point bottomRightPosition = getBottomRight().getTransformed(transformationMatrix);

    return Rect(topLeftPosition, topRightPosition, bottomLeftPosition, bottomRightPosition);
}

Rect operator*(const Rect& rect, float scaleFactor)
{
    return rect.getScaled(scaleFactor);
}

Rect operator*(float scaleFactor, const Rect& rect)
{
    return rect.getScaled(scaleFactor);
}

Rect operator&(const Rect& a, const Rect& b) noexcept
{
    return a.getIntersected(b);
}

Rect operator|(const Rect& a, const Rect& b) noexcept
{
    return a.getJoined(b);
}

}
