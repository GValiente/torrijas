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

#ifndef TRJ_TEXT_NODE_H
#define TRJ_TEXT_NODE_H

#include "trjnode.h"
#include "trjstring.h"

struct NVGcontext;

namespace trj
{

class Font;

class TextNode : public Node
{

public:
    class Text
    {

    protected:
        Point mPosition;
        String mString;
        float mBoxWidth;

    public:
        Text(const Point& position, String string, float boxWidth = 0) noexcept :
            mPosition(position),
            mString(std::move(string)),
            mBoxWidth(boxWidth)
        {
        }

        Text(float positionX, float positionY, String string, float boxWidth = 0) noexcept :
            Text(Point(positionX, positionY), std::move(string), boxWidth)
        {
        }

        const Point& getPosition() const noexcept
        {
            return mPosition;
        }

        void setPosition(const Point& position) noexcept
        {
            mPosition = position;
        }

        void setPosition(float x, float y) noexcept
        {
            setPosition(Point(x, y));
        }

        const String& getString() const noexcept
        {
            return mString;
        }

        void setString(String string) noexcept
        {
            mString = std::move(string);
        }

        float getBoxWidth() const noexcept
        {
            return mBoxWidth;
        }

        void setBoxWidth(float boxWidth) noexcept
        {
            mBoxWidth = boxWidth;
        }
    };

    enum class HorizontalAlignment
    {
        LEFT    = 1 << 0, // Default, align text horizontally to left.
        CENTER 	= 1 << 1, // Align text horizontally to center.
        RIGHT 	= 1 << 2  // Align text horizontally to right.
    };

    enum class VerticalAlignment
    {
        TOP         = 1 << 3, // Align text vertically to top.
        MIDDLE      = 1 << 4, // Align text vertically to middle.
        BOTTOM      = 1 << 5, // Align text vertically to bottom.
        BASELINE    = 1 << 6  // Default, align text vertically to baseline.
    };

protected:
    Color mFontColor;
    float mFontSize = 18;
    float mFontBlur = 0;
    float mFontLetterSpacing = 0;
    float mFontLineHeight = 1;
    HorizontalAlignment mHorizontalAlignment = HorizontalAlignment::LEFT;
    VerticalAlignment mVerticalAlignment = VerticalAlignment::BASELINE;
    int mFontHandle;

    std::vector<Text> mTexts;

    TextNode() noexcept;

    TextNode(const Font& font) noexcept;

    TextNode(const String& fontName);

    TextNode(int fontHandle) noexcept;

    TextNode(const TextNode& other) = default;

    Rect generateBoundingBox() override;

    bool renderCacheAvailable(const RenderContext& renderContext) const override;

    void renderItself(RenderContext& renderContext) override;

    void setupContext(NVGcontext& nanoVgContext) const;

public:
    static Ptr<TextNode> create()
    {
        return Ptr<TextNode>(new TextNode());
    }

    static Ptr<TextNode> create(const Font& font)
    {
        return Ptr<TextNode>(new TextNode(font));
    }

    static Ptr<TextNode> create(const String& fontName)
    {
        return Ptr<TextNode>(new TextNode(fontName));
    }

    static Ptr<TextNode> create(int fontHandle)
    {
        return Ptr<TextNode>(new TextNode(fontHandle));
    }

    virtual Ptr<TextNode> getTextClone() const
    {
        return Ptr<TextNode>(new TextNode(*this));
    }

    Ptr<Node> getClone() const override
    {
        return Ptr<Node>(new TextNode(*this));
    }

    int getFontHandle() const noexcept
    {
        return mFontHandle;
    }

    void setFont(const Font& font) noexcept;

    void setFont(const String& fontName);

    void setFont(int fontHandle) noexcept
    {
        mFontHandle = fontHandle;
        invalidateBoundingBox();
    }

    const Color& getFontColor() const noexcept
    {
        return mFontColor;
    }

    void setFontColor(const Color& color) noexcept
    {
        mFontColor = color;
    }

    void setFontColor(float red, float green, float blue, float alpha = 1) noexcept
    {
        setFontColor(Color(red, green, blue, alpha));
    }

    float getFontSize() const noexcept
    {
        return mFontSize;
    }

    void setFontSize(float fontSize) noexcept;

    float getFontBlur() const noexcept
    {
        return mFontBlur;
    }

    void setFontBlur(float fontBlur) noexcept;

    float getFontLetterSpacing() const noexcept
    {
        return mFontLetterSpacing;
    }

    void setFontLetterSpacing(float letterSpacing) noexcept;

    float getFontLineHeight() const noexcept
    {
        return mFontLineHeight;
    }

    void setFontLineHeight(float lineHeight) noexcept;

    HorizontalAlignment getHorizontalAlignment() const noexcept
    {
        return mHorizontalAlignment;
    }

    void setHorizontalAlignment(HorizontalAlignment alignment) noexcept
    {
        mHorizontalAlignment = alignment;
        invalidateBoundingBox();
    }

    VerticalAlignment getVerticalAlignment() const noexcept
    {
        return mVerticalAlignment;
    }

    void setVerticalAlignment(VerticalAlignment alignment) noexcept
    {
        mVerticalAlignment = alignment;
        invalidateBoundingBox();
    }

    const std::vector<Text>& getTexts() const noexcept
    {
        return mTexts;
    }

    void reserveTexts(int numTexts);

    void addText(const Point& position, String string, float boxWidth = 0)
    {
        addText(Text(position, std::move(string), boxWidth));
    }

    void addText(float positionX, float positionY, String string, float boxWidth = 0)
    {
        addText(Text(Point(positionX, positionY), std::move(string), boxWidth));
    }

    void addText(Text text)
    {
        mTexts.push_back(std::move(text));
        invalidateBoundingBox();
    }

    void insertText(int index, Text text);

    void setText(int index, Text text) noexcept;

    void setTexts(std::vector<Text> texts) noexcept
    {
        mTexts = std::move(texts);
        invalidateBoundingBox();
    }

    void removeText(int index) noexcept;

    Text releaseText(int index) noexcept;

    void clearTexts() noexcept
    {
        mTexts.clear();
        invalidateBoundingBox();
    }

    void resetTexts() noexcept
    {
        std::vector<Text>().swap(mTexts);
        invalidateBoundingBox();
    }

    std::vector<Text> releaseTexts() noexcept;

    void getTextMetrics(float& ascender, float& descender, float& lineHeight) const;

    float getTextAscender() const
    {
        float ascender, descender, lineHeight;
        getTextMetrics(ascender, descender, lineHeight);
        return ascender;
    }

    float getTextDescender() const
    {
        float ascender, descender, lineHeight;
        getTextMetrics(ascender, descender, lineHeight);
        return descender;
    }

    float getTextLineHeight() const
    {
        float ascender, descender, lineHeight;
        getTextMetrics(ascender, descender, lineHeight);
        return lineHeight;
    }
};

}

#endif
