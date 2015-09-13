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

#include "trjrotateaction.h"

#include "trjnode.h"
#include "trjdebug.h"

namespace trj
{

RotateAction::RotateAction(float deltaAngle, float duration) :
    mDeltaAngle(deltaAngle),
    mDuration(duration),
    mDurationLeft(duration)
{
    TRJ_ASSERT(deltaAngle >= -(2 * kPi) && deltaAngle < 2 * kPi, "Invalid delta angle");
    TRJ_ASSERT(isPositive(duration), "Duration must be grater than 0");
}

bool RotateAction::run(float elapsedTime, Node& node)
{
    bool done = checkElapsedTime(elapsedTime, mDurationLeft);
    if(! done || elapsedTime > 0)
    {
        float deltaAngle = (elapsedTime * mDeltaAngle) / mDuration;
        float finalAngle = node.getRotationAngle() + deltaAngle;

        if(finalAngle < 0)
        {
            finalAngle += 2 * kPi;
        }
        else if(finalAngle >= 2 * kPi)
        {
            finalAngle -= 2 * kPi;
        }

        node.setRotationAngle(finalAngle);
    }

    return done;
}

}
