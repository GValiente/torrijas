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

#include "trjpen.h"

#include <vector>
#include "nanovg.h"
#include "trjrendercontext.h"
#include "trjdebug.h"
#include "private/trjimagemanager.h"

namespace trj
{

namespace
{
    NVGcolor getNVGColor(const Color& color,
            const std::vector<std::pair<Color, float>>& blendColors) noexcept
    {
        Color finalColor = color.getBlendedColor(blendColors);

        return nvgRGBAf(finalColor.getRed(), finalColor.getGreen(), finalColor.getBlue(),
                finalColor.getAlpha());
    }

    void renderPaint(NVGcontext& nanoVgContext, const NVGpaint& nanoVgPaint, bool stroke,
            float strokeWidth)
    {
        if(stroke)
        {
            nvgStrokeWidth(&nanoVgContext, strokeWidth);
            nvgStrokePaint(&nanoVgContext, nanoVgPaint);
            nvgStroke(&nanoVgContext);
        }
        else
        {
            nvgFillPaint(&nanoVgContext, nanoVgPaint);
            nvgFill(&nanoVgContext);
        }
    }
}

Pen::Pen(const ColorInfo& info, bool stroke, float strokeWidth) noexcept :
    mColorInfo(info),
    mType(Type::COLOR),
    mStrokeWidth(strokeWidth),
    mStroke(stroke)
{
    TRJ_ASSERT(strokeWidth >= 0, "Invalid stroke width");
}

Pen::Pen(const LinearGradientInfo& info, bool stroke, float strokeWidth) noexcept :
    mLinearGradientInfo(info),
    mType(Type::LINEAR_GRADIENT),
    mStrokeWidth(strokeWidth),
    mStroke(stroke)
{
    TRJ_ASSERT(strokeWidth >= 0, "Invalid stroke width");
}

Pen::Pen(const BoxGradientInfo& info, bool stroke, float strokeWidth) noexcept :
    mBoxGradientInfo(info),
    mType(Type::BOX_GRADIENT),
    mStrokeWidth(strokeWidth),
    mStroke(stroke)
{
    TRJ_ASSERT(! info.rect.isEmpty(), "Rect is empty");
    TRJ_ASSERT(isPositive(info.cornerRadius), "Invalid corner radius");
    TRJ_ASSERT(info.cornerBlur < 0 || info.cornerBlur > 1, "Invalid corner blur");
    TRJ_ASSERT(strokeWidth >= 0, "Invalid stroke width");
}

Pen::Pen(const RadialGradientInfo& info, bool stroke, float strokeWidth) noexcept :
    mRadialGradientInfo(info),
    mType(Type::RADIAL_GRADIENT),
    mStrokeWidth(strokeWidth),
    mStroke(stroke)
{
    TRJ_ASSERT(strokeWidth >= 0, "Invalid stroke width");
}

Pen::Pen(const ImagePatternInfo& info, bool stroke, float strokeWidth) noexcept :
    mImagePatternInfo(info),
    mType(Type::IMAGE_PATTERN),
    mStrokeWidth(strokeWidth),
    mStroke(stroke)
{
    TRJ_ASSERT(strokeWidth >= 0, "Invalid stroke width");

    priv::ImageManager::addImageRef(mImagePatternInfo.imageHandle);
}

Pen& Pen::operator=(const Pen& other)
{
    if(this != &other)
    {
        mType = other.mType;
        mLineCap = other.mLineCap;
        mLineJoin = other.mLineJoin;
        mStrokeWidth = other.mStrokeWidth;
        mStroke = other.mStroke;

        switch(mType)
        {
            case Type::COLOR:
            {
                mColorInfo = other.mColorInfo;
                break;
            }

            case Type::LINEAR_GRADIENT:
            {
                mLinearGradientInfo = other.mLinearGradientInfo;
                break;
            }

            case Type::BOX_GRADIENT:
            {
                mBoxGradientInfo = other.mBoxGradientInfo;
                break;
            }

            case Type::RADIAL_GRADIENT:
            {
                mRadialGradientInfo = other.mRadialGradientInfo;
                break;
            }

            case Type::IMAGE_PATTERN:
            {
                mImagePatternInfo = other.mImagePatternInfo;
                priv::ImageManager::addImageRef(mImagePatternInfo.imageHandle);
                break;
            }

            case Type::NONE:
                break;
        }
    }

    return *this;
}

Pen& Pen::operator=(Pen&& other) noexcept
{
    if(this != &other)
    {
        mType = other.mType;
        mLineCap = other.mLineCap;
        mLineJoin = other.mLineJoin;
        mStrokeWidth = other.mStrokeWidth;
        mStroke = other.mStroke;

        switch(mType)
        {
            case Type::COLOR:
            {
                mColorInfo = other.mColorInfo;
                break;
            }

            case Type::LINEAR_GRADIENT:
            {
                mLinearGradientInfo = other.mLinearGradientInfo;
                break;
            }

            case Type::BOX_GRADIENT:
            {
                mBoxGradientInfo = other.mBoxGradientInfo;
                break;
            }

            case Type::RADIAL_GRADIENT:
            {
                mRadialGradientInfo = other.mRadialGradientInfo;
                break;
            }

            case Type::IMAGE_PATTERN:
            {
                mImagePatternInfo = other.mImagePatternInfo;
                other.mImagePatternInfo.imageHandle = 0;
                break;
            }

            case Type::NONE:
                break;
        }
    }

    return *this;
}

Pen::~Pen()
{
    if(mType == Type::IMAGE_PATTERN && mImagePatternInfo.imageHandle)
    {
        priv::ImageManager::removeImageRef(mImagePatternInfo.imageHandle);
    }
}

void Pen::setupLineCapAndLineJoin(NVGcontext& nanoVgContext) const
{
    nvgLineCap(&nanoVgContext, static_cast<int>(mLineCap));
    nvgLineJoin(&nanoVgContext, static_cast<int>(mLineJoin));
}

void Pen::render(RenderContext& renderContext) const
{
    NVGcontext& nanoVgContext = renderContext.getNanoVgContext();
    const auto& blendColors = renderContext.getBlendColors();

    switch(mType)
    {
        case Type::COLOR:
        {
            setupLineCapAndLineJoin(nanoVgContext);

            if(mStroke)
            {
                nvgStrokeWidth(&nanoVgContext, mStrokeWidth);
                nvgStrokeColor(&nanoVgContext, getNVGColor(mColorInfo.color, blendColors));
                nvgStroke(&nanoVgContext);
            }
            else
            {
                nvgFillColor(&nanoVgContext, getNVGColor(mColorInfo.color, blendColors));
                nvgFill(&nanoVgContext);
            }

            break;
        }

        case Type::LINEAR_GRADIENT:
        {
            const LinearGradientInfo& info = mLinearGradientInfo;
            const Point& startPosition = info.startPosition;
            const Point& endPosition = info.endPosition;
            NVGpaint paint = nvgLinearGradient(&nanoVgContext, startPosition.getX(),
                    startPosition.getY(), endPosition.getX(), endPosition.getY(),
                    getNVGColor(info.innerColor, blendColors),
                    getNVGColor(info.outerColor, blendColors));

            setupLineCapAndLineJoin(nanoVgContext);
            renderPaint(nanoVgContext, paint, mStroke, mStrokeWidth);
            break;
        }

        case Type::BOX_GRADIENT:
        {
            const BoxGradientInfo& info = mBoxGradientInfo;
            const Rect& rect = info.rect;
            NVGpaint paint = nvgBoxGradient(&nanoVgContext, rect.getX(), rect.getY(),
                    rect.getWidth(), rect.getHeight(), info.cornerRadius, info.cornerBlur,
                    getNVGColor(info.innerColor, blendColors),
                    getNVGColor(info.outerColor, blendColors));

            setupLineCapAndLineJoin(nanoVgContext);
            renderPaint(nanoVgContext, paint, mStroke, mStrokeWidth);
            break;
        }

        case Type::RADIAL_GRADIENT:
        {
            const RadialGradientInfo& info = mRadialGradientInfo;
            const Point& position = info.position;
            NVGpaint paint = nvgRadialGradient(&nanoVgContext, position.getX(),
                    position.getY(), info.innerRadius, info.outerRadius,
                    getNVGColor(info.innerColor, blendColors),
                    getNVGColor(info.outerColor, blendColors));

            setupLineCapAndLineJoin(nanoVgContext);
            renderPaint(nanoVgContext, paint, mStroke, mStrokeWidth);
            break;
        }

        case Type::IMAGE_PATTERN:
        {
            const ImagePatternInfo& info = mImagePatternInfo;
            const Rect& rect = info.rect;
            NVGpaint paint = nvgImagePattern(&nanoVgContext, rect.getX(), rect.getY(),
                    rect.getWidth(), rect.getHeight(), info.angle, info.imageHandle, info.opacity);

            setupLineCapAndLineJoin(nanoVgContext);
            renderPaint(nanoVgContext, paint, mStroke, mStrokeWidth);
            break;
        }

        case Type::NONE:
            break;
    }
}

void Pen::horizontalFlip() noexcept
{
    switch(mType)
    {
        case Type::LINEAR_GRADIENT:
        {
            LinearGradientInfo& info = mLinearGradientInfo;
            info.startPosition.setX(-info.startPosition.getX());
            info.endPosition.setX(-info.endPosition.getX());
            break;
        }

        case Type::BOX_GRADIENT:
        {
            BoxGradientInfo& info = mBoxGradientInfo;
            Point center = info.rect.getCenter();
            info.rect.setCenter(-center.getX(), center.getY());
            break;
        }

        case Type::RADIAL_GRADIENT:
        {
            RadialGradientInfo& info = mRadialGradientInfo;
            info.position.setX(-info.position.getX());
            break;
        }

        default:
            return;
    }
}

void Pen::verticalFlip() noexcept
{
    switch(mType)
    {
        case Type::LINEAR_GRADIENT:
        {
            LinearGradientInfo& info = mLinearGradientInfo;
            info.startPosition.setY(-info.startPosition.getY());
            info.endPosition.setY(-info.endPosition.getY());
            break;
        }

        case Type::BOX_GRADIENT:
        {
            BoxGradientInfo& info = mBoxGradientInfo;
            Point center = info.rect.getCenter();
            info.rect.setCenter(center.getX(), -center.getY());
            break;
        }

        case Type::RADIAL_GRADIENT:
        {
            RadialGradientInfo& info = mRadialGradientInfo;
            info.position.setY(-info.position.getY());
            break;
        }

        default:
            return;
    }
}

}
