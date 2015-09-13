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

#ifndef TRJ_SHAPE_H
#define TRJ_SHAPE_H

#include <vector>
#include "trjrect.h"

struct NVGcontext;

namespace trj
{

class ShapeGroup;

class Shape
{
    friend class ShapeGroup;

protected:
    enum class Type
    {
        NONE,
        ARC,
        TRIANGLE,
        RECT,
        ELLIPSE,
        MOVE_TO,
        LINE_TO,
        BEZIER_TO,
        QUAD_TO,
        CLOSE_PATH
    };

    struct ArcInfo
    {
        Point position;
        float radius;
        float startAngle;
        float endAngle;
        bool clockWise;
    };
    
    struct TriangleInfo
    {
        Point firstVertex;
        Point secondVertex;
        Point thirdVertex;
    };

    struct RectInfo
    {
        Rect rect;
        float cornerRadius;
        bool rounded;
    };

    struct EllipseInfo
    {
        Point position;
        float horizontalRadius;
        float verticalRadius;
    };

    struct MoveToInfo
    {
        Point position;
    };

    struct LineToInfo
    {
        Point position;
    };

    struct BezierToInfo
    {
        Point controlPosition1;
        Point controlPosition2;
        Point position;
    };

    struct QuadToInfo
    {
        Point controlPosition;
        Point position;
    };

    struct ClosePathInfo
    {
    };

    union
    {
        ArcInfo mArcInfo;
        TriangleInfo mTriangleInfo;
        RectInfo mRectInfo;
        EllipseInfo mEllipseInfo;
        MoveToInfo mMoveToInfo;
        LineToInfo mLineToInfo;
        BezierToInfo mBezierToInfo;
        QuadToInfo mQuadToInfo;
        ClosePathInfo mClosePathInfo;
    };

    Type mType = Type::NONE;
    bool mHole = false;

    Shape(const ArcInfo& info) noexcept;
    
    Shape(const TriangleInfo& info) noexcept;

    Shape(const RectInfo& info) noexcept;

    Shape(const EllipseInfo& info) noexcept;

    Shape(const MoveToInfo& info) noexcept;

    Shape(const LineToInfo& info) noexcept;

    Shape(const BezierToInfo& info) noexcept;

    Shape(const QuadToInfo& info) noexcept;

    Shape(const ClosePathInfo& info) noexcept;

    void joinToBoundingBox(Rect& boundingBox, Point& lastPoint) const noexcept;

    void render(NVGcontext& nanoVgContext) const;

public:
    Shape() noexcept
    {
    }

    bool isHole() const noexcept
    {
        return mHole;
    }

    void setHole(bool hole) noexcept
    {
        mHole = hole;
    }

    void horizontalFlip() noexcept;

    Shape getHorizontalFlipped() const noexcept
    {
        Shape flipped = *this;
        flipped.horizontalFlip();

        return flipped;
    }

    void verticalFlip() noexcept;

    Shape getVerticalFlipped() const noexcept
    {
        Shape flipped = *this;
        flipped.verticalFlip();

        return flipped;
    }
};

}

#endif
