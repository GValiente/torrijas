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

#include "trjfont.h"

#include "nanovg.h"
#include "trjapplication.h"
#include "trjfile.h"
#include "trjexception.h"
#include "trjdebug.h"

namespace trj
{

const Font& Font::getDefaultFont() noexcept
{
    return Application::getDefaultFont();
}

void Font::setDefaultFont(const Font& font) noexcept
{
    Application::getDefaultFont() = font;
}

int Font::getFontHandle(const String& name)
{
    NVGcontext& nanoVgContext = Application::getNanoVgContext();
    int handle = nvgFindFont(&nanoVgContext, name.getCharArray());
    if(handle < 0)
    {
        throw Exception(__FILE__, __LINE__, "Font load failed");
    }

    return handle;
}

Font::Font(String name, const String& filePath) :
    mName(std::move(name))
{
    NVGcontext& nanoVgContext = Application::getNanoVgContext();
    mHandle = nvgCreateFont(&nanoVgContext, mName.getCharArray(), filePath.getCharArray());
    if(mHandle < 0)
    {
        throw Exception(__FILE__, __LINE__, "Font load failed");
    }
}

Font::Font(String name, const File& file) :
    Font(std::move(name), file.getPath())
{
}

Font::Font(String name, unsigned char* data, int dataSize, bool freeData) :
    mName(std::move(name))
{
    TRJ_ASSERT(data, "Data is null");
    TRJ_ASSERT(dataSize > 0, "Invalid data size");

    NVGcontext& nanoVgContext = Application::getNanoVgContext();
    mHandle = nvgCreateFontMem(&nanoVgContext, mName.getCharArray(), data, dataSize, freeData);
    if(! mHandle)
    {
        throw Exception(__FILE__, __LINE__, "Font load failed");
    }
}

Font::Font(String name) :
    mName(std::move(name)),
    mHandle(getFontHandle(mName))
{
}

}
