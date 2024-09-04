#include "runko.hpp"


runko::runko()
{
}

runko::~runko()
{
}

void runko::draw(float time)
{
}

bool runko::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool runko::free()
{
    return true;
}