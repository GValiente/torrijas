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

#include "trjkeyboard.h"

#include <algorithm>
#include <GLFW/glfw3.h>
#include "trjdebug.h"

namespace trj
{

namespace
{
    std::vector<Keyboard::KeyObject> smNextPressedKeys;
    std::vector<Keyboard::KeyObject> smNextReleasedKeys;
    std::vector<Keyboard::KeyObject> smNextRepeatingKeys;

    void glfwKeyCallback(GLFWwindow*, int key, int, int action, int mods)
    {
        Keyboard::KeyObject keyObject(static_cast<Keyboard::Key>(key), mods);

        if(action == GLFW_PRESS)
        {
            smNextPressedKeys.push_back(keyObject);
        }
        else if(action == GLFW_RELEASE)
        {
            smNextReleasedKeys.push_back(keyObject);
        }
        else if(action == GLFW_REPEAT)
        {
            smNextRepeatingKeys.push_back(keyObject);
        }
    }
}

Keyboard* Keyboard::smInstance = nullptr;

Keyboard::Keyboard(void* window)
{
    TRJ_ASSERT(window, "Window is null");

    smInstance = this;

    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window);
    glfwSetKeyCallback(glfwWindow, glfwKeyCallback);
}

void Keyboard::update()
{
    mPressedKeys.clear();
    mPressedKeys.swap(smNextPressedKeys);

    mReleasedKeys.clear();
    mReleasedKeys.swap(smNextReleasedKeys);

    mRepeatingKeys.clear();
    mRepeatingKeys.swap(smNextRepeatingKeys);

    for(const KeyObject& key : mPressedKeys)
    {
        auto it = std::find(mHeldKeys.begin(), mHeldKeys.end(), key);
        if(it == mHeldKeys.end())
        {
            mHeldKeys.push_back(key);
        }
    }

    for(const KeyObject& key : mReleasedKeys)
    {
        auto it = std::find(mHeldKeys.begin(), mHeldKeys.end(), key);
        if(it != mHeldKeys.end())
        {
            mHeldKeys.erase(it);
        }
    }
}

Keyboard::~Keyboard()
{
    smInstance = nullptr;

    std::vector<KeyObject>().swap(smNextPressedKeys);
    std::vector<KeyObject>().swap(smNextReleasedKeys);
    std::vector<KeyObject>().swap(smNextRepeatingKeys);
}

const Keyboard::KeyObject* Keyboard::getHeldKey() noexcept
{
    const auto& keys = smInstance->mHeldKeys;
    return keys.empty() ? nullptr : &(keys.front());
}

const Keyboard::KeyObject* Keyboard::getPressedKey() noexcept
{
    const auto& keys = smInstance->mPressedKeys;
    return keys.empty() ? nullptr : &(keys.front());
}

const Keyboard::KeyObject* Keyboard::getReleasedKey() noexcept
{
    const auto& keys = smInstance->mReleasedKeys;
    return keys.empty() ? nullptr : &(keys.front());
}

const Keyboard::KeyObject* Keyboard::getRepeatingKey() noexcept
{
    const auto& keys = smInstance->mRepeatingKeys;
    return keys.empty() ? nullptr : &(keys.front());
}

bool Keyboard::isKeyHeld(const KeyObject& key) noexcept
{
    const auto& keys = smInstance->mHeldKeys;
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool Keyboard::isKeyPressed(const KeyObject& key) noexcept
{
    const auto& keys = smInstance->mPressedKeys;
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool Keyboard::isKeyReleased(const KeyObject& key) noexcept
{
    const auto& keys = smInstance->mReleasedKeys;
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool Keyboard::isKeyRepeating(const KeyObject& key) noexcept
{
    const auto& keys = smInstance->mRepeatingKeys;
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

}
