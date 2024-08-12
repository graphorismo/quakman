#include "GraphicalTickedLoop.hpp"
#include "Messages.hpp"

using namespace Quakman::Threads;

GraphicalTickedLoop::GraphicalTickedLoop(std::shared_ptr<Quakman::Threads::MainToGraphicMailBox> mailBox)
:   mailBox(mailBox)
{

}

bool GraphicalTickedLoop::WorkWithMailBox()
{
    mailBox->mutex.lock();
    if (mailBox->downwardMessages.empty())
    {
        mailBox->mutex.unlock();
        return false;
    }
    Quakman::Threads::GraphicalThreadInput message = mailBox->downwardMessages.top();
    mailBox->downwardMessages.pop();
    mailBox->mutex.unlock();
    switch (message.command) {
        case Quakman::Threads::GraphicalThreadCommands::LOAD:
        {
            if (! std::holds_alternative<Quakman::Graphics::Drawable>(message.data)) break;
            Quakman::Graphics::Drawable drawable = std::get<Quakman::Graphics::Drawable>(message.data);
            graphicalEngine->LoadTexture(drawable);
            break;
        }   
        
        case Quakman::Threads::GraphicalThreadCommands::DRAW:
            if (message.data.index() == 0)
                graphicalEngine->Draw
                    (std::get<Quakman::Graphics::Drawable>(message.data), message.position);
            else
                graphicalEngine->Draw
                    (std::get<Quakman::Graphics::Writeable>(message.data), message.position);
        break;
        case Quakman::Threads::GraphicalThreadCommands::REPORT:
            mailBox->mutex.lock();
            mailBox->upwardMessages.push(Quakman::Threads::Report()); // TODO build a real report
            mailBox->mutex.unlock();
        break;
        
        case Quakman::Threads::GraphicalThreadCommands::EXIT:
            return true;
        case Quakman::Threads::GraphicalThreadCommands::PAUSE:
        default:
            return false;
    }  
    return true;
} 
