#pragma once

#include <unordered_map>

#include <SFML/Audio.hpp>

#include "AbstractAudioOutput.hpp"
#include "SFML/Audio/Sound.hpp"
#include "Soundable.hpp"


namespace Quakman::Audio
{
    
    class SfmlAudioEngine : public AbstractAudioOutput
    {
    public:
        SfmlAudioEngine();

        virtual void Load(Soundable soundable) override;
        virtual void Play(Soundable soundable, float volume, float pitch) override;
    private:
        std::pmr::unordered_map<Soundable, sf::SoundBuffer> soundableToSoundBuffer;
        sf::Sound playingSound;
    };
}