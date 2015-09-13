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

#ifndef TRJ_MOUSE_H
#define TRJ_MOUSE_H

#include <array>
#include "trjpoint.h"

namespace trj
{

class Application;

class Mouse
{
    friend class Application;

public:
    class Button
    {
        friend class Mouse;

    protected:
        bool mPressed = false;
        bool mReleased = false;
        bool mHeld = false;

        void update(int state);

    public:
        bool isPressed() const noexcept
        {
            return mPressed;
        }

        bool isReleased() const noexcept
        {
            return mReleased;
        }

        bool isHeld() const noexcept
        {
            return mHeld;
        }
    };

protected:
    static Mouse* smInstance;

    Point mPosition;
    Point mRealPosition;
    float mWheelXOffset = 0;
    float mWheelYOffset = 0;
    std::array<Button, 8> mButtons;

    explicit Mouse(void* window);

    void update(void* window, int windowWidth, int windowHeight, int logicalWindowHeight);

public:
    Mouse(const Mouse& other) = delete;
    Mouse& operator=(const Mouse& other) = delete;

    ~Mouse();

    static const Point& getPosition() noexcept
    {
        return smInstance->mPosition;
    }

    static const Point& getRealPosition() noexcept
    {
        return smInstance->mRealPosition;
    }

    static const Button& getLeftButton() noexcept
    {
        return smInstance->mButtons[0];
    }

    static const Button& getRightButton() noexcept
    {
        return smInstance->mButtons[1];
    }

    static const Button& getMiddleButton() noexcept
    {
        return smInstance->mButtons[2];
    }

    static const std::array<Button, 8>& getButtons() noexcept
    {
        return smInstance->mButtons;
    }

    static float getWheelXOffset() noexcept
    {
        return smInstance->mWheelXOffset;
    }

    static float getWheelYOffset() noexcept
    {
        return smInstance->mWheelYOffset;
    }
};

}

#endif
