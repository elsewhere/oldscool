#ifndef _picpulse_HPP_
#define _picpulse_HPP_

#include "../global.h"

class picpulse : public Event
{
public:
    picpulse();
    ~picpulse();

    void setTexture(Texture *text);
    void setMaxPulse(float max);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:

    float maxpulse;
    Texture *texture;


};


#endif