#pragma once

#include <unordered_map>

#include <raylib.h>

#include "AbstractAudioOutput.hpp"
#include "Soundable.hpp"


namespace Quakman::Audio
{
    
    class RaylibAudioEngine : public AbstractAudioOutput
    {
    public:
        RaylibAudioEngine();
        virtual ~RaylibAudioEngine();

        virtual void Load(Soundable soundable) override;
        virtual void Play(Soundable soundable, float volume, float pitch) override;
    private:
        std::pmr::unordered_map<Soundable, ::Sound> soundableToSound;
    };
}