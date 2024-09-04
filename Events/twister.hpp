#ifndef _twister_HPP_
#define _twister_HPP_

#include "../global.h"

class twisterpoint
{
public:

    float x;
    float z;
    float rx;
    float rz;
    float y;

    float u, v;


};

class twister : public Event
{
public:
    twister();
    ~twister();

    void setTexture(Texture *text);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    int spancount;
    int pointsperline;
    
    float spanheight;
    twisterpoint **points;
    Texture *texture;


};


#endif