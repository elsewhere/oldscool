#ifndef _sirpale_HPP_
#define _sirpale_HPP_

#include "../global.h"

class sirpaleitem
{
public:
    Vertex v1, v2, v3;
    Vector pos;
    Vector d;
    Vector rotspeed;
    
};

class sirpale : public Event
{
public:
    sirpale();
    ~sirpale ();

    void setTexture(Texture *text);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    int xres;
    int yres;
    int itemcount;
    sirpaleitem *items;

    Texture *texture;


};


#endif