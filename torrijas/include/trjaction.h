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

#ifndef TRJ_ACTION_H
#define TRJ_ACTION_H

#include "trjptr.h"

namespace trj
{

class Node;
class RepeatAction;
class SequenceAction;

class Action
{
    friend class Node;
    friend class RepeatAction;
    friend class SequenceAction;

protected:
    static bool checkElapsedTime(float& elapsedTime, float& durationLeft);

    Action() = default;

    virtual bool run(float elapsedTime, Node& node) = 0;

public:
    Action(const Action& other) = delete;
    Action& operator=(const Action& other) = delete;

    virtual ~Action() = default;

    virtual Ptr<Action> getClone() const = 0;

    virtual Ptr<Action> getReversed() const = 0;
};

}

#endif
