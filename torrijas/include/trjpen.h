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

#ifndef TRJ_PEN_H
#define TRJ_PEN_H

#include "trjrect.h"
#include "trjcolor.h"

struct NVGcontext;

namespace trj
{

class RenderContext;
class ShapeGroup;

enum class LineCap
{
    BUTT,
    ROUND,
    SQUARE
};

enum class LineJoin
{
    ROUND = 1,
    BEVEL = 3
};

class Pen
{
    friend class ShapeGroup;

protected:
    enum class Type
    {
        NONE,
        COLOR,
        LINEAR_GRADIENT,
        BOX_GRADIENT,
        RADIAL_GRADIENT,
        IMAGE_PATTERN
    };

    struct ColorInfo
    {
        Color color;
    };

    struct LinearGradientInfo
    {
        Point startPosition;
        Point endPosition;
        Color innerColor;
        Color outerColor;
    };

    struct BoxGradientInfo
    {
        Rect rect;
        Color innerColor;
        Color outerColor;
        float cornerRadius;
        float cornerBlur;
    };

    struct RadialGradientInfo
    {
        Point position;
        Color innerColor;
        Color outerColor;
        float innerRadius;
        float outerRadius;
    };

    struct ImagePatternInfo
    {
        Rect rect;
        float angle;
        float opacity;
        int imageHandle;
    };

    union
    {
        ColorInfo mColorInfo;
        LinearGradientInfo mLinearGradientInfo;
        BoxGradientInfo mBoxGradientInfo;
        RadialGradientInfo mRadialGradientInfo;
        ImagePatternInfo mImagePatternInfo;
    };

    Type mType = Type::NONE;
    LineCap mLineCap = LineCap::ROUND;
    LineJoin mLineJoin = LineJoin::ROUND;
    float mStrokeWidth = 0;
    bool mStroke = false;

    Pen(const ColorInfo& info, bool stroke, float strokeWidth) noexcept;

    Pen(const LinearGradientInfo& info, bool stroke, float strokeWidth) noexcept;

    Pen(const BoxGradientInfo& info, bool stroke, float strokeWidth) noexcept;

    Pen(const RadialGradientInfo& info, bool stroke, float strokeWidth) noexcept;

    Pen(const ImagePatternInfo& info, bool stroke, float strokeWidth) noexcept;

    void setupLineCapAndLineJoin(NVGcontext& nanoVgContext) const;

    void render(RenderContext& renderContext) const;

public:
    Pen() noexcept
    {
    }

    Pen(const Pen& other)
    {
        *this = other;
    }

    Pen& operator=(const Pen& other);

    Pen(Pen&& other) noexcept
    {
        *this = std::move(other);
    }

    Pen& operator=(Pen&& other) noexcept;

    ~Pen();

    bool isValid() const noexcept
    {
        return (mType != Type::NONE);
    }

    bool isStroke() const noexcept
    {
        return mStroke;
    }

    float getStrokeWidth() const noexcept
    {
        return mStrokeWidth;
    }

    LineCap getLineCap() const noexcept
    {
        return mLineCap;
    }

    void setLineCap(LineCap lineCap) noexcept
    {
        mLineCap = lineCap;
    }

    LineJoin getLineJoin() const noexcept
    {
        return mLineJoin;
    }

    void setLineJoin(LineJoin lineJoin) noexcept
    {
        mLineJoin = lineJoin;
    }

    void horizontalFlip() noexcept;

    Pen getHorizontalFlipped() const noexcept
    {
        Pen flipped = *this;
        flipped.horizontalFlip();

        return flipped;
    }

    void verticalFlip() noexcept;

    Pen getVerticalFlipped() const noexcept
    {
        Pen flipped = *this;
        flipped.verticalFlip();

        return flipped;
    }
};

}

#endif
