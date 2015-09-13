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

#ifndef TRJ_IMAGE_H
#define TRJ_IMAGE_H

#include "trjstring.h"

struct NVGcontext;

namespace trj
{

class ImageData;
class File;
class Node;
class Color;

class Image
{

protected:
    int mHandle = 0;
    int mWidth = 0;
    int mHeight = 0;

    void initSize();

public:
    enum Flag
    {
        GENERATE_MIPMAPS    = 1 << 0, // Generate mipmaps during creation of the image.
        REPEAT_X			= 1 << 1, // Repeat image in X direction.
        REPEAT_Y			= 1 << 2, // Repeat image in Y direction.
        FLIP_Y				= 1 << 3, // Flips (inverses) image in Y direction when rendered.
        PREMULTIPLIED		= 1 << 4, // Image data has premultiplied alpha.
    };

    static void getSize(int imageHandle, int& imageWidth, int& imageHeight);

    static void getSize(NVGcontext& nanoVgContext, int imageHandle, int& imageWidth, int& imageHeight);

    Image(const ImageData& imageData, int flags = REPEAT_X | REPEAT_Y);

    Image(const String& filePath, int flags = REPEAT_X | REPEAT_Y);

    Image(const File& file, int flags = REPEAT_X | REPEAT_Y);

    Image(const unsigned char* compressedData, int compressedDataSize, int flags = REPEAT_X | REPEAT_Y);

    Image(const unsigned char* data, int width, int height, int flags = REPEAT_X | REPEAT_Y);

    Image(Node& node, int height, int flags = REPEAT_X | REPEAT_Y);

    Image(Node& node, int width, int height, int flags = REPEAT_X | REPEAT_Y);

    Image(Node& node, int height, const Color& backgroundColor, int flags = REPEAT_X | REPEAT_Y);

    Image(Node& node, int width, int height, const Color& backgroundColor,
            int flags = REPEAT_X | REPEAT_Y);

    Image(const Image& other);

    Image& operator=(const Image& other);

    Image(Image&& other) noexcept;

    Image& operator=(Image&& other) noexcept;

    ~Image();

    int getHandle() const noexcept
    {
        return mHandle;
    }

    int getWidth() const noexcept
    {
        return mWidth;
    }

    int getHeight() const noexcept
    {
        return mHeight;
    }
};

}

#endif
