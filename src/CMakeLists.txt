
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(QuakMan main.cpp

    audio/AbstractAudioOutput.hpp
    audio/SfmlAudioEngine.hpp audio/SfmlAudioEngine.cpp
    audio/Soundable.hpp audio/Soundable.cpp

    graphics/AbstractGraphicalOutput.hpp
    graphics/Drawable.hpp graphics/Drawable.cpp
    graphics/SfmlGraphicalEngine.hpp graphics/SfmlGraphicalEngine.cpp
    graphics/Writeable.hpp

    math/Vector2i.hpp math/Vector2i.cpp

    threads/MailBox.hpp
    threads/Messages.hpp
    threads/Report.hpp
)

target_link_libraries(QuakMan sfml-system sfml-graphics sfml-audio)