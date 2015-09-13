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

#ifndef TRJ_SHAPE_GROUP_H
#define TRJ_SHAPE_GROUP_H

#include "trjpen.h"
#include "trjshape.h"

namespace trj
{

class Node;
class RenderContext;

class ShapeGroup
{
    friend class Node;

protected:
    Pen mPen;
    Rect mBoundingBox;
    std::vector<Shape> mShapes;
    bool mInvalidateBoundingBox = false;

    void invalidateBoundingBox() noexcept
    {
        mInvalidateBoundingBox = true;
    }

    void render(RenderContext& renderContext) const;

public:
    ShapeGroup() noexcept
    {
    }

    explicit ShapeGroup(const Pen& pen) noexcept :
        mPen(pen)
    {
    }

    const Pen& getPen() const noexcept
    {
        return mPen;
    }

    void setPen(const Pen& pen) noexcept
    {
        mPen = pen;
        invalidateBoundingBox();
    }

    const Rect& getBoundingBox() noexcept;

    const std::vector<Shape>& getShapes() const noexcept
    {
        return mShapes;
    }

    void reserveShapes(int numShapes);

    void addShape(Shape shape)
    {
        mShapes.push_back(std::move(shape));
        invalidateBoundingBox();
    }

    void addHoleShape(Shape shape)
    {
        shape.setHole(true);
        addShape(std::move(shape));
        invalidateBoundingBox();
    }

    void insertShape(int index, Shape shape);

    void setShape(int index, Shape shape) noexcept;

    void setShapes(std::vector<Shape> shapes) noexcept
    {
        mShapes = std::move(shapes);
        invalidateBoundingBox();
    }

    void removeShape(int index) noexcept;

    Shape releaseShape(int index) noexcept;

    void clearShapes() noexcept
    {
        mShapes.clear();
        invalidateBoundingBox();
    }

    void resetShapes() noexcept
    {
        std::vector<Shape>().swap(mShapes);
        invalidateBoundingBox();
    }

    std::vector<Shape> releaseShapes() noexcept;

    void horizontalFlip() noexcept;

    ShapeGroup getHorizontalFlipped() const noexcept
    {
        ShapeGroup flipped = *this;
        flipped.horizontalFlip();

        return flipped;
    }

    void verticalFlip() noexcept;

    ShapeGroup getVerticalFlipped() const noexcept
    {
        ShapeGroup flipped = *this;
        flipped.verticalFlip();

        return flipped;
    }
};

}

#endif
