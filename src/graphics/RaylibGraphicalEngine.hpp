#pragma once

#include <chrono>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <utility>
#include <variant>

#include <raylib.h>

#include "AbstractGraphicalOutput.hpp"
#include "Drawable.hpp"
#include "Writeable.hpp"



namespace Quakman::Graphics
{
    class RaylibGraphicalEngine : public AbstractGraphicalOutput
    {
    public:
        RaylibGraphicalEngine(std::int16_t width, std::int16_t height);
        virtual ~RaylibGraphicalEngine();

        virtual void LoadTexture(Drawable drawable) override;
        virtual void Draw(Drawable drawable, Math::Vector2i position) override;
        virtual void Draw(Writeable writeable, Math::Vector2i position) override;

    private:
        void DrawQueue();

        std::queue<std::pair<std::variant<Drawable,Writeable>, Math::Vector2i>> drawQueue;
        std::unordered_map<Drawable, Texture2D> drawableToTexture;
        std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
    };
}