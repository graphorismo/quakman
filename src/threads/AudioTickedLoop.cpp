#include "AudioTickedLoop.hpp"

using namespace Quakman::Threads;

AudioTickedLoop::AudioTickedLoop(std::shared_ptr<MainToAudioMailBox> mailBox)
:   mailBox(mailBox)
{

}

bool AudioTickedLoop::WorkWithMailBox()
{
    mailBox->mutex.lock();
    if (mailBox->downwardMessages.empty())
    {
        mailBox->mutex.unlock();
        return false;
    }
    auto message = mailBox->downwardMessages.top();
    mailBox->downwardMessages.pop();
    mailBox->mutex.unlock();
    switch (message.command) {
        case Quakman::Threads::AudioThreadCommands::LOAD:
        {
            audioEngine->Load(message.data);
            break;
        }   
        
        case Quakman::Threads::AudioThreadCommands::PLAY:
            audioEngine->Play(message.data, message.volume, message.pitch);
        break;

        case Quakman::Threads::AudioThreadCommands::REPORT:
            mailBox->mutex.lock();
            mailBox->upwardMessages.push(Quakman::Threads::Report()); // TODO build a real report
            mailBox->mutex.unlock();
        break;
        
        case Quakman::Threads::AudioThreadCommands::EXIT:
            return true;

        case Quakman::Threads::AudioThreadCommands::PAUSE:
        default:
            return false;
    }
    return false;
}