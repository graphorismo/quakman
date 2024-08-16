#include <variant>

#include "RaylibGraphicalEngine.hpp"
#include "Drawable.hpp"
#include "Writeable.hpp"

using namespace Quakman::Graphics;

RaylibGraphicalEngine::RaylibGraphicalEngine(std::int16_t width, std::int16_t height)
{
    InitWindow(width, height, "Quakman");
    

}

RaylibGraphicalEngine::~RaylibGraphicalEngine()
{
    for (auto pair : drawableToTexture)
    {
        ::UnloadTexture(pair.second);
    }
    ::CloseWindow();
}

void RaylibGraphicalEngine::LoadTexture(Drawable drawable)
{
    Texture2D texture = ::LoadTexture(drawable.pathToAtlas.data());
    drawableToTexture.insert_or_assign(drawable, texture);
}

void RaylibGraphicalEngine::Draw(Drawable drawable, Math::Vector2i position)
{
    drawQueue.push({drawable, position});
    DrawQueue();
}

void RaylibGraphicalEngine::Draw(Writeable writeable, Math::Vector2i position)
{
    drawQueue.push({writeable, position});
    DrawQueue();
}

void RaylibGraphicalEngine::DrawQueue()
{
    auto frontQueuePair = drawQueue.front();
    drawQueue.pop();

    Vector2 position = { frontQueuePair.second.x, frontQueuePair.second.y };
    auto able = frontQueuePair.first;

    ::BeginDrawing();
    ::ClearBackground(RAYWHITE);
    if (std::holds_alternative<Drawable>(able))
    {
        auto drawable = std::get<Drawable>(able);
        auto texture = drawableToTexture[drawable];
        Rectangle cutArea = { drawable.atlasCutStart.x, drawable.atlasCutStart.y,
                             drawable.atlasCutEnd.x - drawable.atlasCutStart.x, 
                             drawable.atlasCutEnd.y - drawable.atlasCutStart.y };
        ::DrawTextureRec(texture, cutArea, position, WHITE);
    }
    else
    {
        auto writable = std::get<Writeable>(able);
        ::DrawText(writable.text.data(), position.x, position.y, writable.fontSize, BLACK);
    }
    ::EndDrawing();
}