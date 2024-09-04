#ifndef _vilkku_HPP_
#define _vilkku_HPP_

#include "../global.h"

class vilkkuitem
{
public:
    Vector pos;

    float uv1[2];
    float uv2[2];

    float alpha;
    float phase;
    float speed;
};

class vilkku : public Event
{
public:
    vilkku();
    ~vilkku ();

    void setTexture(Texture *text);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    int xres;
    int yres;
    vilkkuitem *items;

    Texture *texture;


};


#endif