#ifndef _SPLINE_HPP_
#define _SPLINE_HPP_

#include "global.h"

class Spline
{
public:
	Spline();
	~Spline();
	
	void destroy();
	void addpoint(Vector &point);

	void draw();
	Vector getValue(float t);

private:
	
	int count;
	Lista *points;

};



#endif