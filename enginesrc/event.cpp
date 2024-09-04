#include "event.hpp"

Event::Event()
{
    priority = 0;
    dt = 0.0f;
    previousPosition = 0.0f;

}
Event::~Event()
{
}

const int Event::getPriority()
{
    return priority;
}

void Event::setPriority(int value)
{
    priority = value;
}

bool Event::operator < (Event &event)
{
    if (priority < event.getPriority())
        return true;
    else 
        return false;

}
bool Event::operator > (Event &event)
{
    if (this->getPriority() > event.getPriority())
        return true;
    else 
        return false;

}

bool Event::isActive(float time)
{
    if (time >= this->starttime && time < this->endtime)
        return true;
    else return false;
}

void Event::frameUpdate(float time)
{
    this->framePosition = calcPosFloat(time, starttime, endtime);
    this->dt = framePosition - previousPosition;
    this->previousPosition = this->framePosition;


}
