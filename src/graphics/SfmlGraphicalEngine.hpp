#pragma once

#include "AbstractGraphicalOutput.hpp"
#include "Drawable.hpp"
#include "Writeable.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Window.hpp>
#include <chrono>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <utility>
#include <variant>

namespace Quakman::Graphics
{
    class SfmlGraphicalEngine : public AbstractGraphicalOutput
    {
    public:
        SfmlGraphicalEngine(std::int16_t width, std::int16_t height);

        virtual void LoadTexture(Drawable drawable) override;
        virtual void Draw(Drawable drawable, Math::Vector2i position) override;
        virtual void Draw(Writeable writeable, Math::Vector2i position) override;

    private:
        bool CanDrawSingleFrame();
        void DrawQueue();

        sf::RenderWindow window;
        std::queue<std::pair<std::variant<Drawable,Writeable>, Math::Vector2i>> drawQueue;
        std::unordered_map<Drawable, sf::Texture> drawableToTexture;
        std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
    };
}