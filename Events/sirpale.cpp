#include "sirpale.hpp"

/*

class sirpale : public Event
{
public:
    sirpale();
    ~sirpale();

    bool init(float start, float end);
    bool free();
    void draw(float time);


};

*/


sirpale::sirpale()
{
    int x, y;
    xres = 60;
    yres = 35;

    itemcount = xres*yres*2;

    items = new sirpaleitem[itemcount];

    const float xstep = 1.0f / (float)(xres-1);
    const float ystep = 1.0f / (float)(yres-1);

    int offs = 0;
    for (y=0;y<yres;y++)
    {
        const float ykohta = y / (float)(yres-1);

        for (x=0;x<xres;x++)
        {
            const float xkohta = x / (float)(xres-1);

            items[offs].v1.position = Vector(xkohta, ykohta, 0);
            items[offs].v1.u = xkohta;
            items[offs].v1.v = 1 - ykohta;

            items[offs].v2.position = Vector(xkohta + xstep, ykohta + ystep, 0);
            items[offs].v2.u = xkohta + xstep;
            items[offs].v2.v = 1 - (ykohta + ystep);

            items[offs].v3.position = Vector(xkohta, ykohta + ystep, 0);
            items[offs].v3.u = xkohta;
            items[offs].v3.v = 1 - (ykohta + ystep);

            items[offs].d = randomSphereVector(1);

            offs++;

            items[offs].v1.position = Vector(xkohta, ykohta, 0);
            items[offs].v1.u = xkohta;
            items[offs].v1.v = 1 - ykohta;

            items[offs].v2.position = Vector(xkohta + xstep, ykohta , 0);
            items[offs].v2.u = xkohta + xstep;
            items[offs].v2.v = 1 - ykohta;

            items[offs].v3.position = Vector(xkohta + xstep, ykohta + ystep, 0);
            items[offs].v3.u = xkohta + xstep;
            items[offs].v3.v = 1 - (ykohta + ystep);

            items[offs].d = randomSphereVector(5);

            offs++;
        }
    }

    for (int i=0;i<itemcount;i++)
    {
        Vector c = items[i].v1.position + items[i].v1.position + items[i].v1.position;
        c *= 0.33333f;
        items[i].pos = c;
        items[i].v1.position -= c;
        items[i].v2.position -= c;
        items[i].v3.position -= c;

        items[i].rotspeed = randomSphereVector(2);


    }
}

sirpale::~sirpale()
{
    delete [] items;
}

void sirpale::draw(float time)
{
    int i;
    glLoadIdentity();
    perspective2D(1,1);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);


    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture->getID());

    int offs = 0;

    static float dist = 0.0f;
    static float rotation = 0.0f;

    float colorpos = 1 - framePosition;
    glColor3f(colorpos, colorpos, colorpos);

    dist += dt; 
    rotation += dt;
    for (i=0;i<itemcount;i++)
    {
        glPushMatrix();

        Vector d = items[i].pos + items[i].d * dist;
        Vector r = items[i].rotspeed * rotation;
        glTranslatef(d.x, d.y, d.z);
        glRotatef(360*r.x, 1, 0, 0);
        glRotatef(360*r.y, 0, 1, 0);
        glRotatef(360*r.z, 0, 0, 1);

        glBegin(GL_TRIANGLES);
        glTexCoord2f(items[i].v1.u, items[i].v1.v);
        glVertex3fv((float *)&items[i].v1.position);
        glTexCoord2f(items[i].v2.u, items[i].v2.v);
        glVertex3fv((float *)&items[i].v2.position);
        glTexCoord2f(items[i].v3.u, items[i].v3.v);
        glVertex3fv((float *)&items[i].v3.position);
        glEnd();
        glPopMatrix();
    }

/*
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex2f(0, 0);
    glTexCoord2f(1, 1);
    glVertex2f(1, 0);
    glTexCoord2f(1, 0);
    glVertex2f(1, 1);
    glTexCoord2f(0, 0);
    glVertex2f(0, 1);

    glEnd();
*/
    perspective3D();
    glLoadIdentity();

}

void sirpale::setTexture(Texture *text)
{
    texture = text;
}
bool sirpale::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;

    return true;
}

bool sirpale::free()
{
    return true;
}