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
    VLOG(2) <<"Starting to work with the mailbox for graphical thread";
    mailBox->mutex.lock();
    if (mailBox->downwardMessages.empty())
    {
        mailBox->mutex.unlock();
        VLOG(2) <<"There is no messages for the graphical thread";
        return false;
    }
    Quakman::Threads::GraphicalThreadInput message = mailBox->downwardMessages.top();
    mailBox->downwardMessages.pop();
    mailBox->mutex.unlock();
    switch (message.command) {
        case Quakman::Threads::GraphicalThreadCommands::LOAD:
        {
            VLOG(2) <<"There is a LOAD message for the graphical thread";
            if (! std::holds_alternative<Quakman::Graphics::Drawable>(message.data)) break;
            Quakman::Graphics::Drawable drawable = std::get<Quakman::Graphics::Drawable>(message.data);
            graphicalEngine->LoadTexture(drawable);
            break;
        }   
        
        case Quakman::Threads::GraphicalThreadCommands::DRAW:
            VLOG(2) <<"There is a DRAW message for the graphical thread";
            if (message.data.index() == 0)
                graphicalEngine->Draw
                    (std::get<Quakman::Graphics::Drawable>(message.data), message.position);
            else
                graphicalEngine->Draw
                    (std::get<Quakman::Graphics::Writeable>(message.data), message.position);
        break;
        case Quakman::Threads::GraphicalThreadCommands::REPORT:
            VLOG(2) <<"There is a REPORT message for the graphical thread";
            mailBox->mutex.lock();
            mailBox->upwardMessages.push(Quakman::Threads::Report()); // TODO build a real report
            mailBox->mutex.unlock();
        break;
        
        case Quakman::Threads::GraphicalThreadCommands::EXIT:
            VLOG(2) <<"There is an EXIT message for the graphical thread";
            return true;

        case Quakman::Threads::GraphicalThreadCommands::PAUSE:
        default:
            VLOG(2) <<"There is a PAUSE message for the graphical thread";
            return false;
    }  
    return true;
    VLOG(2) <<"Something went wrong, code must be unrechable";
} 
