#include <glog/logging.h>

#include <chrono>

#include "TickedLoop.hpp"

using namespace Quakman::Threads;
using namespace std::chrono_literals;

void TickedLoop::SetMaxTicksPerSecond(int maxTicks)
{
    if (maxTicks > 0)
        maxTicksPerSec = maxTicks;
    else
        maxTicksPerSec = 1;
}

void TickedLoop::Run()
{
    VLOG(2) <<"  Starting the Ticked Loop.";
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
        auto remainTime = (1000ms / maxTicksPerSec) - processTime;
        std::this_thread::sleep_for(std::chrono::milliseconds(remainTime));
    
    }
}