#include "wobble.hpp"

/*

class wobble : public Event
{
public:
    wobble();
    ~wobble();

    bool init(float start, float end);
    bool free();
    void draw(float time);


};

*/


wobble::wobble()
{
    int x, y;
    xres = 80;
    yres = 50;

    items = new wobbleitem[xres*yres];

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
            items[x+y*xres].pos = Vector(xkohta - 0.5f, (ykohta - 0.5f) * 600.0f/800.0f, 0);
            items[x+y*xres].u = xkohta;
            items[x+y*xres].u = 1-ykohta;
            items[x+y*xres].phase = alphaphase;//randFloat();
            items[x+y*xres].speed = alphaspeed;//randFloat();
        }
    }
}

wobble::~wobble()
{
    delete [] items;
}

void wobble::draw(float time)
{
    int x, y;
    glLoadIdentity();
    glTranslatef(0, 0, -0.8f);//- framePosition*0.5f - 0.1f*sinf(framePosition * 13));

//    glRotatef(framePosition * 700, 0.1f, 0.9f, 1.0f);
//    glRotatef(framePosition * 70, 0.1f * sinf(framePosition * 7), 0.1f*cosf(framePosition * 7), 0.11f*sinf(framePosition * 5));

    const float xstep = 1.0f / (float)(xres-1);
    const float ystep = 1.0f / (float)(yres-1);


    for (y=0;y<yres;y++)
    {
        const float ykohta = y / (float)(yres-1);

        float ymul = 0.0f;
        if (ykohta < 0.5f)
            ymul = ykohta * 2;
        else
            ymul = (1.0f - ykohta)*2;

        for (x=0;x<xres;x++)
        {
            const float xkohta = x / (float)(xres-1);
            items[x+y*xres].alpha = 1.0f;

            float xmul = 0.0f;
            if (xkohta < 0.5f)
                xmul = xkohta * 2;
            else
                xmul = (1.0f - xkohta)*2;

            Matrix rot;
            rot.makeRotation(-framePosition * 2, framePosition*5, framePosition * 5);
            const float dist = 1.1f + 0.7f*sinf(xkohta * 17 + framePosition * 21) + 0.3f*cosf(ykohta *13 + framePosition * 15);
            Vector dir = Vector(xkohta-0.5f, ykohta-0.5f, 0)*dist * rot;

            items[x+y*xres].u = (xkohta) + dir.x*xmul*ymul;
            items[x+y*xres].v = ((1-ykohta)) + dir.y*ymul*xmul;
        }
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float alpha = calcSaturate(framePosition, 0, 1, 3.0f)*(0.65f+0.1f*sinf(framePosition*14));

    glColor4f(1,1,1, alpha);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture->getID());

    for (y=0;y<yres-1;y++)
    {
        glBegin(GL_QUAD_STRIP);
        for (x=0;x<xres;x++)
        {
            const float lightness = 30.0f;
            const float ambient = 0.5f;
            float du1 = items[((x+1)%xres)+y*xres].u - items[x+y*xres].u;
            float dv1 = items[((x+1)%xres)+y*xres].v - items[x+y*xres].v;
            const float l1 = ambient + (du1 + dv1)*lightness;
            glColor4f(l1, l1, l1, alpha);
//            glColor3f(items[x+y*xres].alpha, items[x+y*xres].alpha, items[x+y*xres].alpha);
            glTexCoord2f(items[x+y*xres].u, items[x+y*xres].v);
            glVertex3fv((float *)&items[x+y*xres].pos);

            float du2 = items[((x+1)%xres)+(y+1)*xres].u - items[x+(y+1)*xres].u;
            float dv2 = items[((x+1)%xres)+(y+1)*xres].v - items[x+(y+1)*xres].v;
            const float l2 = ambient + (du2 + dv2)*lightness;
            glColor4f(l2, l2, l2, alpha);
//            glColor3f(items[x+(y+1)*xres].alpha, items[x+(y+1)*xres].alpha, items[x+(y+1)*xres].alpha);
            glTexCoord2f(items[x+(y+1)*xres].u, items[x+(y+1)*xres].v);
            glVertex3fv((float *)&items[x+(y+1)*xres].pos);

        }
        glEnd();
    }

}

void wobble::setTexture(Texture *text)
{
    texture = text;
}
bool wobble::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;

    return true;
}

bool wobble::free()
{
    return true;
}