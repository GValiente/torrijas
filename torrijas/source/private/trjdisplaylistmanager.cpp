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

#include "private/trjdisplaylistmanager.h"

#include "nanovg.h"
#include "trjdebug.h"

namespace trj
{

namespace priv
{

DisplayListManager* DisplayListManager::smInstance = nullptr;

DisplayListManager::~DisplayListManager()
{
    #ifdef TRJ_CFG_ENABLE_RENDER_CACHE
        for(NVGdisplayList* displayList : mDisplayLists)
        {
            nvgDeleteDisplayList(displayList);
        }
    #endif

    smInstance = nullptr;
}

#ifdef TRJ_CFG_ENABLE_RENDER_CACHE

NVGdisplayList& DisplayListManager::pull()
{
    auto& displayLists = smInstance->mDisplayLists;
    NVGdisplayList* displayList;
    if(displayLists.empty())
    {
        displayList = nvgCreateDisplayList(-1);
        TRJ_ASSERT(displayList, "Display list build failed");
    }
    else
    {
        displayList = displayLists.back();
        displayLists.pop_back();
    }

    return *displayList;
}

void DisplayListManager::push(NVGdisplayList& displayList)
{
    nvgResetDisplayList(&displayList);
    smInstance->mDisplayLists.push_back(&displayList);
}

#endif

}

}
