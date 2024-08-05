#include "Drawable.hpp"

using namespace Quakman::Graphics;

bool Drawable::operator==(const Drawable& other) const
{
    return pathToAtlas == other.pathToAtlas
            && atlasCutStart == other.atlasCutStart
            && atlasCutEnd == other.atlasCutEnd;
}
