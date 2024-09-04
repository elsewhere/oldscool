#ifndef _tunneli2_HPP_
#define _tunneli2_HPP_

#include "../global.h"

class tunneli2 : public Event
{
public:
    tunneli2();
    ~tunneli2();

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


};


#endif