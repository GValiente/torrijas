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

#include "trjtextnode.h"

#include "nanovg.h"
#include "trjfont.h"
#include "trjapplication.h"
#include "trjrendercontext.h"
#include "trjdebug.h"

namespace trj
{

TextNode::TextNode() noexcept :
    TextNode(Font::getDefaultFont().getHandle())
{
}

TextNode::TextNode(const Font& font) noexcept :
    TextNode(font.getHandle())
{
}

TextNode::TextNode(const String& fontName) :
    TextNode(Font::getFontHandle(fontName))
{
}

TextNode::TextNode(int fontHandle) noexcept :
    mFontHandle(fontHandle)
{
}

Rect TextNode::generateBoundingBox()
{
    Rect boundingBox = Node::generateBoundingBox();

    if(mFontColor.isVisible())
    {
        NVGcontext& nanoVgContext = Application::getNanoVgContext();
        nvgSave(&nanoVgContext);

        float bounds[4];
        setupContext(nanoVgContext);

        for(const Text& text : mTexts)
        {
            const char* charArray = text.getString().getCharArray();
            float positionX = text.getPosition().getX();
            float positionY = text.getPosition().getY();
            if(isPositive(text.getBoxWidth()))
            {
                nvgTextBoxBounds(&nanoVgContext, positionX, positionY, text.getBoxWidth(),
                        charArray, nullptr, bounds);
            }
            else
            {
                nvgTextBounds(&nanoVgContext, positionX, positionY, charArray, nullptr, bounds);
            }

            boundingBox.join(Rect(Point(bounds[0], bounds[1]), Point(bounds[2], bounds[3])));
        }

        nvgRestore(&nanoVgContext);
    }

    return boundingBox;
}

bool TextNode::renderCacheAvailable(const RenderContext& renderContext) const
{
    return Node::renderCacheAvailable(renderContext) && areEquals(renderContext.getOpacity(), 1);
}

void TextNode::renderItself(RenderContext& renderContext)
{
    NVGcontext& nanoVgContext = renderContext.getNanoVgContext();
    nvgSave(&nanoVgContext);

    setupContext(nanoVgContext);

    Color finalColor = mFontColor.getBlendedColor(renderContext.getBlendColors());
    nvgFillColor(&nanoVgContext, nvgRGBAf(finalColor.getRed(), finalColor.getGreen(),
            finalColor.getBlue(), finalColor.getAlpha()));

    for(const Text& text : mTexts)
    {
        const char* charArray = text.getString().getCharArray();
        float positionX = text.getPosition().getX();
        float positionY = text.getPosition().getY();
        if(isPositive(text.getBoxWidth()))
        {
            nvgTextBox(&nanoVgContext, positionX, positionY, text.getBoxWidth(), charArray, nullptr);
        }
        else
        {
            nvgText(&nanoVgContext, positionX, positionY, charArray, nullptr);
        }
    }

    nvgRestore(&nanoVgContext);

    Node::renderItself(renderContext);
}

void TextNode::setupContext(NVGcontext& nanoVgContext) const
{
    nvgFontFaceId(&nanoVgContext, mFontHandle);
    nvgFontSize(&nanoVgContext, mFontSize);
    nvgFontBlur(&nanoVgContext, mFontBlur);
    nvgTextLetterSpacing(&nanoVgContext, mFontLetterSpacing);
    nvgTextLineHeight(&nanoVgContext, mFontLineHeight);

    int alignment = static_cast<int>(mHorizontalAlignment) | static_cast<int>(mVerticalAlignment);
    nvgTextAlign(&nanoVgContext, alignment);
}

void TextNode::setFont(const Font& font) noexcept
{
    setFont(font.getHandle());
}

void TextNode::setFont(const String& fontName)
{
    setFont(Font::getFontHandle(fontName));
}

void TextNode::setFontSize(float fontSize) noexcept
{
    TRJ_ASSERT(isPositive(fontSize), "Invalid font size");

    mFontSize = fontSize;
    invalidateBoundingBox();
}

void TextNode::setFontBlur(float fontBlur) noexcept
{
    TRJ_ASSERT(fontBlur >= 0, "Invalid font blur");

    mFontBlur = fontBlur;
    invalidateBoundingBox();
}

void TextNode::setFontLetterSpacing(float letterSpacing) noexcept
{
    TRJ_ASSERT(letterSpacing >= 0, "Invalid letter spacing");

    mFontLetterSpacing = letterSpacing;
    invalidateBoundingBox();
}

void TextNode::setFontLineHeight(float lineHeight) noexcept
{
    TRJ_ASSERT(lineHeight >= 0, "Invalid line height");

    mFontLineHeight = lineHeight;
    invalidateBoundingBox();
}

void TextNode::reserveTexts(int numTexts)
{
    TRJ_ASSERT(numTexts > 0, "Invalid num texts");

    mTexts.reserve(numTexts);
}

void TextNode::insertText(int index, Text text)
{
    TRJ_ASSERT(index >= 0 && index <= (int) mTexts.size(), "Invalid text index");

    mTexts.insert(mTexts.begin() + index, std::move(text));
    invalidateBoundingBox();
}

void TextNode::setText(int index, Text text) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mTexts.size(), "Invalid text index");

    mTexts[index] = std::move(text);
    invalidateBoundingBox();
}

void TextNode::removeText(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mTexts.size(), "Invalid text index");

    mTexts.erase(mTexts.begin() + index);
    invalidateBoundingBox();
}

TextNode::Text TextNode::releaseText(int index) noexcept
{
    TRJ_ASSERT(index >= 0 && index < (int) mTexts.size(), "Invalid text index");

    Text text(std::move(mTexts[index]));
    mTexts.erase(mTexts.begin() + index);
    invalidateBoundingBox();

    return text;
}

std::vector<TextNode::Text> TextNode::releaseTexts() noexcept
{
    std::vector<Text> texts;
    texts.swap(mTexts);
    invalidateBoundingBox();

    return texts;
}

void TextNode::getTextMetrics(float& ascender, float& descender, float& lineHeight) const
{
    NVGcontext& nanoVgContext = Application::getNanoVgContext();
    nvgSave(&nanoVgContext);

    setupContext(nanoVgContext);

    nvgTextMetrics(&nanoVgContext, &ascender, &descender, &lineHeight);

    nvgRestore(&nanoVgContext);
}

}
