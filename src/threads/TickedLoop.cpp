#include <chrono>
#include <thread>

#include "TickedLoop.hpp"

using namespace Quakman::Threads;

void TickedLoop::SetMaxTicksPerSecond(int maxTicks)
{
    maxTicksPerSec = maxTicks;
}

void TickedLoop::Run()
{
    bool shouldExit= false;
    while (!shouldExit) 
    {
        auto startTime = 
            std::chrono::round<std::chrono::milliseconds>
                (std::chrono::steady_clock::now());

        shouldExit = WorkWithMailBox();

        auto endTime = 
            std::chrono::round<std::chrono::milliseconds>
                (std::chrono::steady_clock::now());

        auto processTime = endTime - startTime;
        auto remainTime = processTime % (1000 / maxTicksPerSec);
        std::this_thread::sleep_for(remainTime);
    
    }
}