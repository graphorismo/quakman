#include <glog/logging.h>

#include "AudioTickedLoop.hpp"

using namespace Quakman::Threads;

AudioTickedLoop::AudioTickedLoop(std::shared_ptr<MainToAudioMailBox> mailBox)
:   mailBox(mailBox)
{

}

bool AudioTickedLoop::WorkWithMailBox()
{
    VLOG(2) <<"Starting to work with the mailbox for audio thread";
    mailBox->mutex.lock();
    if (mailBox->downwardMessages.empty())
    {
        VLOG(2) <<"There is no message for audio thread";
        mailBox->mutex.unlock();
        return false;
    }
    auto message = mailBox->downwardMessages.top();
    mailBox->downwardMessages.pop();
    mailBox->mutex.unlock();
    switch (message.command) {
        case Quakman::Threads::AudioThreadCommands::LOAD:
        {
            VLOG(2) <<"There is a LOAD message for audio thread";
            audioEngine->Load(message.data);
            break;
        }   
        
        case Quakman::Threads::AudioThreadCommands::PLAY:
            VLOG(2) <<"There is a PLAY message for audio thread";
            audioEngine->Play(message.data, message.volume, message.pitch);
        break;

        case Quakman::Threads::AudioThreadCommands::REPORT:
            VLOG(2) <<"There is a REPORT message for audio thread";
            mailBox->mutex.lock();
            mailBox->upwardMessages.push(Quakman::Threads::Report()); // TODO build a real report
            mailBox->mutex.unlock();
        break;
        
        case Quakman::Threads::AudioThreadCommands::EXIT:
            VLOG(2) <<"There is an EXIT message for audio thread";
            return true;

        case Quakman::Threads::AudioThreadCommands::PAUSE:
            VLOG(2) <<"There is a PAUSE message for audio thread";
        default:
            return false;
    }
    return false;
}