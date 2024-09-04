#ifndef _FONT_HPP_
#define _FONT_HPP_

#include "stuff.hpp"

class Letter
{
public:
    Letter() {};
    ~Letter() {};

    float u1, v1;
    float u2, v2;
};

class Fontsystem
{
public:

    Fontsystem() {};
    ~Fontsystem() {};


private:

    Letter *letters;

};




#endif