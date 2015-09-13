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

#ifndef TRJ_SCALE_ACTION_H
#define TRJ_SCALE_ACTION_H

#include "trjaction.h"

namespace trj
{

class ScaleAction : public Action
{

protected:
    float mDeltaScaleX;
    float mDeltaScaleY;
    float mDuration;
    float mDurationLeft;

    ScaleAction(float deltaScaleX, float deltaScaleY, float duration);

    bool run(float elapsedTime, Node& node) override;

public:
    static Ptr<ScaleAction> create(float deltaScaleX, float deltaScaleY, float duration)
    {
        return Ptr<ScaleAction>(new ScaleAction(deltaScaleX, deltaScaleY, duration));
    }

    virtual Ptr<ScaleAction> getScaleClone() const
    {
        return Ptr<ScaleAction>(new ScaleAction(mDeltaScaleX, mDeltaScaleY, mDuration));
    }

    Ptr<Action> getClone() const override
    {
        return Ptr<Action>(getScaleClone());
    }

    virtual Ptr<ScaleAction> getScaleReversed() const
    {
        return Ptr<ScaleAction>(new ScaleAction(-mDeltaScaleX, -mDeltaScaleY, mDuration));
    }

    Ptr<Action> getReversed() const override
    {
        return Ptr<Action>(getScaleReversed());
    }

    float getDeltaScaleX() const noexcept
    {
        return mDeltaScaleX;
    }

    float getDeltaScaleY() const noexcept
    {
        return mDeltaScaleY;
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
