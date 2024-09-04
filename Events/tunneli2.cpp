#include "tunneli2.hpp"


Vector tunneli2::pathfunc1(float t)
{
	float r = 1.2f;//0.8f+0.2f*(float)sin(t*2*3.141592f*3);
	float x = r*5*(float)sin(t*2*3.141592f);
	float y = r*5*(float)cos(t*2*3.141592f);
	float z = r*3*(float)sin(t*2*3.141592f*4);

	return Vector(x, y, z);
}
Vector tunneli2::pathfunc2(float t)
{
	float r = 0.8f+0.2f*(float)sin(t*2*3.141592f*3);
	float x = r*5*(float)sin(t*2*3.141592f);
	float y = r*5*(float)cos(t*2*3.141592f*3);
	float z = r*3*(float)sin(t*2*3.141592f*2);
	return Vector(x, y, z);
}

#define SCALE 5

tunneli2::tunneli2()
{
	float t = 0.0f;
	yres = 400;
	xres = 40;
	const int splinepoints = 200;

	vertexcount = xres*yres;

	vertices = new Vertex[vertexcount];

	kamera = new Spline();
	for (t = 0.0f;t<1.0f; t+=1.0f/splinepoints)
	{
		Vector p = pathfunc2(t)*SCALE;
        kamera->addpoint(p);
	}

}

tunneli2::~tunneli2()
{
}

void tunneli2::setTexture(Texture *text)
{
    texture = text;
}

void tunneli2::draw(float time)
{
	int x, y;

	const float kameraspeed = 0.9f;
	const float epsilon = 0.03f;

	float kamerapos = fmodf(framePosition*kameraspeed, 1);

    if (kamerapos > 1.0f)
		kamerapos -= 1.0f;
	float kamerapos2 = kamerapos + epsilon;
	if (kamerapos2 > 1.0f)
		kamerapos2 -= 1.0f;
	if (kamerapos2 < 0.0f)
		kamerapos2 = 1+kamerapos2;

	Vector cam1 = kamera->getValue(kamerapos);
	Vector cam2 = kamera->getValue(kamerapos2);
	Vector camup = Vector(0, 1, 0);

    cam1 *= SCALE;
    cam2 *= SCALE;

	glLoadIdentity();
	gluLookAt(cam1.x, cam1.y, cam1.z, cam2.x, cam2.y, cam2.z, camup.x, camup.y, camup.z);

    glEnable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);
    glDepthMask(0);
    Primitives::texturedCube(1000, 1);
    glBindTexture(GL_TEXTURE_2D, texture->getID());

    glDepthMask(1);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(2.0f);
    
    glColor4f(0.7f,0.7f,1,0.9f);


	for (y=0;y<yres;y++)
	{
		const float ykohta = y / (float)yres;
		const float epsilon = 0.01f;

		Vector p0 = pathfunc2(ykohta)*SCALE;
		Vector p1 = pathfunc2(ykohta+epsilon)*SCALE;

		Vector T = (p1-p0);//.normalize();
		Vector N = (p1+p1);//.normalize();
		Vector B = T.crossProduct(N);
		N = B.crossProduct(T);

		B.normalize();
		N.normalize();

		for (x=0;x<xres;x++)
		{
			const float xkohta = x / (float)(xres-1);
			const float u = xkohta*2*3.141592f + framePosition * 7;
			const float radius = 1.25f + 0.3f*(float)cos(ykohta*17*2*3.141592f + framePosition*75) 
                + 0.15f*(float)cos(xkohta*5*2*3.141592f+ ykohta*15 + framePosition*7);

			Vector p = Vector((float)sin(u), (float)cos(u), 0)*radius;
			Vector np = N*p.y + B*p.x;

            vertices[x+y*xres].position = np+p0;
		}
	}

    const int colorcount = 5;
    static const unsigned char colors[colorcount][3] = 
    {
        {0x66, 0x01, 0xFF},
        {0xF2, 0x19, 0x70},
        {0x00, 0xCC, 0xFF},
        {0xFF, 0x61, 0x00},
        {0xFF, 0x06, 0x00}
    };

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    for (y=0;y<yres-1;y+=4)
	{
        glColor3ubv((unsigned char *)&colors[(y >> 2) % colorcount]);
//        glColor4f(0,0,1,1);
//		glBegin(GL_POINTS);
        glBegin(GL_QUAD_STRIP);

        for (x=0;x<xres;x++)
		{
			Vector v1 = vertices[x+y*xres].position*SCALE;
			Vector v2 = vertices[x+(y+1)*xres].position*SCALE;
			glVertex3fv((float *)&v1);
			glVertex3fv((float *)&v2);
		}
		glEnd();
	}
}

bool tunneli2::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool tunneli2::free()
{
    return true;
}