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

#include "boundingboxtest.h"

#include "trjmain.h"
#include "trjnode.h"
#include "trjcolorpen.h"
#include "trjlineshapes.h"
#include "trjarcshape.h"
#include "trjtriangleshape.h"
#include "trjrectshape.h"
#include "trjellipseshape.h"

void BoundingBoxTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);
        trj::Application::setShowBoundingBoxes(true);

        trj::ShapeGroup shapeGroup(trj::StrokeColorPen(0.75, 0.125, 0.125, 1, 5));
        shapeGroup.addShape(trj::MoveToShape(-20, -20));
        shapeGroup.addShape(trj::LineToShape(50, 30));

        auto& rootNode = trj::Node::getRootNode();
        auto& lineToNode = rootNode.addNewChild();
        lineToNode.addShapeGroup(std::move(shapeGroup));
        lineToNode.setPosition(-150, -150);

        shapeGroup.addShape(trj::MoveToShape(-20, -20));
        shapeGroup.addShape(trj::BezierToShape(trj::Point(-30, -10), trj::Point(-30, 0), trj::Point(50, 10)));

        auto& bezierToNode = rootNode.addNewChild();
        bezierToNode.addShapeGroup(std::move(shapeGroup));
        bezierToNode.setPosition(0, -150);

        shapeGroup.addShape(trj::MoveToShape(-20, -20));
        shapeGroup.addShape(trj::QuadToShape(trj::Point(-30, 10), trj::Point(50, 10)));

        auto& quadToNode = rootNode.addNewChild();
        quadToNode.addShapeGroup(std::move(shapeGroup));
        quadToNode.setPosition(150, -150);

        shapeGroup.addShape(trj::ArcShape(0, 0, 50, trj::toRadians(30), trj::toRadians(300)));

        auto& arcNode = rootNode.addNewChild();
        arcNode.addShapeGroup(std::move(shapeGroup));
        arcNode.setPosition(-300, 100);

        shapeGroup.addShape(trj::TriangleShape(trj::Point(-20, -20), trj::Point(50, 0), trj::Point(0, 50)));

        auto& triangleNode = rootNode.addNewChild();
        triangleNode.addShapeGroup(std::move(shapeGroup));
        triangleNode.setPosition(-150, 100);

        shapeGroup.addShape(trj::RectShape(-20, -20, 80, 60));

        auto& rectNode = rootNode.addNewChild();
        rectNode.addShapeGroup(std::move(shapeGroup));
        rectNode.setPosition(0, 100);

        shapeGroup.addShape(trj::RectShape(-20, -20, 80, 60, 20));

        auto& roundRectNode = rootNode.addNewChild();
        roundRectNode.addShapeGroup(std::move(shapeGroup));
        roundRectNode.setPosition(150, 100);

        shapeGroup.addShape(trj::EllipseShape(0, 10, 40, 30));

        auto& ellipseNode = rootNode.addNewChild();
        ellipseNode.addShapeGroup(std::move(shapeGroup));
        ellipseNode.setPosition(300, 100);

        setTitle("Bounding Box Test");

        while(true)
        {
            trj::Application::update();
            checkEscapeKey();
        }
    });
}
