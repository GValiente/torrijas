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

#include "trjimage.h"

#include "nanovg.h"
#include "trjfile.h"
#include "trjcolor.h"
#include "trjapplication.h"
#include "trjimagedata.h"
#include "trjdebug.h"
#include "private/trjimagemanager.h"

namespace trj
{

void Image::initSize()
{
    int imageWidth, imageHeight;
    getSize(mHandle, imageWidth, imageHeight);
    mWidth = imageWidth;
    mHeight = imageHeight;
}

void Image::getSize(int imageHandle, int& imageWidth, int& imageHeight)
{
    NVGcontext& nanoVgContext = Application::getNanoVgContext();
    getSize(nanoVgContext, imageHandle, imageWidth, imageHeight);
}

void Image::getSize(NVGcontext& nanoVgContext, int imageHandle, int& imageWidth, int& imageHeight)
{
    nvgImageSize(&nanoVgContext, imageHandle, &imageWidth, &imageHeight);
}

Image::Image(const ImageData& imageData, int flags) :
    mHandle(priv::ImageManager::addImage(imageData, flags))
{
    initSize();
}

Image::Image(const String& filePath, int flags) :
    Image(ImageData(filePath), flags)
{
}

Image::Image(const File& file, int flags) :
    Image(ImageData(file), flags)
{
}

Image::Image(const unsigned char* compressedData, int compressedDataSize, int flags) :
    Image(ImageData(compressedData, compressedDataSize), flags)
{
}

Image::Image(const unsigned char* data, int width, int height, int flags) :
    mHandle(priv::ImageManager::addImage(data, width, height, flags))
{
    initSize();
}

Image::Image(Node& node, int height, int flags) :
    Image(node, height * Application::getScreenAspectRatio(), height, flags)
{
}

Image::Image(Node& node, int width, int height, int flags) :
    Image(node, width, height, Color(0, 0, 0, 0), flags)
{
}

Image::Image(Node& node, int height, const Color& backgroundColor, int flags) :
    Image(node, height * Application::getScreenAspectRatio(), height, backgroundColor, flags)
{
}

Image::Image(Node& node, int width, int height, const Color& backgroundColor, int flags) :
    mHandle(priv::ImageManager::addImage(node, width, height, backgroundColor, flags))
{
    initSize();
}

Image::Image(const Image& other) :
    mHandle(other.mHandle),
    mWidth(other.mWidth),
    mHeight(other.mHeight)
{
    priv::ImageManager::addImageRef(mHandle);
}

Image& Image::operator=(const Image& other)
{
    if(this != &other)
    {
        mHandle = other.mHandle;
        mWidth = other.mWidth;
        mHeight = other.mHeight;

        priv::ImageManager::addImageRef(mHandle);
    }

    return *this;
}

Image::Image(Image&& other) noexcept :
    mHandle(other.mHandle),
    mWidth(other.mWidth),
    mHeight(other.mHeight)
{
    other.mHandle = 0;
    other.mWidth = 0;
    other.mHeight = 0;
}

Image& Image::operator=(Image&& other) noexcept
{
    if(this != &other)
    {
        mHandle = other.mHandle;
        mWidth = other.mWidth;
        mHeight = other.mHeight;

        other.mHandle = 0;
        other.mWidth = 0;
        other.mHeight = 0;
    }

    return *this;
}

Image::~Image()
{
    if(mHandle)
    {
        priv::ImageManager::removeImageRef(mHandle);
        mHandle = 0;
    }
}

}
