#ifndef _levy_HPP_
#define _levy_HPP_

#include "../global.h"

class levyitem
{
public:
    Vector pos;

    float u;
    float v;

    float alpha;
    float phase;
    float speed;
};

class levy : public Event
{
public:
    levy();
    ~levy ();

    void setTexture(Texture *text);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    int xres;
    int yres;
    levyitem *items;

    Texture *texture;


};


#endif