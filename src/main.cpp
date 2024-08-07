
#include <chrono>
#include <cmath>
#include <memory>
#include <thread>
#include <variant>

#include "audio/AbstractAudioOutput.hpp"
#include "audio/SfmlAudioEngine.hpp"
#include "audio/Soundable.hpp"
#include "graphics/AbstractGraphicalOutput.hpp"
#include "graphics/Writeable.hpp"
#include "graphics/Drawable.hpp"
#include "graphics/SfmlGraphicalEngine.hpp"
#include "threads/Report.hpp"
#include "threads/Messages.hpp"


void RunAudioThread(std::shared_ptr<Quakman::Threads::MainToAudioMailBox> mailBox, int maxTicks)
{
    auto audioEngine = 
        std::shared_ptr<Quakman::Audio::AbstractAudioOutput>
            (new Quakman::Audio::SfmlAudioEngine());

    while(true)
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
                return;

            case Quakman::Threads::AudioThreadCommands::PAUSE:
            default:
                continue;
        }

        auto endTime = 
            std::chrono::round<std::chrono::milliseconds>
                (std::chrono::steady_clock::now());

        auto processTime = endTime - startTime;

        auto remainTime = processTime % (1000 / maxTicks);

        std::this_thread::sleep_for(remainTime);
    }
}

void RunGraphicalThread(std::shared_ptr<Quakman::Threads::MainToGraphicMailBox> mailBox, int maxFPS)
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

        auto remainTime = processTime % (1000 / maxFPS);

        std::this_thread::sleep_for(remainTime);
    }
}

void RunMainThread()
{
    auto mainToGraphicMailBox = 
        std::shared_ptr<Quakman::Threads::MainToGraphicMailBox>
                (new Quakman::Threads::MainToGraphicMailBox());

    std::thread graphicalThread (RunGraphicalThread, mainToGraphicMailBox, 60);

    auto mainToAudioMailBox =
        std::shared_ptr<Quakman::Threads::MainToAudioMailBox>
            (new Quakman::Threads::MainToAudioMailBox());

    std::thread audioThread (RunAudioThread, mainToAudioMailBox, 60);

    Quakman::Graphics::Drawable drawableWall {
        .pathToAtlas = "../res/quakman-atlas.jpg",
        .atlasCutStart = {0,0},
        .atlasCutEnd = {40, 40}
    };

    Quakman::Audio::Soundable playableSound {
        .pathToFile = "../res/WOW.wav",
        .startTime = 0.0f
    };

    mainToGraphicMailBox->mutex.lock();
    mainToGraphicMailBox->downwardMessages.push
        ({.command= Quakman::Threads::GraphicalThreadCommands::LOAD, 
            .position = {400,200}, .data = drawableWall}
        );
    mainToGraphicMailBox->mutex.unlock();

    mainToAudioMailBox->mutex.lock();
    mainToAudioMailBox->downwardMessages.push
        ( {.command=Quakman::Threads::AudioThreadCommands::LOAD,
              .volume = 100.0f, .pitch = 1.0f, .data = playableSound }
        );
    mainToAudioMailBox->mutex.unlock();

    float runner = 0.0f;
    while (true) 
    {
        mainToGraphicMailBox->mutex.lock();
        mainToGraphicMailBox->downwardMessages.push
        ({.command= Quakman::Threads::GraphicalThreadCommands::DRAW, 
            .position = {.x=static_cast<std::int16_t>(300+static_cast<int>(std::sin(runner))),
                        .y=static_cast<std::int16_t>(100+static_cast<int>(100*runner))}, 
                        .data = drawableWall}
        );
        mainToGraphicMailBox->mutex.unlock();
        
        runner+=0.05f;
        if(runner > 1.0f)
        {
            runner = 0.0f;
            mainToAudioMailBox->mutex.lock();
            mainToAudioMailBox->downwardMessages.push
                ( {.command=Quakman::Threads::AudioThreadCommands::PLAY,
                    .volume = 100.0f, .pitch = 1.0f, .data = playableSound }
                );
            mainToAudioMailBox->mutex.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }

    graphicalThread.join();
    audioThread.join();


}

int main()
{
    RunMainThread();
    return 0;
}