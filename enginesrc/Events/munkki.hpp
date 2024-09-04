#ifndef _munkki_HPP_
#define _munkki_HPP_

#include "../global.h"

class munkki : public Event
{
public:
    munkki();
    ~munkki();

    bool init(float start, float end);
    bool free();
    void draw(float time);
    void setTexture(Texture *text);

private:
    Mesh *ball;
    Texture *texture;

};


#endif