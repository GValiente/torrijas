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

#ifndef TRJ_APPLICATION_H
#define TRJ_APPLICATION_H

#include "trjcommon.h"

struct NVGcontext;

namespace trj
{

class Node;
class Font;
class Size;
class Rect;
class Color;
class ImageData;
class ImageManager;
class ApplicationConfig;

namespace priv
{
    class ImageManager;
}

class Application
{
    friend class Node;
    friend class Font;
    friend class ImageData;
    friend class priv::ImageManager;

private:
    struct Impl;
    Impl* mImpl;

protected:
    static Application* smInstance;

    static Node& getRootNode() noexcept;

    static Font& getDefaultFont() noexcept;

    static ImageData getScreenshot();

    static void* getFrameBuffer(Node& node, const Color& backgroundColor, int imageFlags,
            int width, int height, int& imageHandle);

    static void deleteFrameBuffer(void* frameBuffer);

    void render(Node& node, int frameBufferWidth, int frameBufferHeight, int windowWidth,
            int windowHeight, const Color& backgroundColor, bool renderPerformanceGraphs);

public:
    Application();

    explicit Application(ApplicationConfig config);

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    ~Application();

    static const Color& getBackgroundColor() noexcept;

    static void setBackgroundColor(const Color& color) noexcept;

    static void setBackgroundColor(float red, float green, float blue, float alpha = 1) noexcept;

    static bool showPerformanceGraphs() noexcept;

    static void setShowPerformanceGraphs(bool show) noexcept;

    static const ApplicationConfig& getConfig() noexcept;

    static float getScreenWidth() noexcept;

    static float getScreenHeight() noexcept;

    static Size getScreenSize() noexcept;

    static Rect getScreenRect() noexcept;

    static float getScreenAspectRatio() noexcept;

    static int getRealScreenWidth() noexcept;

    static int getRealScreenHeight() noexcept;

    static double getElapsedTime();

    static float getFrameTime() noexcept;

    static float getFpsLimit() noexcept;

    static void setFpsLimit(float fpsLimit) noexcept;

    static bool isClosed();

    static void setClosed(bool closed);

    static bool showBoundingBoxes() noexcept;

    static void setShowBoundingBoxes(bool show) noexcept;

    static NVGcontext& getNanoVgContext() noexcept;

    static void update();

    static void update(float seconds);
};

}

#endif
