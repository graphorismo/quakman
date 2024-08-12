#include "SfmlControlEngine.hpp"
#include "AbstractControlInput.hpp"
#include "SFML/Window/Keyboard.hpp"

using namespace Quakman::Control;


Controls SfmlControlEngine::GetPressedControl()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        return Controls::UP;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        return Controls::RIGHT;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        return Controls::DOWN;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        return Controls::LEFT;
    else
        return Controls::EMPTY;
}

