#include "picunwrap.hpp"

/*

class picunwrap : public Event
{
public:
    picunwrap();
    ~picunwrap();

    bool init(float start, float end);
    bool free();
    void draw(float time);


};

*/


picunwrap::picunwrap()
{
    int x, y;
    xres = 2;
    yres = 100;

    items = new picunwrapitem[xres*yres];

    const float xstep = 1.0f / (float)(xres-1);
    const float ystep = 1.0f / (float)(yres-1);

    for (y=0;y<yres;y++)
    {
        const float ykohta = y / (float)(yres-1);

        float alphaspeed = 7 + 5*randFloat();
        float alphaphase = randFloat();
        for (x=0;x<xres;x++)
        {
            const float xkohta = x / (float)(xres-1);
            items[x+y*xres].pos = Vector(xkohta, ykohta, 0);
            items[x+y*xres].uv1[0] = xkohta;
            items[x+y*xres].uv1[1] = 1-ykohta;
            items[x+y*xres].uv2[0] = xkohta + xstep;
            items[x+y*xres].uv2[1] = 1-(ykohta + ystep);
            items[x+y*xres].phase = alphaphase;//randFloat();
            items[x+y*xres].speed = alphaspeed;//randFloat();
        }
    }
}

picunwrap::~picunwrap()
{
    delete [] items;
}

void picunwrap::draw(float time)
{
    int x, y;
    glLoadIdentity();
    perspective2D(1,1);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1,1,1);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture->getID());

    int reached = (int)((framePosition) * yres);
    float frac = fmodf((framePosition) * yres, 1);


    for (y=0; y < yres-1; y++)
    {
        if (y < reached)
            continue;

        if (y == reached)
        {
            glColor4f(1,1,1,frac);
        }
        else
        {
            glColor4f(1,1,1,1);
        }
        const float ypos = y / (float)(yres-1);
        glBegin(GL_QUAD_STRIP);
        for (x=0;x<xres;x++)
        {
            glTexCoord2f(items[x+y*xres].uv1[0], items[x+y*xres].uv1[1]);
            glVertex2fv((float *)&items[x+y*xres].pos);
            glTexCoord2f(items[x+(y+1)*xres].uv1[0], items[x+(y+1)*xres].uv1[1]);
            glVertex2fv((float *)&items[x+(y+1)*xres].pos);

        }
        glEnd();
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

    glDisable(GL_BLEND);

}

void picunwrap::setTexture(Texture *text)
{
    texture = text;
}
bool picunwrap::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;

    return true;
}

bool picunwrap::free()
{
    return true;
}