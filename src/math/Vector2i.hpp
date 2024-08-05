#pragma once

#include <cstdint>
#include <functional>

namespace Quakman::Math 
{
    struct Vector2i
    {
        std::int16_t x = 0;
        std::int16_t y = 0;

        bool operator==(const Vector2i&) const;
    };

}

template<>
struct std::hash<Quakman::Math::Vector2i>
{
    std::size_t operator()(const Quakman::Math::Vector2i& vect) const noexcept
    {
        std::size_t h1 = vect.x;
        std::size_t h2 = vect.y;
        return h1 ^ (h2 << 1);
    }
};
