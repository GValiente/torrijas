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

#ifndef TRJ_CALL_BACK_ACTION_H
#define TRJ_CALL_BACK_ACTION_H

#include <functional>
#include "trjaction.h"

namespace trj
{

class CallBackAction : public Action
{

protected:
    std::function<void()> mFunction;
    bool mCalled;

    CallBackAction(std::function<void()> function);

    bool run(float elapsedTime, Node& node) override;

public:
    static Ptr<CallBackAction> create(std::function<void()> function)
    {
        return Ptr<CallBackAction>(new CallBackAction(std::move(function)));
    }

    virtual Ptr<CallBackAction> getCallBackClone() const
    {
        return create(mFunction);
    }

    Ptr<Action> getClone() const override
    {
        return Ptr<Action>(create(mFunction));
    }

    virtual Ptr<CallBackAction> getCallBackReversed() const
    {
        return create(mFunction);
    }

    Ptr<Action> getReversed() const override
    {
        return Ptr<Action>(create(mFunction));
    }

    const std::function<void()>& getFunction() const noexcept
    {
        return mFunction;
    }

    std::function<void()>& getFunction() noexcept
    {
        return mFunction;
    }

    bool hasBeenCalled() const noexcept
    {
        return mCalled;
    }
};

}

#endif
