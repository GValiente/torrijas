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

#ifndef TRJ_IMAGE_NODE_H
#define TRJ_IMAGE_NODE_H

#include "trjnode.h"
#include "trjimage.h"

namespace trj
{

class Image;

class ImageNode : public Node
{

protected:
    Rect mRect;
    Image mImage;
    Rect mImageRegion;
    Rect mImagePatternRect;

    ImageNode(const Size& size, const Image& image, const Rect& imageRegion) noexcept;

    ImageNode(float height, const Image& image, const Rect& imageRegion) noexcept;

    ImageNode(const ImageNode& other) = default;

    void setup() noexcept;

    Rect generateBoundingBox() override;

    bool renderCacheAvailable(const RenderContext& renderContext) const override;

    void renderItself(RenderContext& renderContext) override;

public:
    static Ptr<ImageNode> create(const Size& size, const Image& image,
            const Rect& imageRegion = Rect())
    {
        return Ptr<ImageNode>(new ImageNode(size, image, imageRegion));
    }

    static Ptr<ImageNode> create(float width, float height, const Image& image,
            const Rect& imageRegion = Rect())
    {
        return Ptr<ImageNode>(new ImageNode(Size(width, height), image, imageRegion));
    }

    static Ptr<ImageNode> create(float height, const Image& image, const Rect& imageRegion = Rect())
    {
        return Ptr<ImageNode>(new ImageNode(height, image, imageRegion));
    }

    virtual Ptr<ImageNode> getImageClone() const
    {
        return Ptr<ImageNode>(new ImageNode(*this));
    }

    Ptr<Node> getClone() const override
    {
        return Ptr<Node>(new ImageNode(*this));
    }

    const Size& getSize() const noexcept
    {
        return mRect.getSize();
    }

    const Rect& getRect() const noexcept
    {
        return mRect;
    }

    void setSize(const Size& size) noexcept;

    void setHeight(float height) noexcept;

    const Image& getImage() const noexcept
    {
        return mImage;
    }

    const Rect& getImageRegion() const noexcept
    {
        return mImageRegion;
    }

    void setImage(const Image& image, const Rect& imageRegion = Rect()) noexcept;

    void setImage(const Size& size, const Image& image, const Rect& imageRegion = Rect()) noexcept;

    void setImage(float height, const Image& image, const Rect& imageRegion = Rect()) noexcept;

    void setImageRegion(const Rect& imageRegion) noexcept;

    const Rect& getImagePatternRect() const noexcept
    {
        return mImagePatternRect;
    }
};

}

#endif
