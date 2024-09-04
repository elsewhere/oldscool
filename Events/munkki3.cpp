#include "munkki3.hpp"
#include "../console.hpp"


munkki3::munkki3()
{
    ball = new Mesh();
//    ball->buildSphere(1.0f, 16, 16, Vector(1, 1, 1));

//    ball->buildTorus(2, 1.0f, 32, 32);

    ball->buildEmpty(64, 64);
//    ball->calculateNormals();

//    ball->scale(1.7f);
}

munkki3::~munkki3()
{
    delete ball;
}

void munkki3::setTexture(Texture *text)
{
    texture = text;
}

void munkki3::draw(float time)
{
    if (!isActive(time))
        return;

    int i;
    int x, y; 

    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glDepthMask(0);

    glRotatef(80-framePosition*130, 0.3f, 0.7f-cosf(framePosition*6), 0.8f-0.4f*sinf(framePosition*11));

    glBindTexture(GL_TEXTURE_2D, texture->getID());
    Primitives::texturedCube(200, 2);
    glDepthMask(1);

    glLoadIdentity();
    glTranslatef(sinf(framePosition*6), 0.6f*cosf(framePosition*11), -11);
    glRotatef(50+framePosition*460, 0.3f+cosf(framePosition*7)*0.4f, 0.2f - framePosition * 0.4f, 0.4f + framePosition * 0.3f);

    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1,1,1,1);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    Vertex *vertices = ball->getVertices();

    int yres = 64;
    int xres = 64;

    float radius1 = 2.5f + 0.5f*sinf(framePosition*34);
    //ulompi kehä
    for (y=0;y<yres;y++)
    {
        float ykohta = y / (float)yres;
        float yangle = ykohta*2*3.141592f;

        //torus tulee xz-tasoon
        Matrix rotation;
        rotation.makeRotation(0, yangle, 0);

        Vector centerpoint = Vector(0, 0, radius1);

        float radius2 = 1.0f + 0.4f*(float)cos(ykohta*4*3.141592f+framePosition*55);

        //sisempi kehä
        for (x=0;x<xres;x++)
        {
            float xkohta = x / (float)xres;
            float xangle = xkohta*2*3.141592f;

            Vector edgepoint = Vector(0, (float)sin(xangle), (float)cos(xangle))*radius2
                *(float)(1.0f+0.1f*cos(xkohta*11*3.141592f+framePosition*35)) ;
            vertices[x+y*xres].position = (centerpoint + edgepoint) * rotation;
        }
    }
    ball->makeFaces(xres, yres);
    ball->calculateNormals();







    int vcount = ball->getVertexCount();
    Vertex *vbuf = ball->getVertices();

    int fcount = ball->getFaceCount();
    Face *faces = ball->getFaces();


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->getID());
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

    glEnable(GL_DEPTH_TEST);
    glBegin(GL_TRIANGLES);
    for (i=0;i<fcount;i++)
    {
        Vertex *v1 = &vbuf[faces[i].v1];
        Vertex *v2 = &vbuf[faces[i].v2];
        Vertex *v3 = &vbuf[faces[i].v3];

//        glNormal3fv((float *)&faces[i].normal);
        glNormal3fv((float *)&v1->normal);
        glVertex3fv((float *)&v1->position);
        glNormal3fv((float *)&v2->normal);
        glVertex3fv((float *)&v2->position);
        glNormal3fv((float *)&v3->normal);
        glVertex3fv((float *)&v3->position);
    }
    glEnd();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

/*
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	myEnable(MY_TEXTURE_GEN_S);
	myEnable(MY_TEXTURE_GEN_T);
*/
}

bool munkki3::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool munkki3::free()
{
    return true;
}