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

#include "test.h"

#include <cmath>
#include "trjapplication.h"
#include "trjkeyboard.h"
#include "trjtextnode.h"
#include "trjcolorpen.h"
#include "trjlineargradientpen.h"
#include "trjradialgradientpen.h"
#include "trjellipseshape.h"

trj::Ptr<trj::Node> Test::getCenterNode()
{
    trj::ShapeGroup shapeGroup(trj::ColorPen(0, 0, 0, 0.5));
    shapeGroup.addShape(trj::EllipseShape(0, 0, 10, 10));

    auto node = trj::Node::create();
    node->addShapeGroup(std::move(shapeGroup));

    return node;
}

trj::Ptr<trj::Node> Test::getTestNode()
{
    trj::ShapeGroup shapeGroup(trj::LinearGradientPen(trj::Point(-50, -100), trj::Point(250, 200),
            trj::Color(0, 0, 0, 0.9), trj::Color(0, 0, 0, 0.1)));
    shapeGroup.addShape(trj::EllipseShape(0, 0, 50, 100));
    shapeGroup.addShape(trj::EllipseShape(150, 150, 100, 50));

    auto node = trj::Node::create();
    node->addShapeGroup(std::move(shapeGroup));

    return node;
}

std::vector<trj::ShapeGroup> Test::getEyesShapeGroups(float x, float y, float w, float h,
        float mx, float my)
{
    std::vector<trj::ShapeGroup> shapeGroups;

    float t = 0;

    float ex = w * 0.23f;
    float ey = h * 0.5f;
    float lx = x + ex;
    float ly = y + ey;
    float rx = x + w - ex;
    float ry = y + ey;
    float br = (ex < ey ? ex : ey) * 0.5f;
    float blink = 1 - pow(sinf(t * 0.5f), 200) * 0.8f;

    trj::ShapeGroup shadowShapeGroup(trj::LinearGradientPen(trj::Point(x, y + h * 0.5f),
            trj::Point(x + w * 0.1f, y + h), trj::Color(0, 0, 0, 32 / 255.0f),
            trj::Color(0, 0, 0, 16 / 255.0f)));
    shadowShapeGroup.addShape(trj::EllipseShape(lx + 3.0f, ly + 16.0f, ex, ey));
    shadowShapeGroup.addShape(trj::EllipseShape(rx + 3.0f, ry + 16.0f, ex, ey));
    shapeGroups.push_back(std::move(shadowShapeGroup));

    trj::ShapeGroup whiteShapeGroup(trj::LinearGradientPen(trj::Point(x, y + h * 0.25f),
            trj::Point(x + w * 0.1f, y + h),
            trj::Color(220 / 255.0f, 220 / 255.0f, 220 / 255.0f),
            trj::Color(128 / 255.0f, 128 / 255.0f, 128 / 255.0f)));
    whiteShapeGroup.addShape(trj::EllipseShape(lx, ly, ex, ey));
    whiteShapeGroup.addShape(trj::EllipseShape(rx, ry, ex, ey));
    shapeGroups.push_back(std::move(whiteShapeGroup));

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

    trj::ShapeGroup leftBlackShapeGroup(trj::ColorPen(32 / 255.0f, 32 / 255.0f, 32 / 255.0f));
    leftBlackShapeGroup.addShape(trj::EllipseShape(lx + dx, ly + dy + ey * 0.25f * (1 - blink),
            br, br * blink));
    shapeGroups.push_back(std::move(leftBlackShapeGroup));

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

    trj::ShapeGroup rightBlackShapeGroup(trj::ColorPen(32 / 255.0f, 32 / 255.0f, 32 / 255.0f));
    rightBlackShapeGroup.addShape(trj::EllipseShape(rx + dx, ry + dy + ey * 0.25f * (1 - blink),
            br, br * blink));
    shapeGroups.push_back(std::move(rightBlackShapeGroup));

    trj::ShapeGroup leftGlossShapeGroup(trj::RadialGradientPen(
            lx - ex * 0.25f, ly - ey * 0.5f, ex * 0.1f, ex * 0.75f,
            trj::Color(255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 128 / 255.0f),
            trj::Color(255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 0 / 255.0f)));
    leftGlossShapeGroup.addShape(trj::EllipseShape(lx, ly, ex, ey));
    shapeGroups.push_back(std::move(leftGlossShapeGroup));

    trj::ShapeGroup rightGlossShapeGroup(trj::RadialGradientPen(
            rx - ex * 0.25f, ry - ey * 0.5f, ex * 0.1f, ex * 0.75f,
            trj::Color(255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 128 / 255.0f),
            trj::Color(255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 0 / 255.0f)));
    rightGlossShapeGroup.addShape(trj::EllipseShape(rx, ry, ex, ey));
    shapeGroups.push_back(std::move(rightGlossShapeGroup));

    return shapeGroups;
}

trj::Ptr<trj::Node> Test::getEyesNode(float positionX, float positionY)
{
    auto eyesNode = trj::Node::create();
    eyesNode->setPosition(positionX - 110.25, positionY - 70);
    eyesNode->setScale(1.5f);
    eyesNode->setShapeGroups(getEyesShapeGroups(0, 0, 150, 100, 0, 0));

    return eyesNode;
}

void Test::setTitle(trj::String title)
{
    auto titleNode = trj::TextNode::create();
    titleNode->setFontSize(80);
    titleNode->setHorizontalAlignment(trj::TextNode::HorizontalAlignment::CENTER);
    titleNode->addText(0, (trj::Application::getScreenHeight() * 0.5f) - 25, std::move(title));

    auto& rootNode = trj::Node::getRootNode();
    rootNode.addChild(std::move(titleNode));
}

void Test::setTitle(trj::String title, trj::String aditionalText)
{
    float screenHeight = trj::Application::getScreenHeight();
    auto titleNode = trj::TextNode::create();
    titleNode->setFontSize(80);
    titleNode->setHorizontalAlignment(trj::TextNode::HorizontalAlignment::CENTER);
    titleNode->addText(0, (screenHeight * 0.5f) - 100, std::move(title));

    auto aditionalTextNode = trj::TextNode::create();
    aditionalTextNode->setFontSize(60);
    aditionalTextNode->setHorizontalAlignment(trj::TextNode::HorizontalAlignment::CENTER);
    aditionalTextNode->addText(0, (screenHeight * 0.5f) - 25, std::move(aditionalText));

    auto& rootNode = trj::Node::getRootNode();
    rootNode.addChild(std::move(titleNode));
    rootNode.addChild(std::move(aditionalTextNode));
}

void Test::checkEscapeKey()
{
    if(trj::Keyboard::isKeyPressed(trj::Keyboard::Key::ESCAPE))
    {
        trj::Application::setClosed(true);
    }
}
