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

#include "framebuffertest.h"

#include "trjmain.h"
#include "trjnode.h"
#include "trjimage.h"
#include "trjimagenode.h"

void FrameBufferTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(getCenterNode());

        auto& testNode = rootNode.addChild(getTestNode());

        float scale = 1;
        float scaleInc = 0.6;
        float rotation = 0;
        float rotationInc = 0.3;
        float skewX = 0;
        float skewXInc = 0.3;
        int numFrameBuffers = 16;
        int frameBufferIndex = numFrameBuffers * 2;
        bool titleSet = false;

        while(true)
        {
            float frameTime = trj::Application::getFrameTime();
            scale += frameTime * scaleInc;
            if(scale > 2)
            {
                scale = 2;
                scaleInc = -scaleInc;
            }
            else if(scale < 1)
            {
                scale = 1;
                scaleInc = -scaleInc;
            }

            rotation += frameTime * rotationInc;
            if(rotation > trj::kPi)
            {
                rotation = trj::kPi;
                rotationInc = -rotationInc;
            }
            else if(rotation < 0)
            {
                rotation = 0;
                rotationInc = -rotationInc;
            }

            skewX += frameTime * skewXInc;
            if(skewX > 1)
            {
                skewX = 1;
                skewXInc = -skewXInc;
            }
            else if(skewX < 0)
            {
                skewX = 0;
                skewXInc = -skewXInc;
            }

            testNode.setScale(scale);
            testNode.setRotationAngle(rotation);
            testNode.setSkewXAngle(skewX);

            try
            {
                if(numFrameBuffers > 0)
                {
                    trj::Image frameBufferImage(testNode, trj::Application::getRealScreenHeight());
                    auto& frameBufferNode = rootNode.addChild(
                            trj::ImageNode::create(trj::Application::getScreenHeight(), frameBufferImage));
                    frameBufferNode.setOpacity(0.1);
                    frameBufferNode.setTag(frameBufferIndex);

                    auto oldFrameBufferNode = rootNode.findChild(frameBufferIndex - numFrameBuffers);
                    if(oldFrameBufferNode)
                    {
                        rootNode.removeChild(*oldFrameBufferNode);
                    }

                    ++frameBufferIndex;
                }

                if(! titleSet)
                {
                    setTitle("FrameBuffer Test");
                    titleSet = true;
                }
            }
            catch(const trj::FrameBufferException& exception)
            {
                numFrameBuffers = 0;

                if(! titleSet)
                {
                    setTitle("FrameBuffer Test", exception.getMessage());
                    titleSet = true;
                }
            }

            trj::Application::update();
            checkEscapeKey();
        }
    });
}
