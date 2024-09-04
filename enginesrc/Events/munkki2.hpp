#ifndef _munkki2_HPP_
#define _munkki2_HPP_

#include "../global.h"

class munkki2 : public Event
{
public:
    munkki2();
    ~munkki2();

    bool init(float start, float end);
    bool free();
    void draw(float time);
    void setTexture(Texture *text);

private:
    Mesh *ball;
    Texture *texture;

};


#endif