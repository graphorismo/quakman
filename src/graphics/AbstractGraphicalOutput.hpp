#pragma once

#include "Drawable.hpp"
#include "Writeable.hpp"

namespace Quakman::Graphics
{
    
    class AbstractGraphicalOutput
    {
    public:
        virtual void LoadTexture(Drawable drawable) = 0;
        virtual void Draw(Drawable drawable, Math::Vector2i position) = 0;
        virtual void Draw(Writeable writeable, Math::Vector2i position) = 0;

        virtual ~AbstractGraphicalOutput() {};
    };
}