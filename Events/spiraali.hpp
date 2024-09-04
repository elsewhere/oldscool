#ifndef _spiraali_HPP_
#define _spiraali_HPP_

#include "../global.h"

class spiraali : public Event
{
public:
    spiraali();
    ~spiraali();

    bool init(float start, float end);
    bool free();
    void draw(float time);

private:
    int xres, yres;
    float *table;

};


#endif