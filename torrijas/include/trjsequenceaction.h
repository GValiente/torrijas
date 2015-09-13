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

#ifndef TRJ_SEQUENCE_ACTION_H
#define TRJ_SEQUENCE_ACTION_H

#include <vector>
#include "trjaction.h"

namespace trj
{

class SequenceAction : public Action
{

protected:
    std::vector<Ptr<Action>> mActions;
    int mIndex;
    bool mForward;

    SequenceAction(std::vector<Ptr<Action>>&& actions, bool forward);

    bool run(float elapsedTime, Node& node) override;

public:
    static Ptr<SequenceAction> create(std::vector<Ptr<Action>>&& actions)
    {
        return Ptr<SequenceAction>(new SequenceAction(std::move(actions), true));
    }

    static Ptr<SequenceAction> createBackwards(std::vector<Ptr<Action>>&& actions)
    {
        return Ptr<SequenceAction>(new SequenceAction(std::move(actions), false));
    }

    virtual Ptr<SequenceAction> getSequenceClone() const;

    Ptr<Action> getClone() const override
    {
        return Ptr<Action>(getSequenceClone());
    }

    virtual Ptr<SequenceAction> getSequenceReversed() const;

    Ptr<Action> getReversed() const override
    {
        return Ptr<Action>(getSequenceReversed());
    }
};

}

#endif
