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

#include "trjshapegroup.h"

#include "nanovg.h"
#include "trjrendercontext.h"
#include "trjdebug.h"

namespace trj
{

void ShapeGroup::render(RenderContext& renderContext) const
{
    if(mPen.isValid() && ! mShapes.empty())
    {
        NVGcontext& nanoVgContext = renderContext.getNanoVgContext();
        nvgBeginPath(&nanoVgContext);

        for(const Shape& shape : mShapes)
        {
            shape.render(nanoVgContext);
        }

        mPen.render(renderContext);
    }
}

const Rect& ShapeGroup::getBoundingBox() noexcept
{
    if(mInvalidateBoundingBox)
    {
        mBoundingBox = Rect();
        mInvalidateBoundingBox = false;

        if(mPen.isValid() && ! mShapes.empty())
        {
            Point lastPoint;
            for(const Shape& shape : mShapes)
            {
                shape.joinToBoundingBox(mBoundingBox, lastPoint);
            }

            if(mPen.isStroke() && ! mBoundingBox.isEmpty())
            {
                float strokeWidth = mPen.getStrokeWidth();
                if(mPen.getLineCap() == LineCap::SQUARE)
                {
                    float sizeDelta = strokeWidth * 1.5f;
                    float halfSizeDelta = sizeDelta * 0.5f;
                    mBoundingBox = Rect(mBoundingBox.getX() - halfSizeDelta,
                            mBoundingBox.getY() - halfSizeDelta,
                            mBoundingBox.getWidth() + sizeDelta,
                            mBoundingBox.getHeight() + sizeDelta);
                }
                else
                {
                    float halfStrokeWidth = strokeWidth * 0.5f;
                    mBoundingBox = Rect(mBoundingBox.getX() - halfStrokeWidth,
                            mBoundingBox.getY() - halfStrokeWidth,
                            mBoundingBox.getWidth() + strokeWidth,
                            mBoundingBox.getHeight() + strokeWidth);
                }
            }
        }
    }

    return mBoundingBox;
}

void ShapeGroup::reserveShapes(int numShapes)
{
    TRJ_ASSERT(numShapes > 0, "Invalid num shapes");

    mShapes.reserve(numShapes);
}

void ShapeGroup::insertShape(int index, Shape shape)
{
    TRJ_ASSERT(index >= 0 && index <= (int) mShapes.size(), "Invalid shape index");

    mShapes.insert(mShapes.begin() + index, std::move(shape));
    invalidateBoundingBox();
}

void ShapeGroup::setShape(int index, Shape shape) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mShapes.size(), "Invalid shape index");

    mShapes[index] = std::move(shape);
    invalidateBoundingBox();
}

void ShapeGroup::removeShape(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mShapes.size(), "Invalid shape index");

    mShapes.erase(mShapes.begin() + index);
    invalidateBoundingBox();
}

Shape ShapeGroup::releaseShape(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mShapes.size(), "Invalid shape index");

    auto it = mShapes.begin() + index;
    Shape releasedShape = std::move(*it);
    mShapes.erase(it);
    invalidateBoundingBox();

    return releasedShape;
}

std::vector<Shape> ShapeGroup::releaseShapes() noexcept
{
    std::vector<Shape> releasedShapes;
    releasedShapes.swap(mShapes);
    invalidateBoundingBox();

    return releasedShapes;
}

void ShapeGroup::horizontalFlip() noexcept
{
    for(Shape& shape : mShapes)
    {
        shape.horizontalFlip();
    }

    mPen.horizontalFlip();
    invalidateBoundingBox();
}

void ShapeGroup::verticalFlip() noexcept
{
    for(Shape& shape : mShapes)
    {
        shape.verticalFlip();
    }

    mPen.verticalFlip();
    invalidateBoundingBox();
}

}
