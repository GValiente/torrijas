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

#ifndef TRJ_RECT_H
#define TRJ_RECT_H

#include "trjpoint.h"
#include "trjsize.h"

namespace trj
{

class Rect
{

protected:
    Point mPosition;
    Size mSize;

public:
    Rect() noexcept :
        Rect(Point(), Size())
    {
    }

    Rect(const Point& position, const Size& size) noexcept :
        mPosition(position),
        mSize(size)
    {
    }

    Rect(const Point& position, float width, float height) noexcept :
        Rect(position, Size(width, height))
    {
    }

    Rect(float x, float y, const Size& size) noexcept :
        Rect(Point(x, y), size)
    {
    }

    Rect(float x, float y, float width, float height) noexcept :
        Rect(Point(x, y), Size(width, height))
    {
    }

    Rect(const Point& position1, const Point& position2) noexcept;

    Rect(const Point& position1, const Point& position2, const Point& position3) noexcept;

    Rect(const Point& position1, const Point& position2, const Point& position3,
            const Point& position4) noexcept;

    const Point& getPosition() const noexcept
    {
        return mPosition;
    }

    void setPosition(const Point& position) noexcept
    {
        mPosition = position;
    }

    void setPosition(float x, float y) noexcept
    {
        setPosition(Point(x, y));
    }

    const Point& getTopLeft() const noexcept
    {
        return mPosition;
    }

    Point getTopRight() const noexcept
    {
        return Point(getX() + getWidth(), getY());
    }

    Point getBottomLeft() const noexcept
    {
        return Point(getX(), getY() + getHeight());
    }

    Point getBottomRight() const noexcept
    {
        return Point(getX() + getWidth(), getY() + getHeight());
    }

    Point getCenter() const noexcept
    {
        return Point(getX() + (getWidth() * 0.5f), getY() + (getHeight() * 0.5f));
    }

    void setCenter(const Point& center) noexcept
    {
        setPosition(center.getX() - (getWidth() * 0.5f), center.getY() - (getHeight() * 0.5f));
    }

    void setCenter(float x, float y) noexcept
    {
        setCenter(Point(x, y));
    }

    float getX() const noexcept
    {
        return mPosition.getX();
    }

    void setX(float x) noexcept
    {
        mPosition.setX(x);
    }

    float getY() const noexcept
    {
        return mPosition.getY();
    }

    void setY(float y) noexcept
    {
        mPosition.setY(y);
    }

    const Size& getSize() const noexcept
    {
        return mSize;
    }

    void setSize(const Size& size) noexcept
    {
        mSize = size;
    }

    float getWidth() const noexcept
    {
        return mSize.getWidth();
    }

    void setWidth(float width) noexcept
    {
        mSize.setWidth(width);
    }

    float getHeight() const noexcept
    {
        return mSize.getHeight();
    }

    void setHeight(float height) noexcept
    {
        mSize.setHeight(height);
    }

    bool isEmpty() const noexcept
    {
        return mSize.isEmpty();
    }

    bool isIntersecting(const Rect& other) const noexcept;

    void scale(float scaleFactor) noexcept;

    Rect getScaled(float scaleFactor) const noexcept;

    void join(const Rect& other) noexcept;

    Rect getJoined(const Rect& other) const noexcept;

    void intersect(const Rect& other) noexcept;

    Rect getIntersected(const Rect& other) const noexcept;

    void transform(const std::array<float, 6>& transformationMatrix)
    {
        *this = getTransformed(transformationMatrix);
    }

    Rect getTransformed(const std::array<float, 6>& transformationMatrix) const;

    Rect& operator*=(float scaleFactor) noexcept
    {
        scale(scaleFactor);
        return *this;
    }

    Rect& operator&=(const Rect& other) noexcept
    {
        intersect(other);
        return *this;
    }

    Rect& operator|=(const Rect& other) noexcept
    {
        join(other);
        return *this;
    }

    bool operator==(const Rect& other) noexcept
    {
        return mPosition == other.mPosition && mSize == other.mSize;
    }

    bool operator!=(const Rect& other) noexcept
    {
        return ! (*this == other);
    }
};

Rect operator*(const Rect& rect, float scaleFactor);

Rect operator*(float scaleFactor, const Rect& rect);

Rect operator&(const Rect& a, const Rect& b) noexcept;

Rect operator|(const Rect& a, const Rect& b) noexcept;

}

#endif
