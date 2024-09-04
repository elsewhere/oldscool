#ifndef _picfadein_HPP_
#define _picfadein_HPP_

#include "../global.h"

class picfadein : public Event
{
public:
    picfadein();
    ~picfadein();

    void setTexture(Texture *text);
    void setFadeTime(float fadetime);
    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    Texture *texture;
    float fadetime;


};


#endif