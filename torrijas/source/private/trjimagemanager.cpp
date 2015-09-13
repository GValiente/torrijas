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

#include "private/trjimagemanager.h"

#include "nanovg.h"
#include "stb_image.h"
#include "trjapplication.h"
#include "trjimagedata.h"
#include "trjexception.h"
#include "trjdebug.h"

namespace trj
{

namespace priv
{

ImageManager* ImageManager::smInstance = nullptr;

ImageManager::ImageManager()
{
    smInstance = this;

    stbi_set_unpremultiply_on_load(true);
    stbi_convert_iphone_png_to_rgb(true);
}

ImageManager::~ImageManager()
{
    TRJ_ASSERT(mEntries.empty(), "ImageManager still contains images");

    smInstance = nullptr;
}

int ImageManager::addImage(const ImageData& imageData, int flags)
{
    return addImage(imageData.getData(), imageData.getWidth(), imageData.getHeight(), flags);
}

int ImageManager::addImage(const unsigned char* data, int width, int height, int flags)
{
    TRJ_ASSERT(data, "Data is null");
    TRJ_ASSERT(width > 0, "Invalid width");
    TRJ_ASSERT(height > 0, "Invalid height");

    NVGcontext& nanoVgContext = Application::getNanoVgContext();
    int image = nvgCreateImageRGBA(&nanoVgContext, width, height, flags, data);
    if(! image)
    {
        throw Exception(__FILE__, __LINE__, "Image load failed");
    }

    smInstance->mEntries.insert(std::make_pair(image, Entry{nullptr, 1}));
    return image;
}

int ImageManager::addImage(Node& node, int width, int height, const Color& backgroundColor, int flags)
{
    TRJ_ASSERT(width > 0, "Invalid width");
    TRJ_ASSERT(height > 0, "Invalid height");

    int image;
    void* frameBuffer = Application::getFrameBuffer(node, backgroundColor, flags, width, height, image);

    smInstance->mEntries.insert(std::make_pair(image, Entry{frameBuffer, 1}));
    return image;
}

void ImageManager::addImageRef(int image)
{
    auto it = smInstance->mEntries.find(image);

    TRJ_ASSERT(it != smInstance->mEntries.end(), "Image doesn't exist");

    Entry& entry = it->second;
    entry.count++;
}

void ImageManager::removeImageRef(int image)
{
    auto it = smInstance->mEntries.find(image);

    TRJ_ASSERT(it != smInstance->mEntries.end(), "Image doesn't exist");

    Entry& entry = it->second;
    entry.count--;

    if(! entry.count)
    {
        if(entry.frameBuffer)
        {
            Application::deleteFrameBuffer(entry.frameBuffer);
        }
        else
        {
            NVGcontext& nanoVgContext = Application::getNanoVgContext();
            nvgDeleteImage(&nanoVgContext, it->first);
        }

        smInstance->mEntries.erase(it);
    }
}

}

}
