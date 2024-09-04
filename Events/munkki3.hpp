#ifndef _munkki3_HPP_
#define _munkki3_HPP_

#include "../global.h"

class munkki3 : public Event
{
public:
    munkki3();
    ~munkki3();

    bool init(float start, float end);
    bool free();
    void draw(float time);
    void setTexture(Texture *text);

private:
    Mesh *ball;
    Texture *texture;

};


#endif