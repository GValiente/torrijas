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

#include "trjshape.h"

#include "nanovg.h"
#include "trjpen.h"
#include "trjdebug.h"

namespace trj
{

Shape::Shape(const ArcInfo& info) noexcept :
    mArcInfo(info),
    mType(Type::ARC)
{
    TRJ_ASSERT(isPositive(info.radius), "Invalid radius");
    TRJ_ASSERT(info.startAngle >= 0 && info.startAngle < 2 * kPi, "Invalid start angle");
    TRJ_ASSERT(info.endAngle >= 0 && info.endAngle < 2 * kPi, "Invalid start angle");
}

Shape::Shape(const TriangleInfo& info) noexcept :
    mTriangleInfo(info),
    mType(Type::TRIANGLE)
{
    TRJ_ASSERT(info.firstVertex != info.secondVertex, "First two vertices are equals");
    TRJ_ASSERT(info.secondVertex != info.thirdVertex, "Last two vertices are equals");
}

Shape::Shape(const RectInfo& info) noexcept :
    mRectInfo(info),
    mType(Type::RECT)
{
    TRJ_ASSERT(! info.rect.isEmpty(), "Rect is empty");
    TRJ_ASSERT(! isNegative(info.cornerRadius), "Invalid corner radius");
}

Shape::Shape(const EllipseInfo& info) noexcept :
    mEllipseInfo(info),
    mType(Type::ELLIPSE)
{
    TRJ_ASSERT(isPositive(info.horizontalRadius), "Invalid horizontal radius");
    TRJ_ASSERT(isPositive(info.verticalRadius), "Invalid vertical radius");
}

Shape::Shape(const MoveToInfo& info) noexcept :
    mMoveToInfo(info),
    mType(Type::MOVE_TO)
{
}

Shape::Shape(const LineToInfo& info) noexcept :
    mLineToInfo(info),
    mType(Type::LINE_TO)
{
}

Shape::Shape(const BezierToInfo& info) noexcept :
    mBezierToInfo(info),
    mType(Type::BEZIER_TO)
{
}

Shape::Shape(const QuadToInfo& info) noexcept :
    mQuadToInfo(info),
    mType(Type::QUAD_TO)
{
}

Shape::Shape(const ClosePathInfo& info) noexcept :
    mClosePathInfo(info),
    mType(Type::CLOSE_PATH)
{
}

void Shape::joinToBoundingBox(Rect& boundingBox, Point& lastPoint) const noexcept
{
    switch(mType)
    {
        case Type::ARC:
        {
            const ArcInfo& info = mArcInfo;
            const Point& position = info.position;
            float radius = info.radius;
            Rect shapeBoundingBox(position.getX() - radius, position.getY() - radius,
                    radius * 2, radius * 2);
            boundingBox.join(shapeBoundingBox);
            lastPoint = position;
        }
        break;

        case Type::TRIANGLE:
        {
            const TriangleInfo& info = mTriangleInfo;
            boundingBox.join(Rect(info.firstVertex, info.secondVertex, info.thirdVertex));
            lastPoint = info.thirdVertex;
        }
        break;

        case Type::RECT:
        {
            const RectInfo& info = mRectInfo;
            boundingBox.join(info.rect);

            if(info.rounded)
            {
                lastPoint = info.rect.getBottomLeft();
            }
            else
            {
                lastPoint = info.rect.getTopRight();
            }
        }
        break;

        case Type::ELLIPSE:
        {
            const EllipseInfo& info = mEllipseInfo;
            const Point& position = info.position;
            float horizontalRadius = info.horizontalRadius;
            float verticalRadius = info.verticalRadius;
            Rect shapeBoundingBox(position.getX() - horizontalRadius,
                    position.getY() - verticalRadius, horizontalRadius * 2, verticalRadius * 2);
            boundingBox.join(shapeBoundingBox);
            lastPoint = shapeBoundingBox.getTopLeft();
        }
        break;

        case Type::MOVE_TO:
        {
            const MoveToInfo& info = mMoveToInfo;
            lastPoint = info.position;
        }
        break;

        case Type::LINE_TO:
        {
            const LineToInfo& info = mLineToInfo;
            boundingBox.join(Rect(lastPoint, info.position));
            lastPoint = info.position;
        }
        break;

        case Type::BEZIER_TO:
        {
            const BezierToInfo& info = mBezierToInfo;
            boundingBox.join(Rect(lastPoint, info.controlPosition1, info.controlPosition2,
                    info.position));
            lastPoint = info.position;
        }
        break;

        case Type::QUAD_TO:
        {
            const QuadToInfo& info = mQuadToInfo;
            float x0 = lastPoint.getX();
            float y0 = lastPoint.getY();
            float cx = info.controlPosition.getX();
            float cy = info.controlPosition.getY();
            float x = info.position.getX();
            float y = info.position.getY();
            Point controlPosition1(x0 + 2.0f / 3.0f * (cx - x0), y0 + 2.0f / 3.0f * (cy - y0));
            Point controlPosition2(x + 2.0f / 3.0f * (cx - x), y + 2.0f / 3.0f * (cy - y));
            boundingBox.join(Rect(lastPoint, controlPosition1, controlPosition2, info.position));
            lastPoint = info.position;
        }
        break;

        default:
            break;
    }
}

void Shape::render(NVGcontext& nanoVgContext) const
{
    switch(mType)
    {
        case Type::ARC:
        {
            const ArcInfo& info = mArcInfo;
            const Point& position = info.position;
            nvgArc(&nanoVgContext, position.getX(), position.getY(), info.radius, info.startAngle,
                    info.endAngle, info.clockWise ? NVG_CW : NVG_CCW);
        }
        break;

        case Type::TRIANGLE:
        {
            const TriangleInfo& info = mTriangleInfo;
            nvgMoveTo(&nanoVgContext, info.firstVertex.getX(), info.firstVertex.getY());
            nvgLineTo(&nanoVgContext, info.secondVertex.getX(), info.secondVertex.getY());
            nvgLineTo(&nanoVgContext, info.thirdVertex.getX(), info.thirdVertex.getY());
            nvgClosePath(&nanoVgContext);
        }
        break;

        case Type::RECT:
        {
            const RectInfo& info = mRectInfo;
            const Rect& rect = info.rect;

            if(info.rounded)
            {
                nvgRoundedRect(&nanoVgContext, rect.getX(), rect.getY(), rect.getWidth(),
                        rect.getHeight(), info.cornerRadius);
            }
            else
            {
                nvgRect(&nanoVgContext, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
            }
        }
        break;

        case Type::ELLIPSE:
        {
            const EllipseInfo& info = mEllipseInfo;
            const Point& position = info.position;
            nvgEllipse(&nanoVgContext, position.getX(), position.getY(), info.horizontalRadius,
                    info.verticalRadius);
        }
        break;

        case Type::MOVE_TO:
        {
            const MoveToInfo& info = mMoveToInfo;
            const Point& position = info.position;
            nvgMoveTo(&nanoVgContext, position.getX(), position.getY());
        }
        break;

        case Type::LINE_TO:
        {
            const LineToInfo& info = mLineToInfo;
            const Point& position = info.position;
            nvgLineTo(&nanoVgContext, position.getX(), position.getY());
        }
        break;

        case Type::BEZIER_TO:
        {
            const BezierToInfo& info = mBezierToInfo;
            const Point& control1 = info.controlPosition1;
            const Point& control2 = info.controlPosition2;
            const Point& position = info.position;
            nvgBezierTo(&nanoVgContext, control1.getX(), control1.getY(), control2.getX(),
                    control2.getY(), position.getX(), position.getY());
        }
        break;

        case Type::QUAD_TO:
        {
            const QuadToInfo& info = mQuadToInfo;
            const Point& control = info.controlPosition;
            const Point& position = info.position;
            nvgQuadTo(&nanoVgContext, control.getX(), control.getY(), position.getX(), position.getY());
        }
        break;

        case Type::CLOSE_PATH:
        {
            nvgClosePath(&nanoVgContext);
            return;
        }
        break;

        default:
            return;
    }
}

void Shape::horizontalFlip() noexcept
{
    switch(mType)
    {
        case Type::ARC:
        {
            ArcInfo& info = mArcInfo;
            info.position.setX(-info.position.getX());
            info.clockWise = ! info.clockWise;

            if(info.startAngle > kPi)
            {
                info.startAngle = (2 * kPi) - (info.startAngle - kPi);
            }
            else
            {
                info.startAngle = kPi - info.startAngle;
            }

            if(info.endAngle > kPi)
            {
                info.endAngle = (2 * kPi) - (info.endAngle - kPi);
            }
            else
            {
                info.endAngle = kPi - info.endAngle;
            }
        }
        break;

        case Type::TRIANGLE:
        {
            TriangleInfo& info = mTriangleInfo;
            info.firstVertex.setX(-info.firstVertex.getX());
            info.secondVertex.setX(-info.secondVertex.getX());
            info.thirdVertex.setX(-info.thirdVertex.getX());
        }
        break;

        case Type::RECT:
        {
            RectInfo& info = mRectInfo;
            Point center = info.rect.getCenter();
            info.rect.setCenter(-center.getX(), center.getY());
        }
        break;

        case Type::ELLIPSE:
        {
            EllipseInfo& info = mEllipseInfo;
            info.position.setX(-info.position.getX());
        }
        break;

        case Type::MOVE_TO:
        {
            MoveToInfo& info = mMoveToInfo;
            info.position.setX(-info.position.getX());
        }
        break;

        case Type::LINE_TO:
        {
            LineToInfo& info = mLineToInfo;
            info.position.setX(-info.position.getX());
        }
        break;

        case Type::BEZIER_TO:
        {
            BezierToInfo& info = mBezierToInfo;
            info.controlPosition1.setX(-info.controlPosition1.getX());
            info.controlPosition2.setX(-info.controlPosition2.getX());
            info.position.setX(-info.position.getX());
        }
        break;

        case Type::QUAD_TO:
        {
            QuadToInfo& info = mQuadToInfo;
            info.controlPosition.setX(-info.controlPosition.getX());
            info.position.setX(-info.position.getX());
        }
        break;

        default:
            return;
    }
}

void Shape::verticalFlip() noexcept
{
    switch(mType)
    {
        case Type::ARC:
        {
            ArcInfo& info = mArcInfo;
            info.position.setY(-info.position.getY());
            info.clockWise = ! info.clockWise;

            if(! isPositiveZero(info.startAngle))
            {
                info.startAngle = (2 * kPi) - info.startAngle;
            }

            if(! isPositiveZero(info.endAngle))
            {
                info.endAngle = (2 * kPi) - info.endAngle;
            }
        }
        break;

        case Type::TRIANGLE:
        {
            TriangleInfo& info = mTriangleInfo;
            info.firstVertex.setY(-info.firstVertex.getY());
            info.secondVertex.setY(-info.secondVertex.getY());
            info.thirdVertex.setY(-info.thirdVertex.getY());
        }
        break;

        case Type::RECT:
        {
            RectInfo& info = mRectInfo;
            Point center = info.rect.getCenter();
            info.rect.setCenter(center.getX(), -center.getY());
        }
        break;

        case Type::ELLIPSE:
        {
            EllipseInfo& info = mEllipseInfo;
            info.position.setY(-info.position.getY());
        }
        break;

        case Type::MOVE_TO:
        {
            MoveToInfo& info = mMoveToInfo;
            info.position.setY(-info.position.getY());
        }
        break;

        case Type::LINE_TO:
        {
            LineToInfo& info = mLineToInfo;
            info.position.setY(-info.position.getY());
        }
        break;

        case Type::BEZIER_TO:
        {
            BezierToInfo& info = mBezierToInfo;
            info.controlPosition1.setY(-info.controlPosition1.getY());
            info.controlPosition2.setY(-info.controlPosition2.getY());
            info.position.setY(-info.position.getY());
        }
        break;

        case Type::QUAD_TO:
        {
            QuadToInfo& info = mQuadToInfo;
            info.controlPosition.setY(-info.controlPosition.getY());
            info.position.setY(-info.position.getY());
        }
        break;

        default:
            return;
    }
}

}
