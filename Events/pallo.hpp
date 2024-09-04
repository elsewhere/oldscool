#ifndef _pallo_HPP_
#define _pallo_HPP_

#include "../global.h"

class pallo : public Event
{
public:
    pallo();
    ~pallo();

    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    Mesh *ball;

};


#endif