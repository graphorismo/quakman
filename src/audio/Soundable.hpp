#pragma once

#include <string>

namespace Quakman::Audio
{
    struct Soundable
    {
        std::string pathToFile;
        float startTime;

        bool operator==(const Soundable& other) const;
    };
}

template<>
struct std::hash<Quakman::Audio::Soundable>
{
    std::size_t operator()(const Quakman::Audio::Soundable& soudable) const noexcept
    {
        std::size_t h0 = std::hash<std::string>{}(soudable.pathToFile);
        std::size_t h1 = std::hash<float>{}(soudable.startTime);
        return h0 ^ (h1 << 1);
    }
};