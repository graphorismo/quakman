#pragma once

#include "AbstractControlInput.hpp"

namespace Quakman::Control 
{
    
    class SfmlControlEngine : public AbstractControlInput
    {
    public:
        virtual Controls GetPressedControl() override;

    private:
    };
}