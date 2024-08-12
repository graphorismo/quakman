#pragma once

#include <memory>

#include "Messages.hpp"
#include "TickedLoop.hpp"
#include "../audio/AbstractAudioOutput.hpp"
#include "../audio/SfmlAudioEngine.hpp"

namespace Quakman::Threads 
{
    class AudioTickedLoop : public TickedLoop
    {
    public:
        AudioTickedLoop(std::shared_ptr<MainToAudioMailBox> mailBox);

    protected:
        virtual bool WorkWithMailBox() override;

    private:
        std::shared_ptr<MainToAudioMailBox> mailBox;
        std::shared_ptr<Quakman::Audio::AbstractAudioOutput> audioEngine = 
            std::shared_ptr<Quakman::Audio::AbstractAudioOutput>
                (new Quakman::Audio::SfmlAudioEngine());
    };

}