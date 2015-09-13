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

#ifndef TRJ_ROTATE_ACTION_H
#define TRJ_ROTATE_ACTION_H

#include "trjaction.h"

namespace trj
{

class RotateAction : public Action
{

protected:
    float mDeltaAngle;
    float mDuration;
    float mDurationLeft;

    RotateAction(float deltaAngle, float duration);

    bool run(float elapsedTime, Node& node) override;

public:
    static Ptr<RotateAction> create(float deltaAngle, float duration)
    {
        return Ptr<RotateAction>(new RotateAction(deltaAngle, duration));
    }

    virtual Ptr<RotateAction> getRotateClone() const
    {
        return Ptr<RotateAction>(new RotateAction(mDeltaAngle, mDuration));
    }

    Ptr<Action> getClone() const override
    {
        return Ptr<Action>(getRotateClone());
    }

    virtual Ptr<RotateAction> getRotateReversed() const
    {
        return Ptr<RotateAction>(new RotateAction(-mDeltaAngle, mDuration));
    }

    Ptr<Action> getReversed() const override
    {
        return Ptr<Action>(getRotateReversed());
    }

    float getDeltaAngle() const noexcept
    {
        return mDeltaAngle;
    }

    float getDuration() const noexcept
    {
        return mDuration;
    }

    float getDurationLeft() const noexcept
    {
        return mDurationLeft;
    }
};

}

#endif
