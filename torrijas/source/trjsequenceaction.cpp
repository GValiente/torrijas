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

#include "trjsequenceaction.h"

#include "trjdebug.h"

namespace trj
{

SequenceAction::SequenceAction(std::vector<Ptr<Action>>&& actions, bool forward) :
    mActions(std::move(actions)),
    mForward(forward)
{
    TRJ_ASSERT(! mActions.empty(), "Actions vector is empty");

    #ifdef TRJ_DEBUG
        for(const auto& action : mActions)
        {
            TRJ_ASSERT(action.get(), "Action is empty");
        }
    #endif

    if(forward)
    {
        mIndex = 0;
    }
    else
    {
        mIndex = mActions.size() - 1;
    }
}

bool SequenceAction::run(float elapsedTime, Node& node)
{
    auto& action = mActions[mIndex];
    if(action->run(elapsedTime, node))
    {
        if(mForward)
        {
            ++mIndex;
            if(mIndex == (int) mActions.size())
            {
                return true;
            }
        }
        else
        {
            --mIndex;
            if(mIndex < 0)
            {
                return true;
            }
        }
    }

    return false;
}

Ptr<SequenceAction> SequenceAction::getSequenceClone() const
{
    std::vector<Ptr<Action>> actions;
    actions.reserve(mActions.size());

    for(const auto& action : mActions)
    {
        actions.push_back(action->getClone());
    }

    return Ptr<SequenceAction>(new SequenceAction(std::move(actions), mForward));
}

Ptr<SequenceAction> SequenceAction::getSequenceReversed() const
{
    std::vector<Ptr<Action>> actions;
    actions.reserve(mActions.size());

    for(const auto& action : mActions)
    {
        actions.push_back(action->getReversed());
    }

    return Ptr<SequenceAction>(new SequenceAction(std::move(actions), ! mForward));
}

}
