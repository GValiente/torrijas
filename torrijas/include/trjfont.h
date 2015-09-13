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

#ifndef TRJ_FONT_H
#define TRJ_FONT_H

#include "trjstring.h"

namespace trj
{

class File;

class Font
{

protected:
    String mName;
    int mHandle = 0;

public:
    static const Font& getDefaultFont() noexcept;

    static void setDefaultFont(const Font& font) noexcept;

    static int getFontHandle(const String& name);

    Font(String name, const String& filePath);

    Font(String name, const File& file);

    Font(String name, unsigned char* data, int dataSize, bool freeData);

    Font(String name);

    const String& getName() const noexcept
    {
        return mName;
    }

    int getHandle() const noexcept
    {
        return mHandle;
    }
};

}

#endif
