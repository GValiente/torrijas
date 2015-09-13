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

#include "trjmouse.h"

#include <GLFW/glfw3.h>
#include "trjdebug.h"

namespace trj
{

namespace
{
    float smScrollXOffset = 0;
    float smScrollYOffset = 0;

    void glfwScrollCallback(GLFWwindow*, double xOffset, double yOffset)
    {
        smScrollXOffset += xOffset;
        smScrollYOffset += yOffset;
    }
}

Mouse* Mouse::smInstance = nullptr;

void Mouse::Button::update(int state)
{
    if(state == GLFW_PRESS)
    {
        mPressed = ! mHeld;
        mHeld = true;
        mReleased = false;
    }
    else
    {
        mReleased = mHeld;
        mHeld = false;
        mPressed = false;
    }
}

Mouse::Mouse(void* window)
{
    TRJ_ASSERT(window, "Window is null");

    smInstance = this;

    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window);
    glfwSetScrollCallback(glfwWindow, glfwScrollCallback);
}

void Mouse::update(void* window, int windowWidth, int windowHeight, int logicalWindowHeight)
{
    if(windowWidth <= 0 || windowHeight <= 0)
    {
        return;
    }

    TRJ_ASSERT(window, "Window is null");
    TRJ_ASSERT(logicalWindowHeight > 0, "Invalid logical window height");

    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window);
    double realPositionX, realPositionY;
    glfwGetCursorPos(glfwWindow, &realPositionX, &realPositionY);
    mRealPosition = Point(realPositionX, realPositionY);

    float positionX = realPositionX - (windowWidth / 2);
    float positionY = realPositionY - (windowHeight / 2);
    float scale = logicalWindowHeight / (float) windowHeight;
    mPosition = Point(positionX * scale, positionY * scale);

    for(int index = 0, limit = mButtons.size(); index < limit; ++index)
    {
        Button& button = mButtons[index];
        button.update(glfwGetMouseButton(glfwWindow, index));
    }

    mWheelXOffset = smScrollXOffset;
    smScrollXOffset = 0;

    mWheelYOffset = smScrollYOffset;
    smScrollYOffset = 0;
}

Mouse::~Mouse()
{
    smInstance = nullptr;
    smScrollXOffset = 0;
    smScrollYOffset = 0;
}

}
