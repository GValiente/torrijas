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

#include "eyesbenchmark.h"

#include <random>
#include "trjmain.h"
#include "trjnode.h"

void EyesBenchmark::run()
{
    trj::main([]()
    {
        trj::Application::setBackgroundColor(0.3, 0.3, 0.3);
        trj::Application::setShowPerformanceGraphs(true);

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(getCenterNode());

        std::mt19937 randomGenerator;
        std::uniform_real_distribution<float> positionDistribution(-5000, 5000);
        std::uniform_real_distribution<float> opacityDistribution(0.5f, 1.0f);
        std::uniform_real_distribution<float> colorDistribution(0.0f, 1.0f);
        std::uniform_real_distribution<float> blendFactorDistribution(0.0f, 0.5f);

        const int numEyes = 25000;
        double time = trj::Application::getElapsedTime();
        float aspectRatio = trj::Application::getScreenAspectRatio();
        rootNode.reserveChildren(numEyes + rootNode.getChildren().size());

        for(int index = 0; index < numEyes; ++index)
        {
            float positionX = positionDistribution(randomGenerator);
            float positionY = positionDistribution(randomGenerator);

            auto& eyesNode = rootNode.addChild(getEyesNode(positionX * aspectRatio, positionY));
            eyesNode.setOpacity(opacityDistribution(randomGenerator));

            trj::Color blendColor(colorDistribution(randomGenerator),
                    colorDistribution(randomGenerator), colorDistribution(randomGenerator));
            eyesNode.setBlendColor(blendColor, blendFactorDistribution(randomGenerator));
        }

        double loadTime = trj::Application::getElapsedTime() - time;
        setTitle("Eyes Benchmark", "Load time: " + trj::String(loadTime) + " seconds");

        while(true)
        {
            trj::Application::update();
            checkEscapeKey();
        }
    });
}
