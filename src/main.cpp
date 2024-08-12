
#include <chrono>
#include <cmath>
#include <memory>
#include <thread>

#include "audio/Soundable.hpp"
#include "graphics/Drawable.hpp"
#include "threads/AudioTickedLoop.hpp"
#include "threads/Messages.hpp"
#include "threads/GraphicalTickedLoop.hpp"


void RunMainThread()
{
    auto mainToGraphicMailBox = 
        std::shared_ptr<Quakman::Threads::MainToGraphicMailBox>
                (new Quakman::Threads::MainToGraphicMailBox());

    auto graphicalTickedLoop = Quakman::Threads::GraphicalTickedLoop(mainToGraphicMailBox);
    graphicalTickedLoop.SetMaxTicksPerSecond(60);

    auto mainToAudioMailBox =
        std::shared_ptr<Quakman::Threads::MainToAudioMailBox>
            (new Quakman::Threads::MainToAudioMailBox());

    auto audioTickedLoop = Quakman::Threads::AudioTickedLoop(mainToAudioMailBox);
    audioTickedLoop.SetMaxTicksPerSecond(60);

    std::thread graphicalThread (&Quakman::Threads::GraphicalTickedLoop::Run, &graphicalTickedLoop);
    std::thread audioThread (&Quakman::Threads::AudioTickedLoop::Run, &audioTickedLoop);

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