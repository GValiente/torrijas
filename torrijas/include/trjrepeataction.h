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

#ifndef TRJ_REPEAT_ACTION_H
#define TRJ_REPEAT_ACTION_H

#include "trjaction.h"

namespace trj
{

class RepeatAction : public Action
{

protected:
    Ptr<Action> mAction;
    int mInitialTimes;
    int mTimes;

    RepeatAction(Ptr<Action>&& action, int times);

    bool run(float elapsedTime, Node& node) override;

public:
    template<class ActionClass>
    static Ptr<RepeatAction> create(Ptr<ActionClass>&& action, int times = 0)
    {
        return Ptr<RepeatAction>(new RepeatAction(std::move(action), times));
    }

    virtual Ptr<RepeatAction> getRepeatClone() const;

    Ptr<Action> getClone() const override
    {
        return Ptr<Action>(getRepeatClone());
    }

    virtual Ptr<RepeatAction> getRepeatReversed() const;

    Ptr<Action> getReversed() const override
    {
        return Ptr<Action>(getRepeatReversed());
    }

    const Action& getAction() const noexcept
    {
        return *mAction;
    }

    int getInitialTimes() const noexcept
    {
        return mInitialTimes;
    }

    int getTimesLeft() const noexcept
    {
        return mTimes;
    }
};

}

#endif
