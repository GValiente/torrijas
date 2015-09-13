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

#include "torrijotest.h"
#include "transformationstest.h"
#include "actionstest.h"
#include "keyboardtest.h"
#include "mousetest.h"
#include "eyesbenchmark.h"
#include "linestest.h"
#include "filestest.h"
#include "imagestest.h"
#include "texttest.h"
#include "boundingboxtest.h"
#include "framebuffertest.h"

#include "trjaction.h"
#include "trjapplication.h"
#include "trjapplicationconfig.h"
#include "trjarcshape.h"
#include "trjboxgradientpen.h"
#include "trjcallbackaction.h"
#include "trjcolor.h"
#include "trjcolorpen.h"
#include "trjcommon.h"
#include "trjconfig.h"
#include "trjdebug.h"
#include "trjellipseshape.h"
#include "trjexception.h"
#include "trjfile.h"
#include "trjfolder.h"
#include "trjfont.h"
#include "trjimage.h"
#include "trjimagedata.h"
#include "trjimagenode.h"
#include "trjimagepatternpen.h"
#include "trjkeyboard.h"
#include "trjlineargradientpen.h"
#include "trjlineshapes.h"
#include "trjmain.h"
#include "trjmouse.h"
#include "trjmoveaction.h"
#include "trjnode.h"
#include "trjoptional.h"
#include "trjpoint.h"
#include "trjptr.h"
#include "trjradialgradientpen.h"
#include "trjrect.h"
#include "trjrectshape.h"
#include "trjrendercontext.h"
#include "trjrepeataction.h"
#include "trjrotateaction.h"
#include "trjscaleaction.h"
#include "trjsequenceaction.h"
#include "trjshape.h"
#include "trjshapegroup.h"
#include "trjsize.h"
#include "trjstring.h"
#include "trjtextnode.h"
#include "trjwaitaction.h"

int main()
{
    TorrijoTest().run();
    TransformationsTest().run();
    ActionsTest().run();
    KeyboardTest().run();
    MouseTest().run();
    EyesBenchmark().run();
    LinesTest().run();
    FilesTest().run();
    ImagesTest().run();
    TextTest().run();
    BoundingBoxTest().run();
    FrameBufferTest().run();

    return 0;
}
