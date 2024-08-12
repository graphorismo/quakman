#pragma once

namespace Quakman::Control 
{
    enum class Controls
    {
        UNKNOWN = -1,
        EMPTY = 0,
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    class AbstractControlInput
    {
    public:
        virtual Controls GetPressedControl() = 0;
    };

}