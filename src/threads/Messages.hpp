#pragma once

#include <variant>

#include "../math/Vector2i.hpp"
#include "../graphics/Drawable.hpp"
#include "../graphics/Writeable.hpp"
#include "../audio/Soundable.hpp"
#include "MailBox.hpp"
#include "Report.hpp"

namespace Quakman::Threads 
{
    enum class GraphicalThreadCommands
    {
        DRAW,
        LOAD,
        REPORT,
        PAUSE,
        EXIT
    };

    struct GraphicalThreadInput
    {
        GraphicalThreadCommands command; 
        Quakman::Math::Vector2i position;
        std::variant<Quakman::Graphics::Drawable, Quakman::Graphics::Writeable> data;
    };

    using MainToGraphicMailBox = MailBox<Report, GraphicalThreadInput>;

    enum class AudioThreadCommands
    {
        PLAY,
        LOAD,
        REPORT,
        PAUSE,
        EXIT
    };

    struct AudioThreadInput
    {
        AudioThreadCommands command; 
        float volume;
        float pitch;
        Quakman::Audio::Soundable data;
    };

    using MainToAudioMailBox = MailBox<Report, AudioThreadInput>;
}