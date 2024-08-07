#include "SfmlAudioEngine.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

using namespace Quakman::Audio;

SfmlAudioEngine::SfmlAudioEngine() {}

void SfmlAudioEngine::Load(Soundable soundable)
{
    sf::SoundBuffer soundBuffer;
    soundBuffer.loadFromFile(soundable.pathToFile);
    soundableToSoundBuffer.insert_or_assign(soundable, soundBuffer);
}

void SfmlAudioEngine::Play(Soundable soundable, float volume, float pitch)
{
    playingSound.stop();
    playingSound.setBuffer(soundableToSoundBuffer[soundable]);
    playingSound.setVolume(volume);
    playingSound.setPitch(pitch);
    playingSound.setPlayingOffset(sf::seconds(soundable.startTime));
    playingSound.play();
}