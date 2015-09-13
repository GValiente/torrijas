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

#include "trjapplication.h"

#include <sstream>
#include <chrono>
#include <thread>

#ifdef TRJ_CFG_ENABLE_GLEW
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif

#if defined(TRJ_CFG_GLES3)
    #define GLFW_INCLUDE_ES3
#elif defined(TRJ_CFG_GL3)
    #ifdef __APPLE__
        #define GLFW_INCLUDE_GLCOREARB
    #endif
#elif defined(TRJ_CFG_GLES2)
    #define GLFW_INCLUDE_ES2
#endif

#include <GLFW/glfw3.h>

#if defined(TRJ_CFG_GLES3)
    #define NANOVG_GLES3_IMPLEMENTATION
    #ifdef __APPLE__
        #include <OpenGL/glext.h>
    #endif
#elif defined(TRJ_CFG_GL3)
    #define NANOVG_GL3_IMPLEMENTATION
#elif defined(TRJ_CFG_GLES2)
    #define NANOVG_GLES2_IMPLEMENTATION
    #ifdef __APPLE__
        #include <OpenGL/glext.h>
    #endif
#else
    #define NANOVG_GL2_IMPLEMENTATION
#endif

#include "nanovg.h"
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

#include "trjnode.h"
#include "trjfont.h"
#include "trjkeyboard.h"
#include "trjmouse.h"
#include "trjimagedata.h"
#include "trjperfgraph.h"
#include "trjapplicationconfig.h"
#include "trjrendercontext.h"
#include "trjexception.h"
#include "trjdebug.h"
#include "private/trjimagemanager.h"
#include "private/trjdisplaylistmanager.h"

namespace
{
    #ifdef TRJ_CFG_ENABLE_GLEW
        bool smGlewLoaded = false;
    #endif

    void glfwErrorCallback(int error, const char* desc)
    {
        std::stringstream stringStream;
        stringStream << "GLFW error ";
        stringStream << error;
        stringStream << ": ";
        stringStream << desc;

        throw trj::Exception(__FILE__, __LINE__, stringStream.str());
    }
}

namespace trj
{

Application* Application::smInstance = nullptr;

struct Application::Impl
{
    Ptr<Font> font;
    Ptr<Node> node;
    Ptr<Keyboard> keyboard;
    Ptr<Mouse> mouse;
    priv::ImageManager imageManager;
    #ifdef TRJ_CFG_ENABLE_RENDER_CACHE
        Ptr<priv::DisplayListManager> displayListManager;
    #endif
    Color backgroundColor;
    ApplicationConfig config;
    PerfGraph frameTimeGraph;
    PerfGraph cpuGraph;
    GLFWwindow* window = nullptr;
    NVGcontext* context = nullptr;
    double previousTime = 0;
    double cpuPreviousTime = 0;
    double fpsLimitWaitTime = 0;
    float frameTime = kEpsilon;
    float pixelAspectRatio = 0;
    int windowWidth = -1;
    int windowHeight = -1;
    int lastWindowWidth = -1;
    int lastWindowHeight = -1;
    bool showPerformanceGraphs = false;
    bool showBoundingBoxes = false;
    bool glfwLoaded = false;

    explicit Impl(ApplicationConfig config) :
        config(std::move(config)),
        frameTimeGraph(PerfGraph::Style::FPS, "Frame Time"),
        cpuGraph(PerfGraph::Style::MS, "CPU Time")
    {
    }
};

Node& Application::getRootNode() noexcept
{
    return *(smInstance->mImpl->node);
}

Font& Application::getDefaultFont() noexcept
{
    return *(smInstance->mImpl->font);
}

ImageData Application::getScreenshot()
{
    auto impl = smInstance->mImpl;
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(impl->window, &frameBufferWidth, &frameBufferHeight);

    ImageData screenshot(frameBufferWidth, frameBufferHeight, false);
    unsigned char* pixels = screenshot.getData();
    int stride = screenshot.getStride();
    glReadPixels(0, 0, frameBufferWidth, frameBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Set alpha = 255:
    for(int y = 0; y < frameBufferHeight; ++y)
    {
        unsigned char* row = &pixels[y * stride];
        for(int x = 0; x < frameBufferWidth; ++x)
        {
            row[(x * 4) + 3] = 255;
        }
    }

    // Horizontal flip:
    int i = 0;
    int j = frameBufferHeight - 1;
    while(i < j)
    {
        unsigned char* ri = &pixels[i * stride];
        unsigned char* rj = &pixels[j * stride];
        for(int k = 0; k < stride; ++k)
        {
            unsigned char t = ri[k];
            ri[k] = rj[k];
            rj[k] = t;
        }

        i++;
        j--;
    }

    return screenshot;
}

void* Application::getFrameBuffer(Node& node, const Color& backgroundColor, int imageFlags,
        int width, int height, int& imageHandle)
{
    TRJ_ASSERT(width > 0, "Invalid width");
    TRJ_ASSERT(height > 0, "Invalid height");

    auto impl = smInstance->mImpl;
    NVGLUframebuffer* frameBuffer = nvgluCreateFramebuffer(impl->context, width, height, imageFlags);
    if(! frameBuffer)
    {
        throw FrameBufferException(__FILE__, __LINE__);
    }

    nvgluBindFramebuffer(frameBuffer);

    int frameBufferWidth, frameBufferHeight;
    nvgImageSize(impl->context, frameBuffer->image, &frameBufferWidth, &frameBufferHeight);
    imageHandle = frameBuffer->image;

    smInstance->render(node, frameBufferWidth, frameBufferHeight,
            frameBufferWidth / impl->pixelAspectRatio, frameBufferHeight / impl->pixelAspectRatio,
            backgroundColor, false);

    nvgluBindFramebuffer(nullptr);

    return frameBuffer;
}

void Application::deleteFrameBuffer(void* frameBuffer)
{
    nvgluDeleteFramebuffer(static_cast<NVGLUframebuffer*>(frameBuffer));
}

void Application::render(Node& node, int frameBufferWidth, int frameBufferHeight,
        int windowWidth, int windowHeight, const Color& backgroundColor,
        bool renderPerformanceGraphs)
{
    if(frameBufferWidth <= 0 || frameBufferHeight <= 0)
    {
        return;
    }

    TRJ_ASSERT(windowWidth > 0, "Invalid windowWidth");
    TRJ_ASSERT(windowHeight > 0, "Invalid windowHeight");

    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glClearColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(),
            backgroundColor.getAlpha());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    #if defined(TRJ_CFG_GLES2) || defined(TRJ_CFG_GLES3)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
    #endif

    // Calculate pixel ratio for hi-dpi devices:
    nvgBeginFrame(mImpl->context, windowWidth, windowHeight, mImpl->pixelAspectRatio);

    nvgSave(mImpl->context);
    nvgTranslate(mImpl->context, windowWidth * 0.5f, windowHeight * 0.5f);

    float windowScale = windowHeight / getScreenHeight();
    nvgScale(mImpl->context, windowScale, windowScale);

    bool windowWidthChanged = false;
    if(windowWidth != mImpl->lastWindowWidth)
    {
        mImpl->lastWindowWidth = windowWidth;
        windowWidthChanged = true;
    }

    bool windowHeightChanged = false;
    if(windowHeight != mImpl->lastWindowHeight)
    {
        mImpl->lastWindowHeight = windowHeight;
        windowHeightChanged = true;
    }

    RenderContext renderContext(*(mImpl->context), windowWidth, windowHeight, windowWidthChanged,
            windowHeightChanged, mImpl->showBoundingBoxes);
    node.render(renderContext);

    nvgRestore(mImpl->context);

    if(renderPerformanceGraphs)
    {
        int fontHandle = mImpl->font->getHandle();
        mImpl->frameTimeGraph.renderGraph(*(mImpl->context), 5, 5, fontHandle);
        mImpl->cpuGraph.renderGraph(*(mImpl->context), 5 + 200 + 5, 5, fontHandle);
    }

    nvgEndFrame(mImpl->context);

    #if defined(TRJ_CFG_GLES2) || defined(TRJ_CFG_GLES3)
        glEnable(GL_DEPTH_TEST);
    #endif
}

Application::Application() :
    Application(ApplicationConfig())
{
}

Application::Application(ApplicationConfig config) :
    mImpl(new Application::Impl(std::move(config)))
{
    if(smInstance)
    {
        throw Exception(__FILE__, __LINE__, "There's already an Application instance");
    }

    smInstance = this;

    if(glfwInit() == GL_FALSE)
    {
        throw Exception(__FILE__, __LINE__, "GLFW initialization failed");
    }

    mImpl->glfwLoaded = true;

    glfwSetErrorCallback(glfwErrorCallback);

    #if defined(TRJ_CFG_GLES3)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    #elif defined(TRJ_CFG_GL3)
        // Not required on win32, and works with more cards:
        #ifndef _WIN32
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #endif

        #ifdef TRJ_DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        #else
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 0);
        #endif
    #elif defined(TRJ_CFG_GLES2)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    #else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    #endif

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if(! monitor)
    {
        throw Exception(__FILE__, __LINE__, "GLFW monitor not found");
    }

    const ApplicationConfig& appConfig = mImpl->config;
    if(appConfig.isFullScreenEnabled())
    {
        const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
        if(! videoMode)
        {
            throw Exception(__FILE__, __LINE__, "GLFW monitor video mode not found");
        }

        glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

        mImpl->window = glfwCreateWindow(videoMode->width, videoMode->height,
                appConfig.getWindowTitle().getCharArray(), monitor, NULL);
        if(! mImpl->window)
        {
            throw Exception(__FILE__, __LINE__, "GLFW full screen window build failed");
        }
    }
    else
    {
        int screenWidth = appConfig.getScreenWidth();
        int screenHeight = appConfig.getScreenHeight();

        TRJ_ASSERT(screenWidth > 0, "Invalid screenWidth");
        TRJ_ASSERT(screenHeight > 0, "Invalid screenHeight");

        mImpl->window = glfwCreateWindow(screenWidth, screenHeight,
                appConfig.getWindowTitle().getCharArray(), NULL, NULL);
        if(! mImpl->window)
        {
            throw Exception(__FILE__, __LINE__, "GLFW window build failed");
        }
    }

    mImpl->keyboard = new Keyboard(mImpl->window);
    mImpl->mouse = new Mouse(mImpl->window);
    glfwMakeContextCurrent(mImpl->window);

    #ifdef TRJ_CFG_ENABLE_GLEW
        if(! smGlewLoaded)
        {
            if(glewInit() != GLEW_OK)
            {
                throw Exception(__FILE__, __LINE__, "GLEW initialization failed");
            }

            smGlewLoaded = true;
        }
    #endif

    int nanoVgFlags = NVG_STENCIL_STROKES;
    if(appConfig.isAntialiasEnabled())
    {
        nanoVgFlags |= NVG_ANTIALIAS;
    }

    #ifdef TRJ_DEBUG
        nanoVgFlags |= NVG_DEBUG;
    #endif

    #if defined(TRJ_CFG_GLES3)
        mImpl->context = nvgCreateGLES3(nanoVgFlags);
        if(! mImpl->context)
        {
            throw Exception(__FILE__, __LINE__, "NanoVG GLES3 context build failed");
        }
    #elif defined(TRJ_CFG_GL3)
        mImpl->context = nvgCreateGL3(nanoVgFlags);
        if(! mImpl->context)
        {
            throw Exception(__FILE__, __LINE__, "NanoVG GL3 context build failed");
        }
    #elif defined(TRJ_CFG_GLES2)
        mImpl->context = nvgCreateGLES2(nanoVgFlags);
        if(! mImpl->context)
        {
            throw Exception(__FILE__, __LINE__, "NanoVG GLES2 context build failed");
        }
    #else
        mImpl->context = nvgCreateGL2(nanoVgFlags);
        if(! mImpl->context)
        {
            throw Exception(__FILE__, __LINE__, "NanoVG GL2 context build failed");
        }
    #endif

    glfwSwapInterval(appConfig.isVSyncEnabled());

    #ifdef TRJ_CFG_ENABLE_RENDER_CACHE
        mImpl->displayListManager = new priv::DisplayListManager();
    #endif

    mImpl->font = new Font(appConfig.getDefaultFontName(), appConfig.getDefaultFontFilePath());
    mImpl->node = Node::create();

    TRJ_ASSERT(isPositive(getScreenHeight()), "Invalid logical screen height");

    glfwSetTime(0);
    update();
}

Application::~Application()
{
    if(mImpl)
    {
        mImpl->node.reset();
        mImpl->font.reset();

        #ifdef TRJ_CFG_ENABLE_RENDER_CACHE
            mImpl->displayListManager.reset();
        #endif

        mImpl->mouse.reset();
        mImpl->keyboard.reset();

        if(mImpl->context)
        {
            #if defined(TRJ_CFG_GLES3)
                nvgDeleteGLES3(mImpl->context);
            #elif defined(TRJ_CFG_GL3)
                nvgDeleteGL3(mImpl->context);
            #elif defined(TRJ_CFG_GLES2)
                nvgDeleteGLES2(mImpl->context);
            #else
                nvgDeleteGL2(mImpl->context);
            #endif

            mImpl->context = nullptr;
        }

        if(mImpl->window)
        {
            glfwDestroyWindow(mImpl->window);
            mImpl->window = nullptr;
        }

        if(mImpl->glfwLoaded)
        {
            glfwTerminate();
            mImpl->glfwLoaded = false;
        }

        delete mImpl;
        mImpl = nullptr;
    }

    if(smInstance == this)
    {
        smInstance = nullptr;
    }
}

const Color& Application::getBackgroundColor() noexcept
{
    return smInstance->mImpl->backgroundColor;
}

void Application::setBackgroundColor(const Color& color) noexcept
{
    smInstance->mImpl->backgroundColor = color;
}

void Application::setBackgroundColor(float red, float green, float blue, float alpha) noexcept
{
    setBackgroundColor(Color(red, green, blue, alpha));
}

bool Application::showPerformanceGraphs() noexcept
{
    return smInstance->mImpl->showPerformanceGraphs;
}

void Application::setShowPerformanceGraphs(bool show) noexcept
{
    smInstance->mImpl->showPerformanceGraphs = show;
}

const ApplicationConfig& Application::getConfig() noexcept
{
    return smInstance->mImpl->config;
}

float Application::getScreenWidth() noexcept
{
    auto impl = smInstance->mImpl;
    return (impl->windowWidth * getScreenHeight()) / impl->windowHeight;
}

float Application::getScreenHeight() noexcept
{
    return smInstance->mImpl->config.getLogicalScreenHeight();
}

Size Application::getScreenSize() noexcept
{
    return Size(getScreenWidth(), getScreenHeight());
}

Rect Application::getScreenRect() noexcept
{
    Size size = getScreenSize();
    return Rect(Point(-size.getWidth() * 0.5f, -size.getHeight() * 0.5f), size);
}

float Application::getScreenAspectRatio() noexcept
{
    auto impl = smInstance->mImpl;
    return impl->windowWidth / (float) impl->windowHeight;
}

int Application::getRealScreenWidth() noexcept
{
    return smInstance->mImpl->windowWidth;
}

int Application::getRealScreenHeight() noexcept
{
    return smInstance->mImpl->windowHeight;
}

double Application::getElapsedTime()
{
    return glfwGetTime();
}

float Application::getFrameTime() noexcept
{
    return smInstance->mImpl->frameTime;
}

float Application::getFpsLimit() noexcept
{
    double fpsLimitWaitTime = smInstance->mImpl->fpsLimitWaitTime;
    if(fpsLimitWaitTime == 0)
    {
        return 0;
    }

    return 1 / fpsLimitWaitTime;
}

void Application::setFpsLimit(float fpsLimit) noexcept
{
    TRJ_ASSERT(isPositive(fpsLimit), "Invalid FPS limit");

    smInstance->mImpl->fpsLimitWaitTime = 1 / fpsLimit;
}

bool Application::isClosed()
{
    return glfwWindowShouldClose(smInstance->mImpl->window);
}

void Application::setClosed(bool closed)
{
    glfwSetWindowShouldClose(smInstance->mImpl->window, closed);
}

bool Application::showBoundingBoxes() noexcept
{
    return smInstance->mImpl->showBoundingBoxes;
}

void Application::setShowBoundingBoxes(bool show) noexcept
{
    smInstance->mImpl->showBoundingBoxes = show;
}

NVGcontext& Application::getNanoVgContext() noexcept
{
    return *(smInstance->mImpl->context);
}

void Application::update()
{
    auto impl = smInstance->mImpl;
    double time = getElapsedTime();
    impl->frameTime = std::max(time - impl->previousTime, (double) kEpsilon);

    float sleepTime = 0;
    if(impl->frameTime < impl->fpsLimitWaitTime)
    {
        std::uint64_t sleepMicroSeconds = (impl->fpsLimitWaitTime - impl->frameTime) * 1000000;
        std::this_thread::sleep_for(std::chrono::microseconds(sleepMicroSeconds));

        double newTime = getElapsedTime();
        sleepTime = newTime - time;
        time = newTime;
        impl->frameTime = std::max(time - impl->previousTime, (double) kEpsilon);
    }

    impl->node->update(impl->frameTime, impl->frameTime, false);
    impl->previousTime = time;

    glfwGetWindowSize(impl->window, &(impl->windowWidth), &(impl->windowHeight));

    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(impl->window, &frameBufferWidth, &frameBufferHeight);
    impl->pixelAspectRatio = frameBufferWidth / (float) impl->windowWidth;

    smInstance->render(*(impl->node), frameBufferWidth, frameBufferHeight, impl->windowWidth,
            impl->windowHeight, impl->backgroundColor, impl->showPerformanceGraphs);

    impl->frameTimeGraph.update(impl->frameTime);
    impl->cpuGraph.update(getElapsedTime() - impl->cpuPreviousTime - sleepTime);

    glfwSwapBuffers(impl->window);

    impl->cpuPreviousTime = getElapsedTime();
    glfwPollEvents();

    impl->keyboard->update();
    impl->mouse->update(impl->window, impl->windowWidth, impl->windowHeight, getScreenHeight());

    if(glfwWindowShouldClose(impl->window))
    {
        throw ApplicationClosedException(__FILE__, __LINE__);
    }
}

void Application::update(float seconds)
{
    TRJ_ASSERT(isPositive(seconds), "Invalid seconds");

    float elapsedSeconds = 0;

    while(elapsedSeconds < seconds)
    {
        update();

        elapsedSeconds += getFrameTime();
    }
}

}

