#ifndef _picfadeout_HPP_
#define _picfadeout_HPP_

#include "../global.h"

class picfadeout : public Event
{
public:
    picfadeout();
    ~picfadeout();

    void setTexture(Texture *text);
    void setFadeTime(float fadetime);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    Texture *texture;
    float fadetime;


};


#endif