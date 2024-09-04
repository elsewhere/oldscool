#include "spiraali.hpp"


spiraali::spiraali()
{
    xres = 80;
    yres = 50;

    table = new float[xres*yres*4];
}

spiraali::~spiraali()
{
    delete [] table;

}

void spiraali::draw(float time)
{
    int x, y;

    int offs = 0;

    Vector col1 = Vector(0x22 / 255.0f, 0x33/ 255.0f, 0x99/ 255.0f);
    Vector col2 = Vector(0xFF / 255.0f, 0x22/ 255.0f, 0x11/ 255.0f);


    int limit = (int)(xres*yres*4*(1-calcPosFloat(framePosition, 0.97f, 1.00f)));
    for (y = 0; y < yres; y++)
    {
        for (x=0; x < xres;x++)
        {
            Vector cp = Vector((x - xres*0.5f)/xres, (y - yres*0.5f)/yres, 0);
            float d = cp.length();

            float a = 0.5f 
                + 0.5f*sinf((10+6*cosf(framePosition*24))*cosf(d*6 + framePosition*11) * sinf(framePosition * 19) + (atan2f(cp.x, cp.y))*5 + framePosition * 17);
/*

            if (a > 0.5f)
                a = 1;
            else
                a = 0;
*/
            Vector col = col1 * (1-a) + col2*a;
            table[offs++] = col.x;
            table[offs++] = col.y;
            table[offs++] = col.z;
            offs++;
        }
    }
    glLoadIdentity();
    glTranslatef(0, 0, -3.7f + sinf(framePosition*3.141592f));
    glRotatef(4 + 30 * framePosition, 0.8f, 0.1f, 0.2f);


    const float xstep = 1.0f / xres;
    const float ystep = 1.0f / yres;

    const float globalsize = 3.0f;
    const float quadsize = 0.035f;
    Vector c2 = Vector(quadsize, 0, 0);
    Vector c3 = Vector(quadsize, quadsize, 0);
    Vector c4 = Vector(0, quadsize, 0);
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
    offs = 0;
    glBegin(GL_QUADS);
    for (y=0;y<yres;y++)
    {
        for (x=0;x<xres;x++)
        {
            Vector cp = Vector((x - xres*0.5f)/xres, (y - yres*0.5f)/yres, 0) * globalsize;

            glColor3fv((float *)&table[offs]);

            glVertex3fv((float *)&cp);
            glVertex3fv((float *)&(cp+c2));
            glVertex3fv((float *)&(cp+c3));
            glVertex3fv((float *)&(cp+c4));

            offs += 4;
            if (offs > limit)
                goto done;
        }

    }
done:
    glEnd();





    
}

bool spiraali::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool spiraali::free()
{
    return true;
}