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

#include "keyboardtest.h"

#include "trjmain.h"
#include "trjkeyboard.h"
#include "trjtextnode.h"

void KeyboardTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        auto& textNode = rootNode.addChild(trj::TextNode::create());
        textNode.setFontSize(60);
        textNode.setHorizontalAlignment(trj::TextNode::HorizontalAlignment::CENTER);
        textNode.addText(0, 0, "");

        setTitle("Keyboard Test");

        while(true)
        {
            trj::String text;
            if(trj::Keyboard::isKeyPressed(trj::Keyboard::Key::K))
            {
                text = "K key is:\npressed";
            }
            else if(trj::Keyboard::isKeyReleased(trj::Keyboard::Key::K))
            {
                text = "K key is:\nreleased";
            }
            else if(trj::Keyboard::isKeyHeld(trj::Keyboard::Key::K))
            {
                text = "K key is:\nheld";
            }
            else
            {
                text = "K key is:\nup";
            }

            float screenWidth = trj::Application::getScreenWidth();
            textNode.setText(0, trj::TextNode::Text(-screenWidth * 0.5f, 0, std::move(text), screenWidth));

            trj::Application::update();
            checkEscapeKey();
        }
    });
}
