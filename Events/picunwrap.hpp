#ifndef _picunwrap_HPP_
#define _picunwrap_HPP_

#include "../global.h"

class picunwrapitem
{
public:
    Vector pos;

    float uv1[2];
    float uv2[2];

    float alpha;
    float phase;
    float speed;
};

class picunwrap : public Event
{
public:
    picunwrap();
    ~picunwrap ();

    void setTexture(Texture *text);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    int xres;
    int yres;
    picunwrapitem *items;

    Texture *texture;


};


#endif