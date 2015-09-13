#include "trjmain.h"
#include "trjtextnode.h"

int main()
{
    trj::ApplicationConfig config;
    config.setWindowTitle("Torrijas Template");

    trj::main(std::move(config), []()
    {
        auto textNode = trj::TextNode::create();
        textNode->setFontSize(80);
        textNode->setFontColor(1, 1, 1);
        textNode->setHorizontalAlignment(trj::TextNode::HorizontalAlignment::CENTER);
        textNode->addText(0, 0, "Hi :D");

        auto& rootNode = trj::Node::getRootNode();
        rootNode.addChild(std::move(textNode));

        while(true)
        {
            trj::Application::update();
        }
    });

    return 0;
}
