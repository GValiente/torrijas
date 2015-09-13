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

#ifndef TRJ_PERF_GRAPH_H
#define TRJ_PERF_GRAPH_H

#include "trjstring.h"

struct NVGcontext;

namespace trj
{

class PerfGraph
{

public:
    enum class Style
    {
        FPS,
        MS,
        PERCENT,
    };

protected:
    static constexpr int kNumHistoryValues = 100;

    Style mStyle;
    String mName;
    int mHead;
    float mValues[kNumHistoryValues];

public:
    PerfGraph(Style style, String name) noexcept;

    Style getStyle() const noexcept
    {
        return mStyle;
    }

    const String& getName() const noexcept
    {
        return mName;
    }

    float getAverage() const noexcept;

    void update(float frameTime) noexcept
    {
        mHead = (mHead + 1) % kNumHistoryValues;
        mValues[mHead] = frameTime;
    }

    void renderGraph(NVGcontext& nanoVgContext, float x, float y, int fontHandle) const;
};

}

#endif
