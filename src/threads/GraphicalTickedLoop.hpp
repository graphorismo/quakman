#pragma once

#include <memory>

#include "../graphics/SfmlGraphicalEngine.hpp"
#include "TickedLoop.hpp"
#include "Messages.hpp"

namespace Quakman::Threads
{
    class GraphicalTickedLoop : public TickedLoop
    {
    public:
        GraphicalTickedLoop(std::shared_ptr<MainToGraphicMailBox> mailBox);

    protected:
        virtual bool WorkWithMailBox() override;
    
    private:
        std::shared_ptr<MainToGraphicMailBox> mailBox;

        std::shared_ptr<Quakman::Graphics::AbstractGraphicalOutput> graphicalEngine = 
            std::shared_ptr<Quakman::Graphics::AbstractGraphicalOutput>
                (new Quakman::Graphics::SfmlGraphicalEngine(800, 600));
    };

};