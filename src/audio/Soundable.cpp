#include "Soundable.hpp"

bool Quakman::Audio::Soundable::operator==(const Soundable& other) const
{
    return pathToFile == other.pathToFile && startTime == other.startTime;

}