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

#include "mousetest.h"

#include <cmath>
#include "trjmain.h"
#include "trjkeyboard.h"
#include "trjmouse.h"
#include "trjnode.h"
#include "trjcolorpen.h"
#include "trjellipseshape.h"

void MouseTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(getCenterNode());

        auto& eyesNode = rootNode.addChild(getEyesNode(0, 0));
        float px = eyesNode.getPosition().getX();
        float py = eyesNode.getPosition().getY();

        setTitle("Mouse Test", "Move the eyes with the arrow keys. Try the mouse keys too!");

        while(true)
        {
            const trj::Point& mousePosition = trj::Mouse::getPosition();
            float x = 0;
            float y = 0;
            float w = 150;
            float h = 100;
            float mx = mousePosition.getX() - px;
            float my = mousePosition.getY() - py;
            float t = trj::Application::getElapsedTime();

            float ex = w * 0.23f;
            float ey = h * 0.5f;
            float lx = x + ex;
            float ly = y + ey;
            float rx = x + w - ex;
            float ry = y + ey;
            float br = (ex < ey ? ex : ey) * 0.5f;
            float blink = 1 - pow(sinf(t * 0.5f), 200) * 0.8f;

            float dx = (mx - rx) / (ex * 10);
            float dy = (my - ry) / (ey * 10);
            float d = sqrtf(dx * dx + dy * dy);
            if(d > 1.0f)
            {
                dx /= d;
                dy /= d;
            }

            dx *= ex * 0.4f;
            dy *= ey * 0.5f;

            float frameTime = trj::Application::getFrameTime();
            if(trj::Keyboard::isKeyHeld(trj::Keyboard::Key::LEFT))
            {
                px -= frameTime * 120;
            }
            else if(trj::Keyboard::isKeyHeld(trj::Keyboard::Key::RIGHT))
            {
                px += frameTime * 120;
            }

            if(trj::Keyboard::isKeyHeld(trj::Keyboard::Key::UP))
            {
                py -= frameTime * 120;
            }
            else if(trj::Keyboard::isKeyHeld(trj::Keyboard::Key::DOWN))
            {
                py += frameTime * 120;
            }

            eyesNode.setPosition(px, py);

            trj::Pen blackShapeGroupsPen;
            if(trj::Mouse::getLeftButton().isHeld() && trj::Mouse::getRightButton().isHeld())
            {
                blackShapeGroupsPen = trj::ColorPen(32 / 255.0f, 32 / 255.0f, 128 / 255.0f);
            }
            else if(trj::Mouse::getLeftButton().isPressed() || trj::Mouse::getLeftButton().isReleased())
            {
                blackShapeGroupsPen = trj::ColorPen(255 / 255.0f, 32 / 255.0f, 32 / 255.0f);
            }
            else if(trj::Mouse::getLeftButton().isHeld())
            {
                blackShapeGroupsPen = trj::ColorPen(128 / 255.0f, 32 / 255.0f, 32 / 255.0f);
            }
            else if(trj::Mouse::getRightButton().isPressed() || trj::Mouse::getRightButton().isReleased())
            {
                blackShapeGroupsPen = trj::ColorPen(32 / 255.0f, 32 / 255.0f, 255 / 255.0f);
            }
            else if(trj::Mouse::getRightButton().isHeld())
            {
                blackShapeGroupsPen = trj::ColorPen(32 / 255.0f, 32 / 255.0f, 128 / 255.0f);
            }
            else
            {
                blackShapeGroupsPen = trj::ColorPen(32 / 255.0f, 32 / 255.0f, 32 / 255.0f);
            }

            trj::ShapeGroup leftBlackShapeGroup(blackShapeGroupsPen);
            leftBlackShapeGroup.addShape(trj::EllipseShape(lx + dx,
                    ly + dy + ey * 0.25f * (1 - blink),  br, br * blink));
            eyesNode.setShapeGroup(2, std::move(leftBlackShapeGroup));

            dx = (mx - rx) / (ex * 10);
            dy = (my - ry) / (ey * 10);
            d = sqrtf(dx * dx + dy * dy);
            if(d > 1.0f)
            {
                dx /= d;
                dy /= d;
            }

            dx *= ex * 0.4f;
            dy *= ey * 0.5f;

            trj::ShapeGroup rightBlackShapeGroup(blackShapeGroupsPen);
            rightBlackShapeGroup.addShape(trj::EllipseShape(rx + dx,
                    ry + dy + ey * 0.25f * (1 - blink), br, br * blink));
            eyesNode.setShapeGroup(3, std::move(rightBlackShapeGroup));

            trj::Application::update();
            checkEscapeKey();
        }
    });
}
