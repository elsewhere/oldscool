#include "tunneli.hpp"


Vector tunneli::pathfunc1(float t)
{
	float r = 1.2f;//0.8f+0.2f*(float)sin(t*2*3.141592f*3);
	float x = r*5*(float)sin(t*2*3.141592f);
	float y = r*5*(float)cos(t*2*3.141592f);
	float z = r*3*(float)sin(t*2*3.141592f*4);

	return Vector(x, y, z);
}
Vector tunneli::pathfunc2(float t)
{
	float r = 0.8f+0.2f*(float)sin(t*2*3.141592f*5);
	float x = r*5*(float)sin(t*3*3.141592f);
	float y = r*5*(float)cos(t*3*3.141592f);
	float z = r*3*(float)sin(t*3*3.141592f*4);
	return Vector(x, y, z);
}

#define SCALE 5

tunneli::tunneli()
{
	float t = 0.0f;
	yres = 400;
	xres = 40;
	const int splinepoints = 200;

	vertexcount = xres*yres;

	vertices = new Vertex[vertexcount];

    effect = new float[vertexcount*4];

    for (int i=0;i<vertexcount;i++)
    {
        effect[i] = randFloat();
    }
	kamera = new Spline();
	for (t = 0.0f;t<1.0f; t+=1.0f/splinepoints)
	{
		Vector p = pathfunc1(t)*SCALE;
        kamera->addpoint(p);
	}

    int x, y;
	for (y=0;y<yres;y++)
	{
		const float ykohta = y / (float)yres;
		const float epsilon = 0.01f;

		Vector p0 = pathfunc1(ykohta)*SCALE;
		Vector p1 = pathfunc1(ykohta+epsilon)*SCALE;

		Vector T = (p1-p0);//.normalize();
		Vector N = (p1+p1);//.normalize();
		Vector B = T.crossProduct(N);
		N = B.crossProduct(T);

		B.normalize();
		N.normalize();

		for (x=0;x<xres;x++)
		{
			const float xkohta = x / (float)(xres-1);
			const float u = xkohta*2*3.141592f;
			const float radius = 1.5f;

			Vector p = Vector((float)sin(u), (float)cos(u), 0)*radius;
			Vector np = N*p.y + B*p.x;

            vertices[x+y*xres].position = np+p0;
		}
	}
}

tunneli::~tunneli()
{
    delete [] effect;
    delete [] vertices;
}
void tunneli::setTexture(Texture *text)
{
    texture = text;
}

void tunneli::draw(float time)
{
	int x, y;

	const float kameraspeed = 0.7f;
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

    int offset = 0;
    for (y=0;y<yres;y++)
    {
        float yv = 0.5f+0.5f*(float)(0.5f*cos(y*0.6+framePosition*17) + 0.5f*sin(y*0.38+framePosition*15));
        for (x=0;x<xres;x++)
        {
            float xv = 0.5f+0.5f*(float)(0.5f*cos(x*0.4+y*0.1f+framePosition*11) + 0.5f*sin(x*0.6+framePosition*23));

            float val = (xv+yv)*0.8f;

            effect[offset++] = val;
            effect[offset++] = val;
            effect[offset++] = val;
            effect[offset++] = 1.0f;
        }
    }

    offset = 0;
    for (y=0;y<yres;y++)
	{
		glBegin(GL_POINTS);
		for (x=0;x<xres;x++)
		{
			Vector v1 = vertices[x+y*xres].position*SCALE;
            glColor4fv((float *)&effect[offset]);//effect[offset], effect[offset], effect[offset], 1.0f);
			glVertex3fv((float *)&v1);
            offset+=4;
		}
		glEnd();
	}
}

bool tunneli::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool tunneli::free()
{
    return true;
}