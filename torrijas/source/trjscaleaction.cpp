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

#include "trjscaleaction.h"

#include "trjnode.h"
#include "trjdebug.h"

namespace trj
{

ScaleAction::ScaleAction(float deltaScaleX, float deltaScaleY, float duration) :
    mDeltaScaleX(deltaScaleX),
    mDeltaScaleY(deltaScaleY),
    mDuration(duration),
    mDurationLeft(duration)
{
    TRJ_ASSERT(isPositive(duration), "Duration must be grater than 0");
}

bool ScaleAction::run(float elapsedTime, Node& node)
{
    bool done = checkElapsedTime(elapsedTime, mDurationLeft);
    if(! done || elapsedTime > 0)
    {
        float deltaScaleX = (elapsedTime * mDeltaScaleX) / mDuration;
        float finalScaleX = node.getScaleX() + deltaScaleX;
        if(! isPositive(finalScaleX))
        {
            finalScaleX = kEpsilon;
        }

        float deltaScaleY = (elapsedTime * mDeltaScaleY) / mDuration;
        float finalScaleY = node.getScaleY() + deltaScaleY;
        if(! isPositive(finalScaleY))
        {
            finalScaleY = kEpsilon;
        }

        node.setScale(finalScaleX, finalScaleY);
    }

    return done;
}

}
