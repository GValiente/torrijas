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

#include "trjrepeataction.h"

#include "trjdebug.h"

namespace trj
{

RepeatAction::RepeatAction(Ptr<Action>&& action, int times) :
    mAction(std::move(action)),
    mInitialTimes(times),
    mTimes(times)
{
    TRJ_ASSERT(mAction.get(), "Action is empty");
    TRJ_ASSERT(times >= 0, "Invalid times");
}

bool RepeatAction::run(float elapsedTime, Node& node)
{
    if(mAction->run(elapsedTime, node))
    {
        bool repeat;
        if(mInitialTimes > 0)
        {
            --mTimes;
            repeat = (mTimes > 0);
        }
        else
        {
            repeat = true;
        }

        if(repeat)
        {
            mAction = mAction->getClone();
        }

        return (! repeat);
    }

    return false;
}

Ptr<RepeatAction> RepeatAction::getRepeatClone() const
{
    return Ptr<RepeatAction>(new RepeatAction(mAction->getClone(), mInitialTimes));
}

Ptr<RepeatAction> RepeatAction::getRepeatReversed() const
{
    return Ptr<RepeatAction>(new RepeatAction(mAction->getReversed(), mInitialTimes));
}

}
