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

#include "actionstest.h"

#include <random>
#include "trjmain.h"
#include "trjnode.h"
#include "trjmoveaction.h"
#include "trjwaitaction.h"
#include "trjcallbackaction.h"
#include "trjsequenceaction.h"
#include "trjrepeataction.h"

void ActionsTest::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(getCenterNode());

        auto moveAction = trj::MoveAction::create(500, 250, 1.5);
        auto reverseMoveAction = moveAction->getReversed();

        auto& testNode = rootNode.addChild(getTestNode());
        std::mt19937 randomGenerator;
        std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

        std::vector<trj::Ptr<trj::Action>> actions;
        actions.push_back(std::move(moveAction));
        actions.push_back(std::move(reverseMoveAction));
        actions.push_back(std::move(trj::WaitAction::create(0.5)));
        actions.push_back(std::move(trj::CallBackAction::create([&]()
        {
            float red = distribution(randomGenerator);
            float green = distribution(randomGenerator);
            float blue = distribution(randomGenerator);
            testNode.setBlendColor(red, green, blue, 0.2f);
        })));

        auto sequenceAction = trj::SequenceAction::create(std::move(actions));
        auto repeatAction = trj::RepeatAction::create(std::move(sequenceAction));

        testNode.addAction(std::move(repeatAction));
        testNode.setActionsSpeed(1.5f);

        setTitle("Actions Test");

        while(true)
        {
            trj::Application::update();
            checkEscapeKey();
        }
    });
}
