#ifndef _pink_HPP_
#define _pink_HPP_

#include "../global.h"

class pinkitem
{
public:
    Vector position;
    Vector rotation;

    float size;
    float starttime;
    float endtime;


};
class pink : public Event
{
public:
    pink();
    ~pink();

    bool init(float start, float end);
    bool free();
    void draw(float time);
    void setTexture(Texture *text);

private:

    int itemcount;
    int streamcount;

    
    pinkitem **streams;

    Texture *texture;


};


#endif