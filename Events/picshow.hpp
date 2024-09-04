#ifndef _picshow_HPP_
#define _picshow_HPP_

#include "../global.h"

class picshow : public Event
{
public:
    picshow();
    ~picshow();

    void setTexture(Texture *text);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    Texture *texture;


};


#endif