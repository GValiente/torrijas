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

#include <cmath>
#include "trjmain.h"
#include "trjimagenode.h"
#include "trjtextnode.h"
#include "trjcolorpen.h"
#include "trjradialgradientpen.h"
#include "trjellipseshape.h"
#include "trjtriangleshape.h"
#include "trjlineshapes.h"
#include "trjrotateaction.h"
#include "trjscaleaction.h"
#include "trjsequenceaction.h"
#include "trjrepeataction.h"

namespace
{
    const trj::StrokeColorPen kBlackStrokePen = trj::StrokeColorPen(0.1, 0.1, 0.1, 1, 50);
    const trj::StrokeColorPen kBlackFillPen = trj::StrokeColorPen(0.125, 0.125, 0.125, 1, 45);

    trj::Ptr<trj::Node> getBackgroundNode()
    {
        std::vector<trj::ShapeGroup> shapeGroups(2);
        trj::ShapeGroup& firstShapeGroup = shapeGroups[0];
        firstShapeGroup.setPen(trj::ColorPen(1, 0, 0));

        trj::ShapeGroup& secondShapeGroup = shapeGroups[1];
        secondShapeGroup.setPen(trj::ColorPen(0.9, 0.376, 0));

        constexpr float radius = 3000;

        for(int index = 0, limit = 40; index < limit; ++index)
        {
            float firstAngle = (index * 2 * trj::kPi) / limit;
            float secondAngle = ((index + 1) * 2 * trj::kPi) / limit;

            trj::TriangleShape triangleShape(trj::Point(0, 0),
                    trj::Point(sinf(firstAngle) * radius, cosf(firstAngle) * radius),
                    trj::Point(sinf(secondAngle) * radius, cosf(secondAngle) * radius));

            if(index % 2)
            {
                secondShapeGroup.addShape(triangleShape);
            }
            else
            {
                firstShapeGroup.addShape(triangleShape);
            }
        }

        auto backgroundNode = trj::Node::create();
        backgroundNode->setShapeGroups(std::move(shapeGroups));

        return backgroundNode;
    }

    std::vector<trj::ShapeGroup> getEyebrowsShapeGroups(float posX, float posY)
    {
        std::vector<trj::ShapeGroup> shapeGroups(2);

        trj::Pen strokePen = kBlackStrokePen;
        strokePen.setLineCap(trj::LineCap::SQUARE);
        strokePen.setLineJoin(trj::LineJoin::BEVEL);

        trj::ShapeGroup& strokeShapeGroup = shapeGroups[0];
        strokeShapeGroup.setPen(strokePen);
        strokeShapeGroup.addShape(trj::MoveToShape(posX, posY));
        strokeShapeGroup.addShape(trj::LineToShape(posX + 150, posY + 30));
        strokeShapeGroup.addShape(trj::LineToShape(posX + 300, posY));

        trj::Pen fillPen = kBlackFillPen;
        fillPen.setLineCap(trj::LineCap::SQUARE);
        fillPen.setLineJoin(trj::LineJoin::BEVEL);

        trj::ShapeGroup& fillShapeGroup = shapeGroups[1];
        fillShapeGroup = strokeShapeGroup;
        fillShapeGroup.setPen(fillPen);

        return shapeGroups;
    }

    std::vector<trj::ShapeGroup> getArmShapeGroups(float posX, float posY)
    {
        std::vector<trj::ShapeGroup> shapeGroups(7);
        constexpr float size = 150;

        trj::ShapeGroup& armStrokeShapeGroup = shapeGroups[0];
        armStrokeShapeGroup.setPen(kBlackStrokePen);
        armStrokeShapeGroup.addShape(trj::MoveToShape(posX, posY));
        armStrokeShapeGroup.addShape(trj::QuadToShape(trj::Point(posX + size, posY),
                trj::Point(posX + size, posY - size)));

        trj::ShapeGroup& armFillShapeGroup = shapeGroups[1];
        armFillShapeGroup = armStrokeShapeGroup;
        armFillShapeGroup.setPen(kBlackFillPen);

        trj::ShapeGroup& handStrokeShapeGroup = shapeGroups[2];
        handStrokeShapeGroup.setPen(kBlackStrokePen);
        handStrokeShapeGroup.addShape(trj::EllipseShape(posX + 150, -posY - 135, 23, 23));

        trj::ShapeGroup& handFillShapeGroup = shapeGroups[3];
        handFillShapeGroup = handStrokeShapeGroup;
        handFillShapeGroup.setPen(kBlackFillPen);

        trj::ShapeGroup& fingerStrokeShapeGroup = shapeGroups[4];
        fingerStrokeShapeGroup.setPen(kBlackStrokePen);
        fingerStrokeShapeGroup.addShape(trj::EllipseShape(posX + 110, -posY - 127, 2, 2));

        trj::ShapeGroup& fingerFillShapeGroup = shapeGroups[5];
        fingerFillShapeGroup = fingerStrokeShapeGroup;
        fingerFillShapeGroup.setPen(kBlackFillPen);

        trj::ShapeGroup& glossShapeGroup = shapeGroups[6];
        const float glossPosX = posX + 165;
        const float glossPosY = -posY - 155;

        glossShapeGroup.setPen(trj::RadialGradientPen(glossPosX, glossPosY, 2, 20,
                trj::Color(1, 1, 1, 0.125), trj::Color(1, 1, 1, 0)));
        glossShapeGroup.addShape(trj::EllipseShape(glossPosX, glossPosY, 30, 30));

        return shapeGroups;
    }

    std::vector<trj::ShapeGroup> getLegShapeGroups(float posX, float posY)
    {
        std::vector<trj::ShapeGroup> shapeGroups(5);

        trj::ShapeGroup& legStrokeShapeGroup = shapeGroups[0];
        legStrokeShapeGroup.setPen(kBlackStrokePen);
        legStrokeShapeGroup.addShape(trj::MoveToShape(posX, posY));
        legStrokeShapeGroup.addShape(trj::QuadToShape(trj::Point(posX + 100, posY),
                trj::Point(posX + 100, posY + 150)));

        trj::ShapeGroup& legFillShapeGroup = shapeGroups[1];
        legFillShapeGroup = legStrokeShapeGroup;
        legFillShapeGroup.setPen(kBlackFillPen);

        trj::ShapeGroup& footStrokeShapeGroup = shapeGroups[2];
        footStrokeShapeGroup.setPen(kBlackStrokePen);
        footStrokeShapeGroup.addShape(trj::EllipseShape(posX + 125, posY + 155, 45, 15));

        trj::ShapeGroup& footFillShapeGroup = shapeGroups[3];
        footFillShapeGroup = footStrokeShapeGroup;
        footFillShapeGroup.setPen(kBlackFillPen);

        trj::ShapeGroup& glossShapeGroup = shapeGroups[4];
        const float glossPosX = posX + 165;
        const float glossPosY = posY + 150;

        glossShapeGroup.setPen(trj::RadialGradientPen(glossPosX, glossPosY, 2, 20,
                trj::Color(1, 1, 1, 0.125), trj::Color(1, 1, 1, 0)));
        glossShapeGroup.addShape(trj::EllipseShape(glossPosX, glossPosY, 30, 30));

        return shapeGroups;
    }

    trj::Ptr<trj::Node> getTorrijoNode()
    {
        auto torrijoNode = trj::Node::create();
        auto& backTorrijoNode = torrijoNode->addNewChild();

        trj::Image torrijoImage("../../torrijas-test/images/torrijo.png");
        auto& frontTorrijoNode = torrijoNode->addChild(trj::ImageNode::create(400, torrijoImage));

        float eyesPosX = -75;
        float eyesPosY = -275;
        frontTorrijoNode.setShapeGroups(Test::getEyesShapeGroups(eyesPosX, eyesPosY, 300, 200, -300, 0));

        auto eyebrowsShapeGroups = getEyebrowsShapeGroups(eyesPosX, eyesPosY);
        for(auto& eyebrowsShapeGroup : eyebrowsShapeGroups)
        {
            frontTorrijoNode.addShapeGroup(std::move(eyebrowsShapeGroup));
        }

        auto leftArmShapeGroups = getArmShapeGroups(eyesPosX + 150, eyesPosY + 290);
        for(auto& leftArmShapeGroup : leftArmShapeGroups)
        {
            leftArmShapeGroup.horizontalFlip();
            backTorrijoNode.addShapeGroup(std::move(leftArmShapeGroup));
        }

        auto rightArmShapeGroups = getArmShapeGroups(eyesPosX + 300, eyesPosY + 290);
        for(auto& rightArmShapeGroup : rightArmShapeGroups)
        {
            frontTorrijoNode.addShapeGroup(std::move(rightArmShapeGroup));
        }

        auto leftLegShapeGroups = getLegShapeGroups(eyesPosX + 50, eyesPosY + 380);
        for(auto& leftLegShapeGroup : leftLegShapeGroups)
        {
            leftLegShapeGroup.horizontalFlip();
            backTorrijoNode.addShapeGroup(std::move(leftLegShapeGroup));
        }

        auto rightLegShapeGroups = getLegShapeGroups(eyesPosX + 200, eyesPosY + 380);
        for(auto& rightLegShapeGroup : rightLegShapeGroups)
        {
            backTorrijoNode.addShapeGroup(std::move(rightLegShapeGroup));
        }

        return torrijoNode;
    }
}

void TorrijoTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        auto& backgroundNode = rootNode.addChild(getBackgroundNode());
        backgroundNode.addAction(trj::RepeatAction::create(trj::RotateAction::create(0.3, 1)));

        std::vector<trj::Ptr<trj::Action>> torrijoActions;
        torrijoActions.push_back(trj::ScaleAction::create(0.25, 0.25, 0.1));
        torrijoActions.push_back(torrijoActions.front()->getReversed());

        auto& torrijoNode = rootNode.addChild(getTorrijoNode());
        torrijoNode.addAction(trj::RepeatAction::create(trj::SequenceAction::create(std::move(torrijoActions))));

        auto titleShadowNode = trj::TextNode::create();
        titleShadowNode->setFontSize(80);
        titleShadowNode->setFontBlur(3);
        titleShadowNode->setHorizontalAlignment(trj::TextNode::HorizontalAlignment::CENTER);
        titleShadowNode->addText(0, (trj::Application::getScreenHeight() * 0.5f) - 25, "TORRIJAS ENGINE");

        auto titleNode = titleShadowNode->getTextClone();
        titleNode->setPosition(-4, -4 + titleShadowNode->getPosition().getY());
        titleNode->setFontColor(1, 1, 1);
        titleNode->setFontBlur(0);

        rootNode.addChild(std::move(titleShadowNode));
        rootNode.addChild(std::move(titleNode));

        while(true)
        {
            trj::Application::update();
            checkEscapeKey();
        }
    });
}
