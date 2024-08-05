#include "Vector2i.hpp"

using namespace Quakman::Math;

bool Vector2i::operator==(const Vector2i& other) const
{
    return x == other.x && y == other.y;
}