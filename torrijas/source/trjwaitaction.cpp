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

#include "trjwaitaction.h"

#include "trjnode.h"
#include "trjdebug.h"

namespace trj
{

WaitAction::WaitAction(float duration) :
    mDuration(duration),
    mDurationLeft(duration)
{
    TRJ_ASSERT(isPositive(duration), "Invalid duration");
}

bool WaitAction::run(float elapsedTime, Node&)
{
    return checkElapsedTime(elapsedTime, mDurationLeft);
}

Ptr<WaitAction> WaitAction::create(float duration)
{
    return Ptr<WaitAction>(new WaitAction(duration));
}

Ptr<WaitAction> WaitAction::getWaitClone() const
{
    return Ptr<WaitAction>(new WaitAction(mDuration));
}

Ptr<WaitAction> WaitAction::getWaitReversed() const
{
    return Ptr<WaitAction>(new WaitAction(mDuration));
}

}
