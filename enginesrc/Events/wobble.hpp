#ifndef _wobble_HPP_
#define _wobble_HPP_

#include "../global.h"

class wobbleitem
{
public:
    Vector pos;

    float u;
    float v;

    float alpha;
    float phase;
    float speed;
};

class wobble : public Event
{
public:
    wobble();
    ~wobble ();

    void setTexture(Texture *text);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    int xres;
    int yres;
    wobbleitem *items;

    Texture *texture;


};


#endif