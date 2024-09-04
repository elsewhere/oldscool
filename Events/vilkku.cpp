#include "vilkku.hpp"

/*

class vilkku : public Event
{
public:
    vilkku();
    ~vilkku();

    bool init(float start, float end);
    bool free();
    void draw(float time);


};

*/


vilkku::vilkku()
{
    int x, y;
    xres = 2;
    yres = 50;

    items = new vilkkuitem[xres*yres];

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

vilkku::~vilkku()
{
    delete [] items;
}

void vilkku::draw(float time)
{
    int x, y;
    glLoadIdentity();
    perspective2D(1,1);

    const float fadein = calcPosFloat(framePosition, 0.3f, 0.7f);
    static float valokohta = 0.0f;
    float valovoima = 0.0f;

    float beatval = beat4->getValue();
    if (beat4->getChanged())
    {
        valokohta = randFloat();
    }
    valovoima = beatval * (1-fadein);//powf(1-sinf(beatval*3.141592f), 2);

    const float maxdist = 0.3f * valovoima;

    for (y=0;y<yres;y++)
    {
        float ypos = y / (float)yres;
        for (x=0;x<xres;x++)
        {
            float add = 0;
            float dist = fabsf(ypos - valokohta);
            if (dist > maxdist)
                add = 0;
            else
            {
                add = (maxdist - dist) / maxdist;

            }

            items[x+y*xres].alpha = add*valovoima + fadein*sinf(fmodf((framePosition * items[x+y*xres].speed + items[x+y*xres].phase), 1)*3.141592f);
        }

    }


    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glColor3f(1,1,1);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture->getID());

    for (y=0;y<yres-1;y++)
    {
        glBegin(GL_QUAD_STRIP);
        for (x=0;x<xres;x++)
        {
            glColor3f(items[x+y*xres].alpha, items[x+y*xres].alpha, items[x+y*xres].alpha);
            glTexCoord2f(items[x+y*xres].uv1[0], items[x+y*xres].uv1[1]);
            glVertex2fv((float *)&items[x+y*xres].pos);
            glColor3f(items[x+(y+1)*xres].alpha, items[x+(y+1)*xres].alpha, items[x+(y+1)*xres].alpha);
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

}

void vilkku::setTexture(Texture *text)
{
    texture = text;
}
bool vilkku::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;

    return true;
}

bool vilkku::free()
{
    return true;
}