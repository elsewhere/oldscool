#include "pink.hpp"


pink::pink()
{
    int i;
    streamcount = 7;

    itemcount = 200;

    streams = new pinkitem*[streamcount];
    for (int j=0;j<streamcount; j++)
    {
        float timestep = 1.0f / itemcount;
        streams[j] = new pinkitem[itemcount];

        Vector position = Vector(0, 0, 0);
        Vector rotation = Vector(0, 0, 0);

        for (i=0;i<itemcount;i++)
        {
            float t = i / (float)itemcount;
            float phase = j * 0.4f;
            float value = j * 1.0f;

            streams[j][i].starttime = i * timestep;
            streams[j][i].endtime = (i + 1) * timestep;

            position = Vector(100.0f*t + 5 * (0.5f+0.5f*sinf(t*18 + j * 0.7f)), 
                              4*sinf(t*16+phase) + 2*cosf(t*11+phase) - j + j*0.2f*cosf(t*4*3.141592f + phase*2), 
                              3*sinf(t*7+phase) + 5*cosf(t*17+phase) + j + j*0.2f*sinf(t*6*3.141592f + phase));
            rotation = Vector(t*17, t*11, sinf(t*14)*21);
            streams[j][i].position = position;
            streams[j][i].rotation = rotation*(1+j*0.2f);
            streams[j][i].size = 0.2f + 0.15f*sinf(t*15);

        }
    }

}

pink::~pink()
{
    int i;
    for (i=0;i<streamcount;i++)
    {
        delete [] streams[i];

    }
    delete [] streams;
}
void pink::setTexture(Texture *text)
{
    texture = text;
}

void pink::draw(float time)
{
    int i;

    int currentitem = (int)(framePosition * itemcount);
    float frac = fmodf(framePosition * itemcount, 1);

    if (currentitem < 1)
        currentitem = 1;
    if (currentitem >= itemcount)
        currentitem = itemcount-1;

    const float camradius = 19.0f;
    const float camspeed = 0.5f;
    Vector cammove = Vector(0.6f * sinf(framePosition * 11 * camspeed) + 0.4f * cosf(framePosition * 18 * camspeed), 
                            0.4f * cosf(framePosition * 7 * camspeed) + 0.9f * cosf(framePosition * 24 * camspeed),
                            0.7f * sinf(framePosition * 15 * camspeed) + 0.32f * cosf(framePosition * 11 * camspeed)) * camradius;

    Vector position = streams[0][currentitem-1].position*(1-frac) + streams[0][currentitem].position*frac;
    Vector campos = position + cammove;

    Vector camtarget = position;
    float alpha = calcSaturate(framePosition, 0.0f, 1.0f, 3.0f);

    glLoadIdentity();
    gluLookAt(campos.x, campos.y, campos.z, 
                    camtarget.x, camtarget.y, camtarget.z, 
                    0, 1, 0);


    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1,1,1,alpha);
    glDepthMask(0);
    Primitives::texturedCube(1000, 1);
    glBindTexture(GL_TEXTURE_2D, texture->getID());

    glDepthMask(1);


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.94f, 0.098f, 0.44f, alpha*0.7f);


	const float ambientvalo[] = {0.1f,0.1f,0.1f,1};
	const float specularvalo[] = {1,1,1, 1};
	const float diffusevalo[] = {1,1,1, 0.2f};
	const float valoposition[] = {1110, 0, 0, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientvalo);
  	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffusevalo);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularvalo);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

    //jos streamcount != 7, r‰j‰ht‰‰
    static const unsigned char colors[7][3] = 
    {
        {0x66, 0x01, 0xFF},
        {0xF2, 0x19, 0x70},
        {0x00, 0xCC, 0xFF},
        {0xFF, 0x61, 0x00},
        {0xFF, 0x06, 0x00},
        {0x00, 0xDC, 0x9F},
        {0xFF, 0xAD, 0x00},

     };



    for (int j=0;j<streamcount;j++)
    {
        Vector c = Vector(colors[j][0] / 255.0f, 
                            colors[j][1] / 255.0f, 
                            colors[j][2] / 255.0f);
	    const float ambientmat[] = {0, 0, 0, 0};
	    const float diffusemat[] = {c.x, c.y, c.z, 0.25f*alpha};
	    const float specularmat[] = {c.x, c.y, c.z, 1.55f*alpha};
	    const float emissionmat[] = {0, 0, 0, 1};
	    float shininessmat = 1.0f;

	    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientmat);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffusemat);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularmat);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, specularmat);
	    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininessmat);

        for (i=0;i<itemcount;i++)
        {
            //do not draw unnecessary cubes
            if (streams[j][i].starttime > framePosition)
                break;

            glPushMatrix();
            Vector p = streams[j][i].position;
            Vector r = streams[j][i].rotation;

            glTranslatef(p.x, p.y, p.z);
            glRotatef(360*r.x, 1, 0, 0);
            glRotatef(360*r.y, 0, 1, 0);
            glRotatef(360*r.z, 0, 0, 1);

            const float size = streams[j][i].size * calcPosFloat(framePosition, streams[j][i].starttime, streams[j][i].endtime);


            Primitives::flatCube(size);

            glPopMatrix();

        }
    }
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glDisable(GL_NORMALIZE);

}

bool pink::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool pink::free()
{
    return true;
}