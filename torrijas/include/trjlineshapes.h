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

#ifndef TRJ_LINE_SHAPES_H
#define TRJ_LINE_SHAPES_H

#include "trjshape.h"

namespace trj
{

class MoveToShape : public Shape
{

public:
    explicit MoveToShape(const Point& position) noexcept :
        Shape(MoveToInfo({position}))
    {
    }

    explicit MoveToShape(float positionX, float positionY) noexcept :
        MoveToShape(Point(positionX, positionY))
    {
    }

    const Point& getPosition() const noexcept
    {
        return mMoveToInfo.position;
    }
};


class LineToShape : public Shape
{

public:
    explicit LineToShape(const Point& position) noexcept :
        Shape(LineToInfo({position}))
    {
    }

    explicit LineToShape(float positionX, float positionY) noexcept :
        LineToShape(Point(positionX, positionY))
    {
    }

    const Point& getPosition() const noexcept
    {
        return mLineToInfo.position;
    }
};


class BezierToShape : public Shape
{

public:
    BezierToShape(const Point& controlPosition1, const Point& controlPosition2,
            const Point& position) noexcept :
        Shape(BezierToInfo({controlPosition1, controlPosition2, position}))
    {
    }

    const Point& getControlPosition1() const noexcept
    {
        return mBezierToInfo.controlPosition1;
    }

    const Point& getControlPosition2() const noexcept
    {
        return mBezierToInfo.controlPosition2;
    }

    const Point& getPosition() const noexcept
    {
        return mBezierToInfo.position;
    }
};


class QuadToShape : public Shape
{

public:
    QuadToShape(const Point& controlPosition, const Point& position) noexcept :
        Shape(QuadToInfo({controlPosition, position}))
    {
    }

    const Point& getControlPosition() const noexcept
    {
        return mQuadToInfo.controlPosition;
    }

    const Point& getPosition() const noexcept
    {
        return mQuadToInfo.position;
    }
};


class ClosePathShape : public Shape
{

public:
    ClosePathShape() noexcept :
        Shape(ClosePathInfo())
    {
    }
};

}

#endif
