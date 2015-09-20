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

#include "trjimagedata.h"

#include <stdlib.h>
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "trjapplication.h"
#include "trjfile.h"
#include "trjcolor.h"
#include "trjexception.h"
#include "trjdebug.h"

namespace trj
{

ImageData ImageData::getScreenshot()
{
    return Application::getScreenshot();
}

ImageData::ImageData(int width, int height, bool clear) :
    mWidth(width),
    mHeight(height)
{
    TRJ_ASSERT(width > 0, "Invalid width");
    TRJ_ASSERT(height > 0, "Invalid height");

    int numBytes = getNumBytes();
    mData = static_cast<unsigned char*>(malloc(numBytes));
    if(! mData)
    {
        throw Exception(__FILE__, __LINE__, "Image build failed");
    }

    if(clear)
    {
        memset(mData, 0, numBytes);
    }
}

ImageData::ImageData(const unsigned char* data, int width, int height) :
    ImageData(width, height, false)
{
    TRJ_ASSERT(data, "Input data is null");

    memcpy(mData, data, getNumBytes());
}

ImageData::ImageData(const String& filePath)
{
    int numComponents;
    mData = stbi_load(filePath.getCharArray(), &mWidth, &mHeight, &numComponents, 4);
    if(! mData)
    {
        throw Exception(__FILE__, __LINE__, "Image file load failed");
    }
}

ImageData::ImageData(const File& file) :
    ImageData(file.getPath())
{
}

ImageData::ImageData(const unsigned char* compressedData, int compressedDataSize)
{
    TRJ_ASSERT(compressedData, "Data is null");
    TRJ_ASSERT(compressedDataSize > 0, "Invalid data size");

    int numComponents;
    mData = stbi_load_from_memory(compressedData, compressedDataSize, &mWidth, &mHeight,
            &numComponents, 4);
    if(! mData)
    {
        throw Exception(__FILE__, __LINE__, "Image data load failed");
    }
}

ImageData::ImageData(ImageData&& other) noexcept :
    mData(other.mData),
    mWidth(other.mWidth),
    mHeight(other.mHeight)
{
    other.mData = nullptr;
    other.mWidth = 0;
    other.mHeight = 0;
}

ImageData& ImageData::operator=(ImageData&& other) noexcept
{
    if(this != &other)
    {
        mData = other.mData;
        mWidth = other.mWidth;
        mHeight = other.mHeight;

        other.mData = nullptr;
        other.mWidth = 0;
        other.mHeight = 0;
    }

    return *this;
}

ImageData::~ImageData()
{
    if(mData)
    {
        stbi_image_free(mData);
        mData = nullptr;
    }
}

Color ImageData::getColor(int x, int y) const noexcept
{
    TRJ_ASSERT(x >= 0 && x < mWidth, "Invalid x");
    TRJ_ASSERT(y >= 0 && y < mHeight, "Invalid y");

    unsigned char* colorData = mData + (((y * mHeight) + x) * 4);
    return Color::createFromChars(colorData[0], colorData[1], colorData[2], colorData[3]);
}

void ImageData::setColor(int x, int y, const Color& color) noexcept
{
    setColor(x, y, color.getRedChar(), color.getGreenChar(), color.getBlueChar(), color.getAlphaChar());
}

void ImageData::setColor(int x, int y, unsigned char red, unsigned char green, unsigned char blue,
        unsigned char alpha) noexcept
{
    TRJ_ASSERT(x >= 0 && x < mWidth, "Invalid x");
    TRJ_ASSERT(y >= 0 && y < mHeight, "Invalid y");

    unsigned char* colorData = mData + (((y * mHeight) + x) * 4);
    colorData[0] = red;
    colorData[1] = green;
    colorData[2] = blue;
    colorData[3] = alpha;
}

void ImageData::save(const String& filePath, FileFormat format) const
{
    bool success = false;
    switch(format)
    {

    case FileFormat::PNG:
        success = stbi_write_png(filePath.getCharArray(), mWidth, mHeight, 4, mData, mWidth * 4);
        break;

    case FileFormat::BMP:
        success = stbi_write_bmp(filePath.getCharArray(), mWidth, mHeight, 4, mData);
        break;

    case FileFormat::TGA:
        success = stbi_write_tga(filePath.getCharArray(), mWidth, mHeight, 4, mData);
        break;
    }

    if(! success)
    {
        throw Exception(__FILE__, __LINE__, "Image save failed");
    }
}

void ImageData::save(const File& file, FileFormat format) const
{
    save(file.getPath(), format);
}

}
