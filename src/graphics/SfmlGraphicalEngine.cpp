#include "SfmlGraphicalEngine.hpp"
#include "Drawable.hpp"
#include "Writeable.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <bits/chrono.h>
#include <chrono>
#include <variant>

using namespace Quakman::Graphics;

SfmlGraphicalEngine::SfmlGraphicalEngine(std::int16_t width, std::int16_t height, std::int8_t maxFPS)
: window(sf::VideoMode(width, height), "Quakman"),
  timePerFrame(std::chrono::milliseconds{1000 / maxFPS}),
  maxFPS(maxFPS)
{

}

void SfmlGraphicalEngine::LoadTexture(Drawable drawable)
{
    auto texture = sf::Texture();
    auto cutRectangle = 
        sf::IntRect(drawable.atlasCutStart.x,
                    drawable.atlasCutStart.y,
                    drawable.atlasCutEnd.x - drawable.atlasCutStart.x, 
                    drawable.atlasCutEnd.y - drawable.atlasCutStart.y);
    texture.loadFromFile(drawable.pathToAtlas, cutRectangle);
    drawableToTexture.insert_or_assign(drawable, texture);
}

void SfmlGraphicalEngine::Draw(Drawable drawable, Math::Vector2i position)
{
    drawQueue.push({drawable, position});
    if (CanDrawSingleFrame()) DrawQueue();
}

void SfmlGraphicalEngine::Draw(Writeable writeable, Math::Vector2i position) 
{
    drawQueue.push({writeable, position});
    if (CanDrawSingleFrame()) DrawQueue();
}

bool SfmlGraphicalEngine::CanDrawSingleFrame()
{
    auto timeDifference = std::chrono::steady_clock::now() - startTime;
    auto diffInMillisec = 
        std::chrono::round<std::chrono::milliseconds>(timeDifference);
    if(diffInMillisec / timePerFrame > fpsCounter)
    {
        fpsCounter <= maxFPS ? fpsCounter+=1 : fpsCounter=0;
        return true;
    }
    return false;
}

void SfmlGraphicalEngine::DrawQueue()
{
    window.clear();
    while(drawQueue.size() > 0)
    {
        auto pair = drawQueue.front();
        drawQueue.pop();
        if(std::holds_alternative<Drawable>(pair.first))
        {
            auto drawable = std::get<Drawable>(pair.first);
            sf::Sprite sprite;
            if (drawableToTexture.find(drawable) == drawableToTexture.end()) LoadTexture(drawable);
            sprite.setTexture(drawableToTexture[drawable]);

            sprite.setPosition(pair.second.x, pair.second.y);
            window.draw(sprite);
        }
        else 
        {
            auto writable = std::get<Writeable>(pair.first);
            sf::Text text;
            text.setString(writable.text);
            text.setCharacterSize(writable.fontSize);
            window.draw(text);
        }
    }
    window.display();
}