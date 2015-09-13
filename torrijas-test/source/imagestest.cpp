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

#include "imagestest.h"

#include "trjmain.h"
#include "trjnode.h"
#include "trjimage.h"
#include "trjfile.h"
#include "trjimagepatternpen.h"
#include "trjrectshape.h"
#include "trjimagenode.h"

void ImagesTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);
        trj::Application::setShowBoundingBoxes(true);

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(getCenterNode());

        trj::Image torrijaImage("../../torrijas-test/images/torrija.jpg");
        trj::Image spritesImage("../../torrijas-test/images/sprites.png", 0);

        trj::ShapeGroup imageShapeGroup(trj::ImagePatternPen(torrijaImage, 0, 0, 200, 200,
                3.1415f * 0.25f, 0.5f));
        float height = trj::Application::getScreenHeight();
        imageShapeGroup.addShape(trj::RectShape(-height / 2, -height / 2, height, height));

        auto& backgroundNode = rootNode.addNewChild();
        backgroundNode.addShapeGroup(std::move(imageShapeGroup));
        backgroundNode.setScaleWithScreenAspectRatio(true);

        auto& spritesNode = rootNode.addChild(trj::ImageNode::create(400, spritesImage));
        spritesNode.setPosition(-500, -250);

        auto spriteNode = trj::ImageNode::create(400, spritesImage, trj::Rect(0, 0, 135, 107));
        spriteNode->setPosition(500, -250);
        rootNode.addChild(std::move(spriteNode));

        spriteNode = trj::ImageNode::create(400, spritesImage, trj::Rect(65, 60, 135, 107));
        spriteNode->setPosition(-500, 250);
        rootNode.addChild(std::move(spriteNode));

        spriteNode = trj::ImageNode::create(200, spritesImage, trj::Rect(65, 60, 60, 50));
        spriteNode->setPosition(500, 250);
        rootNode.addChild(std::move(spriteNode));

        spriteNode = trj::ImageNode::create(200, spritesImage, trj::Rect(225, 137, 40, 45));
        spriteNode->setBlendColor(1.0f, 0.0f, 0.0f, 0.5f);
        rootNode.addChild(std::move(spriteNode));

        setTitle("Images Test");

        while(true)
        {
            trj::Application::update();
            checkEscapeKey();
        }
    });
}
