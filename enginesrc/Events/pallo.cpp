#include "pallo.hpp"
#include "../console.hpp"


pallo::pallo()
{
    ball = new Mesh();
    ball->buildSphere(1.0f, 16);
    ball->calculateNormals();
}

pallo::~pallo()
{
    delete ball;
}

void pallo::draw(float time)
{
    if (!isActive(time))
        return;

    int i;

    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glTranslatef(0, 0, -7);
    glRotatef(time*0.1f, 1, 1, 1);

    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1,1,1,1);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    int vcount = ball->getVertexCount();
    Vertex *vbuf = ball->getVertices();

    int c = 0;
    glBegin(GL_POINTS);
    for (i=0;i<vcount;i++)
    {
        glVertex3fv((float *)&vbuf[i].position);
    }
    glEnd();

    glBegin(GL_LINES);
    for (i=0;i<vcount;i++)
    {
        glColor4f(1,0,0, 1);
        glVertex3fv((float *)&(vbuf[i].position*1.2f));
        glColor4f(0,0,0, 1);
        glVertex3fv((float *)&(vbuf[i].position*1.2f + vbuf[i].normal));
    }
    glEnd();


}

bool pallo::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool pallo::free()
{
    return true;
}