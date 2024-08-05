#pragma once

#include "../math/Vector2i.hpp"
#include <cstddef>
#include <functional>
#include <string>

namespace Quakman::Graphics
{
    struct Drawable
    {
        std::string pathToAtlas {};
        Quakman::Math::Vector2i atlasCutStart {};
        Quakman::Math::Vector2i atlasCutEnd {};

        bool operator==(const Drawable&) const;
    };
}

template<>
struct std::hash<Quakman::Graphics::Drawable>
{
    std::size_t operator()(const Quakman::Graphics::Drawable& drawable) const noexcept
    {
        std::size_t h0 = std::hash<std::string>{}(drawable.pathToAtlas);
        std::size_t h1 = std::hash<Quakman::Math::Vector2i>{}(drawable.atlasCutStart);
        std::size_t h2 = std::hash<Quakman::Math::Vector2i>{}(drawable.atlasCutEnd);
        return h0 ^ (1 << (h1 ^ (h2 << 1)));
    }
};
