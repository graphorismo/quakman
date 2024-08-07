#pragma once

#include "Soundable.hpp"

namespace Quakman::Audio
{
    
    class AbstractAudioOutput
    {
    public:
        virtual void Load(Soundable soundable) = 0;
        virtual void Play(Soundable soundable, float volume, float pitch) = 0;

        virtual ~AbstractAudioOutput() {};
    };
}