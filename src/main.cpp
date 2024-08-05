
#include <chrono>
#include <memory>
#include <thread>
#include <variant>

#include "graphics/AbstractGraphicalOutput.hpp"
#include "graphics/Writeable.hpp"
#include "graphics/Drawable.hpp"
#include "graphics/SfmlGraphicalEngine.hpp"
#include "threads/Report.hpp"
#include "threads/Messages.hpp"


void RunGraphicalThread(std::shared_ptr<Quakman::Threads::MainToGraphicMailBox> mailBox)
{
    bool receivedExitCommand = false;
    auto graphicalEngine = 
        std::shared_ptr<Quakman::Graphics::AbstractGraphicalOutput>
            (new Quakman::Graphics::SfmlGraphicalEngine(800, 600));

    while(!receivedExitCommand)
    {

        auto startTime = 
            std::chrono::round<std::chrono::milliseconds>
                (std::chrono::steady_clock::now());

        mailBox->mutex.lock();
        if (mailBox->downwardMessages.empty())
        {
            mailBox->mutex.unlock();
            continue;
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
                return;

            case Quakman::Threads::GraphicalThreadCommands::PAUSE:
            default:
                continue;
        }

        auto endTime = 
            std::chrono::round<std::chrono::milliseconds>
                (std::chrono::steady_clock::now());

        auto processTime = endTime - startTime;

        const int maxFPS = 60;
        auto remainTime = processTime % (1000 / maxFPS);

        std::this_thread::sleep_for(remainTime);
    }
}

void RunMainThread()
{
    auto mainToGraphicMailBox = 
        std::shared_ptr<Quakman::Threads::MainToGraphicMailBox>
                (new Quakman::Threads::MainToGraphicMailBox());

    std::thread graphicalThread (RunGraphicalThread, mainToGraphicMailBox);

    Quakman::Graphics::Drawable drawableWall {
        .pathToAtlas = "../res/quakman-atlas.jpg",
        .atlasCutStart = {0,0},
        .atlasCutEnd = {40, 40}
    };

    mainToGraphicMailBox->mutex.lock();
    mainToGraphicMailBox->downwardMessages.push
        ({.command= Quakman::Threads::GraphicalThreadCommands::LOAD, 
            .position = {400,200}, .data = drawableWall}
        );
    mainToGraphicMailBox->mutex.unlock();

    while (true) 
    {
        mainToGraphicMailBox->downwardMessages.push
        ({.command= Quakman::Threads::GraphicalThreadCommands::DRAW, 
            .position = {400,200}, .data = drawableWall}
        );
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }

    graphicalThread.join();


}

int main()
{
    RunMainThread();
    return 0;
}