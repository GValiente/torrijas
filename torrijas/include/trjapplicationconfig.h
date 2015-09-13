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

#ifndef TRJ_APPLICATION_CONFIG_H
#define TRJ_APPLICATION_CONFIG_H

#include "trjstring.h"

namespace trj
{

class ApplicationConfig
{

protected:
    String mWindowTitle = String("Torrijas");
    String mDefaultFontName = String("sans");
    String mDefaultFontFilePath = String("../../torrijas/fonts/Roboto-Regular.ttf");
    int mScreenWidth = 1280;
    int mScreenHeight = 720;
    float mLogicalScreenHeight = 1000;
    bool mFullScreen = false;
    bool mVSync = true;
    bool mAntialias = true;

public:
    const String& getWindowTitle() const noexcept
    {
        return mWindowTitle;
    }

    void setWindowTitle(String windowTitle) noexcept
    {
        mWindowTitle = std::move(windowTitle);
    }

    const String& getDefaultFontName() const noexcept
    {
        return mDefaultFontName;
    }

    void setDefaultFontName(String defaultFontName) noexcept
    {
        mDefaultFontName = std::move(defaultFontName);
    }

    const String& getDefaultFontFilePath() const noexcept
    {
        return mDefaultFontFilePath;
    }

    void setDefaultFontFilePath(String defaultFontFilePath) noexcept
    {
        mDefaultFontFilePath = std::move(defaultFontFilePath);
    }

    int getScreenWidth() const noexcept
    {
        return mScreenWidth;
    }

    void setScreenWidth(int screenWidth) noexcept
    {
        mScreenWidth = screenWidth;
    }

    int getScreenHeight() const noexcept
    {
        return mScreenHeight;
    }

    void setScreenHeight(int screenHeight) noexcept
    {
        mScreenHeight = screenHeight;
    }

    float getLogicalScreenHeight() const noexcept
    {
        return mLogicalScreenHeight;
    }

    void setLogicalScreenHeight(float logicalScreenHeight) noexcept
    {
        mLogicalScreenHeight = logicalScreenHeight;
    }

    bool isFullScreenEnabled() const noexcept
    {
        return mFullScreen;
    }

    void setFullScreenEnabled(bool fullScreen) noexcept
    {
        mFullScreen = fullScreen;
    }

    bool isVSyncEnabled() const noexcept
    {
        return mVSync;
    }

    void setVSyncEnabled(bool vSync) noexcept
    {
        mVSync = vSync;
    }

    bool isAntialiasEnabled() const noexcept
    {
        return mAntialias;
    }

    void setAntialiasEnabled(bool antialias) noexcept
    {
        mAntialias = antialias;
    }
};

}

#endif
