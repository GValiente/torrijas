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

#include "texttest.h"

#include "trjmain.h"
#include "trjnode.h"
#include "trjfont.h"
#include "trjtextnode.h"

void TextTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(getCenterNode());

        trj::Font font("sans", "../../torrijas/nanovg/example/Roboto-Regular.ttf");
        auto textNode = trj::TextNode::create(font);
        textNode->setFontSize(100);
        textNode->setFontLetterSpacing(0.5);
        textNode->setFontLineHeight(0.75);
        textNode->setScale(0.75);
        textNode->setFontBlur(0.5);
        textNode->setRotationAngle((2 * trj::kPi) - 0.2);
        textNode->addText(-100, -100, "TORRIJAS!");
        textNode->addText(100, 100, "TORRIJAAAAAAS!", 250);
        rootNode.addChild(std::move(textNode));

        setTitle("Text Test");

        while(true)
        {
            trj::Application::update();
            checkEscapeKey();
        }
    });
}
