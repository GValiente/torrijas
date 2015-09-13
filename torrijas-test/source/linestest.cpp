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

#include "linestest.h"

#include <cmath>
#include "trjmain.h"
#include "trjnode.h"
#include "trjcolorpen.h"
#include "trjlineshapes.h"

void LinesTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(getCenterNode());

        auto& linesMainNode = rootNode.addNewChild();
        linesMainNode.setPosition(-800, 70);
        linesMainNode.setScale(3.0f);

        setTitle("Lines Test");

        float x = 0;
        float y = 0;
        float w = 600;

        float pad = 5.0f;
        float s = w / 9.0f - pad * 2;

        trj::LineJoin joins[2] = {
            trj::LineJoin::ROUND, trj::LineJoin::BEVEL
        };

        trj::LineCap caps[3] = {
            trj::LineCap::BUTT, trj::LineCap::ROUND, trj::LineCap::SQUARE
        };

        while(true)
        {
            float elapsedTime = trj::Application::getElapsedTime();
            float pts[4 * 2];
            pts[0] = -s * 0.25f + cosf(elapsedTime * 0.3f) * s * 0.5f;
            pts[1] = sinf(elapsedTime * 0.3f) * s * 0.5f;
            pts[2] = -s * 0.25;
            pts[3] = 0;
            pts[4] = s * 0.25f;
            pts[5] = 0;
            pts[6] = s * 0.25f + cosf(-elapsedTime * 0.3f) * s * 0.5f;
            pts[7] = sinf(-elapsedTime * 0.3f) * s * 0.5f;
            linesMainNode.clearShapeGroups();

            for(int i = 0; i < 3; ++i)
            {
                for(int j = 0; j < 2; ++j)
                {
                    float fx = x + s * 0.5f + (i * 3 + j) / 9.0f * w + pad;
                    float fy = y - s * 0.5f + pad;

                    trj::StrokeColorPen pen(0, 0, 0, 160 / 255.0f, s * 0.3f);
                    pen.setLineCap(caps[i]);
                    pen.setLineJoin(joins[j]);

                    trj::ShapeGroup shapeGroup(pen);
                    shapeGroup.addShape(trj::MoveToShape(fx + pts[0], fy + pts[1]));
                    shapeGroup.addShape(trj::LineToShape(fx + pts[2], fy + pts[3]));
                    shapeGroup.addShape(trj::LineToShape(fx + pts[4], fy + pts[5]));
                    shapeGroup.addShape(trj::LineToShape(fx + pts[6], fy + pts[7]));
                    linesMainNode.addShapeGroup(std::move(shapeGroup));

                    pen = trj::StrokeColorPen(0, 192 / 255.0f, 1.0f);
                    pen.setLineCap(trj::LineCap::BUTT);
                    pen.setLineJoin(trj::LineJoin::BEVEL);

                    shapeGroup = trj::ShapeGroup(pen);
                    shapeGroup.addShape(trj::MoveToShape(fx + pts[0], fy + pts[1]));
                    shapeGroup.addShape(trj::LineToShape(fx + pts[2], fy + pts[3]));
                    shapeGroup.addShape(trj::LineToShape(fx + pts[4], fy + pts[5]));
                    shapeGroup.addShape(trj::LineToShape(fx + pts[6], fy + pts[7]));
                    linesMainNode.addShapeGroup(std::move(shapeGroup));
                }
            }

            trj::Application::update();
            checkEscapeKey();
        }
    });
}
