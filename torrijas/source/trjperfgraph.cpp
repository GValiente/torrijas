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

#include "trjperfgraph.h"

#include <algorithm>
#include "nanovg.h"

namespace trj
{

PerfGraph::PerfGraph(Style style, String name) noexcept :
    mStyle(style),
    mName(std::move(name)),
    mHead(0)
{
    for(int index = 0; index < kNumHistoryValues; ++index)
    {
        mValues[index] = 0;
    }
}

float PerfGraph::getAverage() const noexcept
{
    float average = 0;
    for(int index = 0; index < kNumHistoryValues; ++index)
    {
        average += mValues[index];
    }

    return average / kNumHistoryValues;
}

void PerfGraph::renderGraph(NVGcontext& nanoVgContext, float x, float y, int fontHandle) const
{
    float average = getAverage();
    float width = 200;
    float height = 35;

    nvgBeginPath(&nanoVgContext);
    nvgRect(&nanoVgContext, x, y, width, height);
    nvgFillColor(&nanoVgContext, nvgRGBA(0, 0, 0, 128));
    nvgFill(&nanoVgContext);

    nvgBeginPath(&nanoVgContext);
    nvgMoveTo(&nanoVgContext, x, y + height);

    if(mStyle == Style::FPS)
    {
        for(int index = 0; index < kNumHistoryValues; ++index)
        {
            float v = std::min(1.0f / (0.00001f + mValues[(mHead + index) % kNumHistoryValues]), 80.0f);
            float vx = x + ((float) index / (kNumHistoryValues - 1)) * width;
            float vy = y + height - ((v / 80.0f) * height);
            nvgLineTo(&nanoVgContext, vx, vy);
        }
    }
    else if(mStyle == Style::PERCENT)
    {
        for(int index = 0; index < kNumHistoryValues; ++index)
        {
            float v = std::min(mValues[(mHead + index) % kNumHistoryValues] * 1.0f, 100.0f);
            float vx = x + ((float) index / (kNumHistoryValues - 1)) * width;
            float vy = y + height - ((v / 100.0f) * height);
            nvgLineTo(&nanoVgContext, vx, vy);
        }
    }
    else
    {
        for(int index = 0; index < kNumHistoryValues; ++index)
        {
            float v = std::min(mValues[(mHead + index) % kNumHistoryValues] * 1000.0f, 20.0f);
            float vx = x + ((float) index / (kNumHistoryValues - 1)) * width;
            float vy = y + height - ((v / 20.0f) * height);
            nvgLineTo(&nanoVgContext, vx, vy);
        }
    }

    nvgLineTo(&nanoVgContext, x + width, y + height);
    nvgFillColor(&nanoVgContext, nvgRGBA(255,192,0,128));
    nvgFill(&nanoVgContext);

    nvgFontFaceId(&nanoVgContext, fontHandle);

    if(! mName.isEmpty())
    {
        nvgFontSize(&nanoVgContext, 14.0f);
        nvgTextAlign(&nanoVgContext, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        nvgFillColor(&nanoVgContext, nvgRGBA(240, 240, 240, 192));
        nvgText(&nanoVgContext, x + 3,y + 1, mName.getCharArray(), NULL);
    }

    nvgFontSize(&nanoVgContext, 18.0f);
    nvgTextAlign(&nanoVgContext,NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
    nvgFillColor(&nanoVgContext, nvgRGBA(240, 240, 240, 255));

    char text[64];
    if(mStyle == Style::FPS)
    {
        sprintf(text, "%.2f FPS", 1.0f / average);
        nvgText(&nanoVgContext, x + width - 3, y + 1, text, NULL);

        nvgFontSize(&nanoVgContext, 15.0f);
        nvgTextAlign(&nanoVgContext, NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM);
        nvgFillColor(&nanoVgContext, nvgRGBA(240, 240, 240, 160));
        sprintf(text, "%.2f ms", average * 1000.0f);
        nvgText(&nanoVgContext, x + width-3, y + height - 1, text, NULL);
    }
    else if(mStyle == Style::PERCENT)
    {
        sprintf(text, "%.1f %%", average * 1.0f);
        nvgText(&nanoVgContext, x + width - 3, y + 1, text, NULL);
    }
    else
    {
        sprintf(text, "%.2f ms", average * 1000.0f);
        nvgText(&nanoVgContext, x + width - 3, y + 1, text, NULL);
    }
}

}
