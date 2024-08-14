
include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG v0.7.1)
FetchContent_MakeAvailable(glog)

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
    threads/AudioTickedLoop.hpp  threads/AudioTickedLoop.cpp
    threads/GraphicalTickedLoop.hpp  threads/GraphicalTickedLoop.cpp
    threads/TickedLoop.hpp  threads/TickedLoop.cpp
)

target_link_libraries(QuakMan glog sfml-system sfml-graphics sfml-audio)