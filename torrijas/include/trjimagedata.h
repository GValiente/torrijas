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

#ifndef TRJ_IMAGE_DATA_H
#define TRJ_IMAGE_DATA_H

#include "trjstring.h"

namespace trj
{

class File;
class Color;

class ImageData
{

public:
    enum class FileFormat
    {
        PNG,
        BMP,
        TGA
    };

protected:
    unsigned char* mData = nullptr;
    int mWidth = 0;
    int mHeight = 0;

public:
    static ImageData getScreenshot();

    ImageData(int width, int height, bool clear = true);

    ImageData(const unsigned char* data, int width, int height);

    ImageData(const String& filePath);

    ImageData(const File& file);

    ImageData(const unsigned char* compressedData, int compressedDataSize);

    ImageData(const ImageData& other) = delete;
    ImageData& operator=(const ImageData& other) = delete;

    ImageData(ImageData&& other) noexcept;

    ImageData& operator=(ImageData&& other) noexcept;

    ~ImageData();

    ImageData getClone() const
    {
        return ImageData(mData, mWidth, mHeight);
    }

    const unsigned char* getData() const noexcept
    {
        return mData;
    }

    unsigned char* getData() noexcept
    {
        return mData;
    }

    int getWidth() const noexcept
    {
        return mWidth;
    }

    int getHeight() const noexcept
    {
        return mHeight;
    }

    int getNumBytes() const noexcept
    {
        return mWidth * mHeight * 4;
    }

    int getStride() const noexcept
    {
        return mWidth * 4;
    }

    Color getColor(int x, int y) const noexcept;

    void setColor(int x, int y, const Color& color) noexcept;

    void setColor(int x, int y, unsigned char red, unsigned char green, unsigned char blue,
            unsigned char alpha = 255) noexcept;

    void save(const String& filePath, FileFormat format = FileFormat::PNG) const;

    void save(const File& file, FileFormat format = FileFormat::PNG) const;
};

}

#endif
