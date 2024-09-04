#include "twister.hpp"

/*

class twister : public Event
{
public:
    twister();
    ~twister();

    bool init(float start, float end);
    bool free();
    void draw(float time);


};

*/


twister::twister()
{
    int i;
    int y;
    spancount = YRES / 2;
    points = new twisterpoint*[spancount];

    spanheight = 1.0f / spancount;

    pointsperline = 40;

    for (y=0;y<spancount;y++)
    {
        const float ypos = y / (float)spancount;
        points[y] = new twisterpoint[pointsperline];

        for (i=0;i<pointsperline;i++)
        {
            const float ipos = i / (float)pointsperline;
            const float istep = 1.0f / pointsperline;
            const float a = ipos * 2 * 3.141592f;
            const float r = 0.3f;

            points[y][i].x = sinf(a)*r;
            points[y][i].z = cosf(a)*r;
            points[y][i].y = ypos;
            points[y][i].u = ipos;
            points[y][i].v = 1-ypos;
        }
    }
}

twister::~twister()
{
    int i;
    for (i=0;i<spancount;i++)
    {
        delete [] points[i];
    }
    delete [] points;
}

void twister::draw(float time)
{
    int y;
    int x;
    int i;
    if (!isActive(time))
        return;

    glLoadIdentity();
    perspective2D(1,1);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1,1,1,1);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture->getID());


    const float phase = calcPosFloat(framePosition, 0.5f, 0.7f);
    Matrix rot;
    rot.makeRotation(0, framePosition*16, 0);

    int spanstodraw = (int)(calcSaturate(framePosition, 0, 1, 7) * spancount);

    for (y=0;y<spanstodraw;y++)
    {
        const float ypos = y / (float)spancount;

        for (i=0;i<pointsperline;i++)
        {
            const float rotation = framePosition * 16;
            const float ipos = i / (float)pointsperline;
            const float istep = 1.0f / pointsperline;
            const float a = ipos * 2 * 3.141592f + rotation;
            const float a2 = (ipos + istep) * 2 * 3.141592f;

            float r = 0.45f + 0.1f*cosf(ypos * 15 + framePosition*31) 
                           + 0.2f*sinf(ypos * 3 * 3.141592f - framePosition*55)
                           + 0.13f*cosf(ypos * 11 + framePosition*4);

            r += fabsf(sinf(ipos * 3.141592f * 5 + ypos*14 + framePosition*33) * 0.3f)*phase;

            points[y][i].x = sinf(a)*r;
            points[y][i].z = cosf(a)*r;
        }
    }
    glBegin(GL_QUADS);


    Matrix lrot;
    lrot.makeRotation(0, sinf(framePosition*40), 0);

    Vector light = Vector(1, 0, 0) * lrot;
    for (y=0;y<spanstodraw;y++)
    {
        const float ypos = y / (float)spancount;

        for (x=0;x<pointsperline;x++)
        {
            twisterpoint *p1 = &points[y][x];
            twisterpoint *p2 = &points[y][(x+1)%pointsperline];

            Vector n = Vector(p2->x - p1->x, 0, p2->z - p1->z);
            float l = light.dotProduct(n) * 10;

            glColor3f(l,l,l);

            const float twisterdist = 4.0f;
            const float screenpos = 0.5f;

            float ypos = p1->y;
            float xpos1 = screenpos + p1->x / (p1->z + twisterdist);
            float xpos2 = screenpos + p2->x / (p2->z + twisterdist);

            if (xpos2 <= xpos1)
            {
                continue;
            }

            glTexCoord2f(p1->u, p1->v);
            glVertex2f(xpos1, ypos);
            glTexCoord2f(p2->u, p1->v);
            glVertex2f(xpos2, ypos);
            glTexCoord2f(p2->u, p2->v);
            glVertex2f(xpos2, ypos + spanheight);
            glTexCoord2f(p1->u, p2->v);
            glVertex2f(xpos1, ypos + spanheight);




        }

    }
    glEnd();

/*
    glDisable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);

    int span = 211;

    glBegin(GL_LINE_STRIP);
    for (i=0;i<pointsperline;i++)
    {
        const float scale = 0.4f;
        twisterpoint *p = &points[span][i];
        glVertex2f(p->x*scale + 0.5f, p->z*scale + 0.5f);
    }
    glEnd();

*/


    perspective3D();
    glLoadIdentity();

}


void twister::setTexture(Texture *text)
{
    texture = text;
}
bool twister::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;

    return true;
}

bool twister::free()
{
    return true;
}