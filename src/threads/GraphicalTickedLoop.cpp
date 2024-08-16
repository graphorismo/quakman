#include <glog/logging.h>

#include "GraphicalTickedLoop.hpp"
#include "Messages.hpp"

using namespace Quakman::Threads;

GraphicalTickedLoop::GraphicalTickedLoop(std::shared_ptr<Quakman::Threads::MainToGraphicMailBox> mailBox)
:   mailBox(mailBox)
{

}

bool GraphicalTickedLoop::WorkWithMailBox()
{
    DLOG(INFO) <<"Starting to work with the mailbox for graphical thread";
    mailBox->mutex.lock();
    if (mailBox->downwardMessages.empty())
    {
        mailBox->mutex.unlock();
        DLOG(INFO) <<"There is no messages for the graphical thread";
        return false;
    }
    Quakman::Threads::GraphicalThreadInput message = mailBox->downwardMessages.front();
    mailBox->downwardMessages.pop();
    mailBox->mutex.unlock();
    switch (message.command) {
        case Quakman::Threads::GraphicalThreadCommands::LOAD:
        {
            DLOG(INFO) <<"There is a LOAD message for the graphical thread";
            if (! std::holds_alternative<Quakman::Graphics::Drawable>(message.data)) break;
            Quakman::Graphics::Drawable drawable = std::get<Quakman::Graphics::Drawable>(message.data);
            graphicalEngine->LoadTexture(drawable);
            break;
        }   
        
        case Quakman::Threads::GraphicalThreadCommands::DRAW:
            DLOG(INFO) <<"There is a DRAW message for the graphical thread";
            if (std::holds_alternative<Quakman::Graphics::Drawable>(message.data))
                graphicalEngine->Draw
                    (std::get<Quakman::Graphics::Drawable>(message.data), message.position);
            else
                graphicalEngine->Draw
                    (std::get<Quakman::Graphics::Writeable>(message.data), message.position);
        break;
        case Quakman::Threads::GraphicalThreadCommands::REPORT:
            DLOG(INFO) <<"There is a REPORT message for the graphical thread";
            mailBox->mutex.lock();
            mailBox->upwardMessages.push(Quakman::Threads::Report()); // TODO build a real report
            mailBox->mutex.unlock();
        break;
        
        case Quakman::Threads::GraphicalThreadCommands::EXIT:
            DLOG(INFO) <<"There is an EXIT message for the graphical thread";
            return true;

        case Quakman::Threads::GraphicalThreadCommands::PAUSE:
        default:
            DLOG(INFO) <<"There is a PAUSE message for the graphical thread";
            return false;
    }  
    return false; 
} 
