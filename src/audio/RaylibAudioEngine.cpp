#include "RaylibAudioEngine.hpp"

using namespace Quakman::Audio;

RaylibAudioEngine::RaylibAudioEngine() {
    ::InitAudioDevice();
}

RaylibAudioEngine::~RaylibAudioEngine() {
    for (auto pair : soundableToSound)
    {
        ::UnloadSound(pair.second);
    }
    ::CloseAudioDevice();
}

void RaylibAudioEngine::Load(Soundable soundable)
{
    Sound sound = ::LoadSound(soundable.pathToFile.data());
    soundableToSound.insert_or_assign(soundable, sound);
}

void RaylibAudioEngine::Play(Soundable soundable, float volume, float pitch)
{
    auto sound = soundableToSound[soundable];
    ::PlaySound(sound);
}