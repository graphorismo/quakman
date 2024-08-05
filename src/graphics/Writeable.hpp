#pragma once

#include <cstdint>
#include <string>

namespace Quakman::Graphics 
{
    struct Writeable
    {
        std::string text;
        std::int8_t fontSize;
    };

}