#ifndef _tunneli_HPP_
#define _tunneli_HPP_

#include "../global.h"

class tunneli : public Event
{
public:
    tunneli();
    ~tunneli();

    bool init(float start, float end);
    bool free();
    void draw(float time);
    void setTexture(Texture *text);

private:
	int xres;
	int yres;

	int vertexcount;
    Vertex *vertices;
	Spline *kamera;
	Vector pathfunc1(float t);
	Vector pathfunc2(float t);
    Texture *texture;
    float *effect;

};


#endif