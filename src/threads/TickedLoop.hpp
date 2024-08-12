#pragma once


namespace Quakman::Threads 
{
    class TickedLoop
    {
    public:
        void SetMaxTicksPerSecond(int maxTicks);

        void Run();

    protected:
        virtual bool WorkWithMailBox() = 0;   

    private:
        int maxTicksPerSec = 60;
    };
}