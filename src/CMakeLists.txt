
include(FetchContent)

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG v0.7.1)
FetchContent_MakeAvailable(glog)

add_executable(QuakMan main.cpp

    audio/AbstractAudioOutput.hpp
    audio/RaylibAudioEngine.hpp audio/RaylibAudioEngine.cpp
    audio/Soundable.hpp audio/Soundable.cpp

    graphics/AbstractGraphicalOutput.hpp
    graphics/Drawable.hpp graphics/Drawable.cpp
    graphics/RaylibGraphicalEngine.hpp graphics/RaylibGraphicalEngine.cpp
    graphics/Writeable.hpp

    math/Vector2i.hpp math/Vector2i.cpp

    threads/MailBox.hpp
    threads/Messages.hpp
    threads/Report.hpp
    threads/AudioTickedLoop.hpp  threads/AudioTickedLoop.cpp
    threads/GraphicalTickedLoop.hpp  threads/GraphicalTickedLoop.cpp
    threads/TickedLoop.hpp  threads/TickedLoop.cpp
)

target_link_libraries(QuakMan glog raylib)