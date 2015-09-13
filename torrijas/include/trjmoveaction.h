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

#ifndef TRJ_MOVE_ACTION_H
#define TRJ_MOVE_ACTION_H

#include "trjaction.h"
#include "trjpoint.h"

namespace trj
{

class MoveAction : public Action
{

protected:
    Point mDeltaPosition;
    float mDuration;
    float mDurationLeft;

    MoveAction(const Point& deltaPosition, float duration);

    bool run(float elapsedTime, Node& node) override;

public:
    static Ptr<MoveAction> create(const Point& deltaPosition, float duration)
    {
        return Ptr<MoveAction>(new MoveAction(deltaPosition, duration));
    }

    static Ptr<MoveAction> create(float deltaPositionX, float deltaPositionY, float duration)
    {
        return create(Point(deltaPositionX, deltaPositionY), duration);
    }

    virtual Ptr<MoveAction> getMoveClone() const
    {
        return Ptr<MoveAction>(new MoveAction(mDeltaPosition, mDuration));
    }

    Ptr<Action> getClone() const override
    {
        return Ptr<Action>(getMoveClone());
    }

    virtual Ptr<MoveAction> getMoveReversed() const
    {
        Point reversedDeltaPosition(-mDeltaPosition.getX(), -mDeltaPosition.getY());
        return Ptr<MoveAction>(new MoveAction(reversedDeltaPosition, mDuration));
    }

    Ptr<Action> getReversed() const override
    {
        return Ptr<Action>(getMoveReversed());
    }

    const Point& getDeltaPosition() const noexcept
    {
        return mDeltaPosition;
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
