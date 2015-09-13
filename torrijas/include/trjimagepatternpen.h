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

#ifndef TRJ_IMAGE_PATTERN_PEN_H
#define TRJ_IMAGE_PATTERN_PEN_H

#include "trjpen.h"

#include "trjimage.h"

namespace trj
{

class ImagePatternPen : public Pen
{

protected:
    ImagePatternPen(const ImagePatternInfo& info, bool stroke, float strokeWidth) noexcept :
        Pen(info, stroke, strokeWidth)
    {
    }

public:
    ImagePatternPen(const Image& image, const Rect& rect, float angle = 0,
            float opacity = 1) noexcept :
        ImagePatternPen(ImagePatternInfo{rect, angle, opacity, image.getHandle()}, false, 0)
    {
    }

    ImagePatternPen(const Image& image, float positionX, float positionY, float width,
            float height, float angle = 0, float opacity = 1) noexcept :
        ImagePatternPen(image, Rect(positionX, positionY, width, height), angle, opacity)
    {
    }

    int getImageHandle() const noexcept
    {
        return mImagePatternInfo.imageHandle;
    }

    const Rect& getRect() const noexcept
    {
        return mImagePatternInfo.rect;
    }

    float getAngle() const noexcept
    {
        return mImagePatternInfo.angle;
    }

    float getOpacity() const noexcept
    {
        return mImagePatternInfo.opacity;
    }
};


class StrokeImagePatternPen : public ImagePatternPen
{

public:
    StrokeImagePatternPen(const Image& image, const Rect& rect, float angle = 0,
            float opacity = 1, float strokeWidth = 1) noexcept :
        ImagePatternPen(ImagePatternInfo{rect, angle, opacity, image.getHandle()}, true, strokeWidth)
    {
    }

    StrokeImagePatternPen(const Image& image, float positionX, float positionY, float width,
            float height, float angle = 0, float opacity = 1, float strokeWidth = 1) noexcept :
        StrokeImagePatternPen(image, Rect(positionX, positionY, width, height), angle, opacity,
                strokeWidth)
    {
    }
};

}

#endif
