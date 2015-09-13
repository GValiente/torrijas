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

#include "trjcolor.h"

#include "nanovg.h"
#include "trjdebug.h"

namespace trj
{

Color Color::createFromHSL(float hue, float saturation, float lightness)
{
    TRJ_ASSERT(hue >= 0 && hue <= 1, "Invalid hue");
    TRJ_ASSERT(saturation >= 0 && saturation <= 1, "Invalid saturation");
    TRJ_ASSERT(lightness >= 0 && lightness <= 1, "Invalid lightness");

    NVGcolor nvgColor = nvgHSL(hue, saturation, lightness);

    return Color(nvgColor.r, nvgColor.g, nvgColor.b);
}

Color Color::createFromHSL(float hue, float saturation, float lightness, float alpha)
{
    TRJ_ASSERT(alpha >= 0 && alpha <= 1, "Invalid alpha");

    Color color = createFromHSL(hue, saturation, lightness);
    color.mAlpha = alpha;

    return color;
}

Color Color::createFromChars(unsigned char red, unsigned char green, unsigned char blue,
        unsigned char alpha) noexcept
{
    return Color(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

std::pair<Color, float> Color::getBlendResult(
        const std::vector<std::pair<Color, float>>& blendColors) noexcept
{
    std::pair<Color, float> result = std::make_pair(Color(), 0.0f);
    int numColors = blendColors.size();
    if(numColors)
    {
        result = blendColors.front();

        for(int index = 1; index < numColors; ++index)
        {
            const std::pair<Color, float>& blendColor = blendColors[index];
            result.first = result.first.getBlendedColor(blendColor.first, blendColor.second);
        }
    }

    return result;
}

Color::Color(float red, float green, float blue, float alpha) noexcept :
    mRed(red),
    mGreen(green),
    mBlue(blue),
    mAlpha(alpha)
{
    TRJ_ASSERT(red >= 0 && red <= 1, "Invalid red");
    TRJ_ASSERT(green >= 0 && green <= 1, "Invalid green");
    TRJ_ASSERT(blue >= 0 && blue <= 1, "Invalid blue");
    TRJ_ASSERT(alpha >= 0 && alpha <= 1, "Invalid alpha");
}

void Color::setRed(float red) noexcept
{
    TRJ_ASSERT(red >= 0 && red <= 1, "Invalid red");

    mRed = red;
}

void Color::setGreen(float green) noexcept
{
    TRJ_ASSERT(green >= 0 && green <= 1, "Invalid green");

    mGreen = green;
}

void Color::setBlue(float blue) noexcept
{
    TRJ_ASSERT(blue >= 0 && blue <= 1, "Invalid blue");

    mBlue = blue;
}

void Color::setAlpha(float alpha) noexcept
{
    TRJ_ASSERT(alpha >= 0 && alpha <= 1, "Invalid alpha");

    mAlpha = alpha;
}

Color Color::getBlendedColor(const Color& blendColor, float blendFactor) const noexcept
{
    TRJ_ASSERT(blendFactor >= 0 && blendFactor <= 1, "Invalid blend factor");

    float blendInverseFactor = 1 - blendFactor;
    float red = (mRed * blendInverseFactor) + (blendColor.mRed * blendFactor);
    float green = (mGreen * blendInverseFactor) + (blendColor.mGreen * blendFactor);
    float blue = (mBlue * blendInverseFactor) + (blendColor.mBlue * blendFactor);
    float alpha = (mAlpha * blendInverseFactor) + (blendColor.mAlpha * blendFactor);

    return Color(red, green, blue, alpha);
}

Color Color::getBlendedColor(const std::vector<std::pair<Color, float>>& blendColors) const noexcept
{
    Color finalColor = *this;

    for(const std::pair<Color, float>& blendColor : blendColors)
    {
        finalColor = finalColor.getBlendedColor(blendColor.first, blendColor.second);
    }

    return finalColor;
}

}
