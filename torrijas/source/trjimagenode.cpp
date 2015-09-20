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

#include "trjimagenode.h"

#include "nanovg.h"
#include "trjimage.h"
#include "trjrendercontext.h"

namespace trj
{

ImageNode::ImageNode(const Size& size, const Image& image, const Rect& imageRegion) noexcept :
    mImage(image)
{
    setImageRegion(imageRegion);
    setSize(size);
}

ImageNode::ImageNode(float height, const Image& image, const Rect& imageRegion) noexcept :
    mImage(image)
{
    setImageRegion(imageRegion);
    setHeight(height);
}

void ImageNode::setup() noexcept
{
    mImagePatternRect = Rect(
            mRect.getX() - (mRect.getWidth() * (mImageRegion.getX() / mImageRegion.getWidth())),
            mRect.getY() - (mRect.getHeight() * (mImageRegion.getY() / mImageRegion.getHeight())),
            mRect.getWidth() * (mImage.getWidth() / mImageRegion.getWidth()),
            mRect.getHeight() * (mImage.getHeight() / mImageRegion.getHeight()));
}

Rect ImageNode::generateBoundingBox()
{
    Rect boundingBox = Node::generateBoundingBox();
    boundingBox.join(mRect);
    return boundingBox;
}

void ImageNode::renderItself(RenderContext& renderContext)
{
    NVGcontext& nanoVgContext = renderContext.getNanoVgContext();
    NVGpaint paint = nvgImagePattern(&nanoVgContext, mImagePatternRect.getX(),
            mImagePatternRect.getY(), mImagePatternRect.getWidth(), mImagePatternRect.getHeight(),
            0, mImage.getHandle(), 1);
    nvgBeginPath(&nanoVgContext);
    nvgRect(&nanoVgContext, mRect.getX(), mRect.getY(), mRect.getWidth(), mRect.getHeight());
    nvgFillPaint(&nanoVgContext, paint);
    nvgFill(&nanoVgContext);

    if(! renderContext.getBlendColors().empty())
    {
        std::pair<Color, float> blendResult = renderContext.getBlendResult();
        const Color& blendColor = blendResult.first;
        nvgBeginPath(&nanoVgContext);
        nvgRect(&nanoVgContext, mRect.getX(), mRect.getY(), mRect.getWidth(), mRect.getHeight());
        nvgFillColor(&nanoVgContext, nvgRGBAf(blendColor.getRed(), blendColor.getGreen(),
                blendColor.getBlue(), blendColor.getAlpha() * blendResult.second));
        nvgFill(&nanoVgContext);
    }

    Node::renderItself(renderContext);
}

void ImageNode::setSize(const Size& size) noexcept
{
    TRJ_ASSERT(! size.isEmpty(), "Size is empty");

    mRect = Rect(size.getWidth() * -0.5f, size.getHeight() * -0.5f, size.getWidth(), size.getHeight());
    setup();
    invalidateBoundingBox();
}

void ImageNode::setHeight(float height) noexcept
{
    TRJ_ASSERT(isPositive(height), "Invalid height");

    float width = (height * mImageRegion.getWidth()) / mImageRegion.getHeight();
    setSize(Size(width, height));
}

void ImageNode::setImage(const Image& image, const Rect& imageRegion) noexcept
{
    mImage = image;
    setImageRegion(imageRegion);
}

void ImageNode::setImage(const Size& size, const Image& image, const Rect& imageRegion) noexcept
{
    setImage(image, imageRegion);
    setSize(size);
}

void ImageNode::setImage(float height, const Image& image, const Rect& imageRegion) noexcept
{
    setImage(image, imageRegion);
    setHeight(height);
}

void ImageNode::setImageRegion(const Rect& imageRegion) noexcept
{
    if(imageRegion.isEmpty())
    {
        setImageRegion(Rect(mImageRegion.getPosition(), Size(mImage.getWidth(), mImage.getHeight())));
        return;
    }

    TRJ_ASSERT(imageRegion.getX() >= 0 && imageRegion.getX() < mImage.getWidth(),
            "Invalid imageRegion x coordinate");
    TRJ_ASSERT(imageRegion.getY() >= 0 && imageRegion.getY() < mImage.getHeight(),
            "Invalid imageRegion y coordinate");
    TRJ_ASSERT(imageRegion.getX() + imageRegion.getWidth() <= mImage.getWidth(),
            "Invalid imageRegion width");
    TRJ_ASSERT(imageRegion.getY() + imageRegion.getHeight() <= mImage.getHeight(),
            "Invalid imageRegion height");

    mImageRegion = imageRegion;
    setup();
    invalidateRenderCache();
}

}
