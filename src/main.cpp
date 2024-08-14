
#include <glog/logging.h>

#include <cmath>
#include <memory>

#include "threads/AudioTickedLoop.hpp"
#include "threads/Messages.hpp"
#include "threads/GraphicalTickedLoop.hpp"


int main(int argc, char* argv[])
{
    fLB::FLAGS_alsologtostderr = 1;
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InstallFailureSignalHandler();
    
    auto mainToGraphicMailBox = 
        std::shared_ptr<Quakman::Threads::MainToGraphicMailBox>
                (new Quakman::Threads::MainToGraphicMailBox());
    DLOG(INFO) << "Create a mail box between main and graphical threads"; 

    auto graphicalTickedLoop = Quakman::Threads::GraphicalTickedLoop(mainToGraphicMailBox);
    graphicalTickedLoop.SetMaxTicksPerSecond(1);
    DLOG(INFO) << "Initialize a Graphical Ticked Loop object";

    auto mainToAudioMailBox =
        std::shared_ptr<Quakman::Threads::MainToAudioMailBox>
            (new Quakman::Threads::MainToAudioMailBox());
    DLOG(INFO) << "Create a mail box between main and audio threads.";

    auto audioTickedLoop = Quakman::Threads::AudioTickedLoop(mainToAudioMailBox);
    audioTickedLoop.SetMaxTicksPerSecond(1);
    DLOG(INFO) << "Initialize an Audio Ticked Loop object.";
   
    std::thread graphicalThread (&Quakman::Threads::GraphicalTickedLoop::Run, &graphicalTickedLoop);
    DLOG(INFO) << "Start a Graphical thread.";
    std::thread audioThread (&Quakman::Threads::AudioTickedLoop::Run, &audioTickedLoop);
    DLOG(INFO) << "Start an Audio thread.";

    Quakman::Graphics::Drawable drawableWall {
        .pathToAtlas = "../res/quakman-atlas.jpg",
        .atlasCutStart = {0,0},
        .atlasCutEnd = {40, 40}
    };

    Quakman::Audio::Soundable playableSound {
        .pathToFile = "../res/WOW.wav",
        .startTime = 0.0f
    };
    DLOG(INFO) << "Initialize a Drawable and a Playable test objects.";

    mainToGraphicMailBox->mutex.lock();
    mainToGraphicMailBox->downwardMessages.push
        ({.command= Quakman::Threads::GraphicalThreadCommands::LOAD, 
            .position = {400,200}, .data = drawableWall}
        );
    mainToGraphicMailBox->mutex.unlock();
    DLOG(INFO) << "Command a Graphical thread to load the test Drawable.";

    mainToAudioMailBox->mutex.lock();
    mainToAudioMailBox->downwardMessages.push
        ( {.command=Quakman::Threads::AudioThreadCommands::LOAD,
              .volume = 100.0f, .pitch = 1.0f, .data = playableSound }
        );
    mainToAudioMailBox->mutex.unlock();
    DLOG(INFO) << "Command an Audio thread to load the test Playable.";

/* 
    float runner = 0.0f;
    DLOG(INFO) << "Start a Main Loop.";
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
        DLOG(INFO) << "Command the Graphical thread to draw the test Drawable.";
        
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
            DLOG(INFO) << "Command the Audio thread to play the test Playable.";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    */
    graphicalThread.join();
    audioThread.join();
    
    return 0;
}