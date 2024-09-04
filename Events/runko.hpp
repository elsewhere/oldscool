#ifndef _runko_HPP_
#define _runko_HPP_

#include "../global.h"

class runko : public Event
{
public:
    runko();
    ~runko();

    bool init(float start, float end);
    bool free();
    void draw(float time);

private:

};


#endif