#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include "global.h"

class Event
{
public:

    Event();
    ~Event();

    bool isActive(float time);
    const int getPriority();
    void setPriority(int value);
    void frameUpdate(float time);

    virtual bool init(float start, float end) = 0;
    virtual bool free() = 0;
    virtual void draw(float time) = 0;

    bool operator < (Event &event);
    bool operator > (Event &event);

protected:
    int priority;
    float starttime, endtime;

    float dt;
    float previousPosition;
    float framePosition;




};



#endif