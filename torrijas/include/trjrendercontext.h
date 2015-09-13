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

#ifndef TRJ_RENDER_CONTEXT_H
#define TRJ_RENDER_CONTEXT_H

#include <vector>
#include "trjrect.h"
#include "trjcolor.h"

struct NVGcontext;

namespace trj
{

class RenderContext
{

protected:
    Rect mWindowRect;
    std::vector<std::pair<Color, float>> mBlendColors;
    NVGcontext& mNanoVgContext;
    float mAspectRatio;
    float mFinalScaleX;
    float mFinalScaleY;
    float mOpacity;
    bool mWindowWidthChanged;
    bool mWindowHeightChanged;
    bool mScissorEnabled;
    bool mShowBoundingBoxes;
    bool mRenderOffScreen;
    bool mInvalidateFinalBoundingBoxes;

public:
    RenderContext(NVGcontext& nanoVgContext, int windowWidth, int windowHeight,
            bool windowWidthChanged, bool windowHeightChanged, bool showBoundingBoxes) noexcept :
        mWindowRect(0, 0, windowWidth, windowHeight),
        mNanoVgContext(nanoVgContext),
        mAspectRatio(windowWidth / (float) windowHeight),
        mFinalScaleX(1),
        mFinalScaleY(1),
        mOpacity(1),
        mWindowWidthChanged(windowWidthChanged),
        mWindowHeightChanged(windowHeightChanged),
        mScissorEnabled(false),
        mShowBoundingBoxes(showBoundingBoxes),
        mRenderOffScreen(false),
        mInvalidateFinalBoundingBoxes(false)
    {
    }

    NVGcontext& getNanoVgContext() noexcept
    {
        return mNanoVgContext;
    }

    const Rect& getWindowRect() const noexcept
    {
        return mWindowRect;
    }

    float getAspectRatio() const noexcept
    {
        return mAspectRatio;
    }

    float getFinalScaleX() const noexcept
    {
        return mFinalScaleX;
    }

    void setFinalScaleX(float finalScaleX) noexcept
    {
        mFinalScaleX = finalScaleX;
    }

    float getFinalScaleY() const noexcept
    {
        return mFinalScaleY;
    }

    void setFinalScaleY(float finalScaleY) noexcept
    {
        mFinalScaleY = finalScaleY;
    }

    float getOpacity() const noexcept
    {
        return mOpacity;
    }

    void setOpacity(float opacity) noexcept
    {
        mOpacity = opacity;
    }

    bool windowWidthChanged() const noexcept
    {
        return mWindowWidthChanged;
    }

    bool windowHeightChanged() const noexcept
    {
        return mWindowHeightChanged;
    }

    bool windowSizeChanged() const noexcept
    {
        return windowWidthChanged() || windowHeightChanged();
    }

    bool isScissorEnabled() const noexcept
    {
        return mScissorEnabled;
    }

    void setScissorEnabled(bool enabled) noexcept
    {
        mScissorEnabled = enabled;
    }

    bool showBoundingBoxes() const noexcept
    {
        return mShowBoundingBoxes;
    }

    void setShowBoundingBoxes(bool show) noexcept
    {
        mShowBoundingBoxes = show;
    }

    bool renderOffScreen() const noexcept
    {
        return mRenderOffScreen;
    }

    void setRenderOffScreen(bool renderOffScreen) noexcept
    {
        mRenderOffScreen = renderOffScreen;
    }

    bool invalidateFinalBoundingBoxes() const noexcept
    {
        return mInvalidateFinalBoundingBoxes;
    }

    void setInvalidateFinalBoundingBoxes(bool invalidate) noexcept
    {
        mInvalidateFinalBoundingBoxes = invalidate;
    }

    const std::vector<std::pair<Color, float>>& getBlendColors() const noexcept
    {
        return mBlendColors;
    }

    std::pair<Color, float> getBlendResult() const noexcept
    {
        return Color::getBlendResult(mBlendColors);
    }

    void pushBlendColor(const Color& blendColor, float blendFactor)
    {
        mBlendColors.push_back(std::make_pair(blendColor, blendFactor));
    }

    void popBlendColor() noexcept
    {
        mBlendColors.pop_back();
    }
};

}
#endif
