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

#ifndef TRJ_COLOR_H
#define TRJ_COLOR_H

#include <vector>
#include "trjcommon.h"

namespace trj
{

class Color
{

protected:
    float mRed;
    float mGreen;
    float mBlue;
    float mAlpha;

public:
    static Color createFromHSL(float hue, float saturation, float lightness);

    static Color createFromHSL(float hue, float saturation, float lightness, float alpha);

    static Color createFromChars(unsigned char red, unsigned char green,
            unsigned char blue, unsigned char alpha = 255) noexcept;

    static std::pair<Color, float> getBlendResult(
            const std::vector<std::pair<Color, float>>& blendColors) noexcept;

    Color() noexcept :
        Color(0, 0, 0, 1)
    {
    }

    Color(float red, float green, float blue, float alpha = 1) noexcept;

    float getRed() const noexcept
    {
        return mRed;
    }

    unsigned char getRedChar() const noexcept
    {
        return (unsigned char)(mRed * 255);
    }

    void setRed(float red) noexcept;

    void setRed(unsigned char red) noexcept
    {
        mRed = red / 255.0f;
    }

    float getGreen() const noexcept
    {
        return mGreen;
    }

    unsigned char getGreenChar() const noexcept
    {
        return mGreen * 255;
    }

    void setGreen(float green) noexcept;

    void setGreen(unsigned char green) noexcept
    {
        mGreen = green / 255.0f;
    }

    float getBlue() const noexcept
    {
        return mBlue;
    }

    unsigned char getBlueChar() const noexcept
    {
        return mBlue * 255;
    }

    void setBlue(float blue) noexcept;

    void setBlue(unsigned char blue) noexcept
    {
        mBlue = blue / 255.0f;
    }

    float getAlpha() const noexcept
    {
        return mAlpha;
    }

    unsigned char getAlphaChar() const noexcept
    {
        return mAlpha * 255;
    }

    void setAlpha(float alpha) noexcept;

    void setAlpha(unsigned char alpha) noexcept
    {
        mAlpha = alpha / 255.0f;
    }

    bool isVisible() const noexcept
    {
        return isPositive(mAlpha);
    }

    Color getBlendedColor(const Color& blendColor, float blendFactor) const noexcept;

    Color getBlendedColor(const std::vector<std::pair<Color, float>>& blendColors) const noexcept;

    bool operator==(const Color& other) const
    {
        return areEquals(mRed, other.mRed) && areEquals(mGreen, other.mGreen) &&
                areEquals(mBlue, other.mBlue) && areEquals(mAlpha, other.mAlpha);
    }

    bool operator!=(const Color& other) const
    {
        return ! (operator==(other));
    }
};

}

#endif
