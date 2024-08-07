#include "SfmlGraphicalEngine.hpp"
#include "Drawable.hpp"
#include "Writeable.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <bits/chrono.h>
#include <variant>

using namespace Quakman::Graphics;

SfmlGraphicalEngine::SfmlGraphicalEngine(std::int16_t width, std::int16_t height)
: window(sf::VideoMode(width, height), "Quakman")
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
    DrawQueue();
}

void SfmlGraphicalEngine::Draw(Writeable writeable, Math::Vector2i position) 
{
    drawQueue.push({writeable, position});
    DrawQueue();
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