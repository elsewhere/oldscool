#include "kuutio.hpp"


kuutio::kuutio()
{
    int i;
    sivucount = 6;

    float size = 100;

    sivut = new Vertexbuffer*[sivucount];
    for (i=0;i<sivucount;i++)
    {
        sivut[i] = new Vertexbuffer();
        sivut[i]->startVertices();
        sivut[i]->startUV();
    }

    kohdat = new Vector[sivucount];

    const float dist = 1.3f * size;
    kohdat[0] = Vector(dist, 0, 0);
    kohdat[1] = Vector(-dist, 0, 0);
    kohdat[2] = Vector(0, dist, 0);
    kohdat[3] = Vector(0, -dist, 0);
    kohdat[4] = Vector(0, 0, dist);
    kohdat[5] = Vector(0, 0, -dist);

	Vector upleftback = Vector( -size, -size, -size);
	Vector upleftfront = Vector(-size, -size, +size);
	Vector uprightback = Vector( size, -size, -size);
	Vector uprightfront = Vector(size, -size, +size);

	Vector downleftback = Vector( -size, size, -size);
	Vector downleftfront = Vector(-size, size, +size);
	Vector downrightback = Vector( size, size, -size);
	Vector downrightfront = Vector(size, size, +size);

    generateChess(upleftfront, upleftback, uprightback, uprightfront, sivut[0]);
    generateChess(downleftfront, downleftback, downrightback, downrightfront, sivut[1]);
    generateChess(upleftfront, upleftback, downleftback, downleftfront, sivut[2]);
    generateChess(uprightfront, uprightback, downrightback, downrightfront, sivut[3]);
    generateChess(upleftfront, uprightfront, downrightfront, downleftfront, sivut[4]);
    generateChess(upleftback, uprightback, downrightback, downleftback, sivut[5]);

    for (i=0;i<sivucount;i++)
    {
        sivut[i]->finalize();
    }

    objekti = new Mesh();
    objekti->buildSphere(30, 32);//buildTorus(15, 6, 16, 16);//buildSphere(30, 32);

    objekti->calculateNormals();

}

void kuutio::setTexture(Texture *text, Texture *text2)
{
    texture = text;
    texture2 = text2;
}
kuutio::~kuutio()
{
    int i;
    for (i=0;i<sivucount;i++)
    {
        delete sivut[i];
    }
    delete [] sivut;
    delete [] kohdat;
    delete objekti;
}

void kuutio::generateChess(Vector &v1, Vector &v2, Vector &v3, Vector &v4, Vertexbuffer *buf)
{
	//v1----v2
	//|     |
	//|     |
	//|     |
	//v4----v3

    int x, y;
    
    const int slices = 20;
    Vector *temp = new Vector[slices*slices];

	Vector dy1 = (v4-v1)*(1.0f/(slices-1));
	Vector dy2 = (v3-v2)*(1.0f/(slices-1));

	Vector u1 = v1;
	Vector u2 = v2;

	for (y=0;y<slices;y++)
	{
		Vector dx = (u2-u1)*(1.0f/(slices-1));
		Vector v = u1;
		for (x=0;x<slices;x++)
		{
			temp[x+y*slices] = v;
			v += dx;
		}
		u1 += dy1;
		u2 += dy2;
	}

    //gridi tehty

    const float tmul = 1.0f;

    for (y=0;y<slices-1;y++)
    {
        for (x=0;x<slices-1;x++)
        {
            Vector p1 = temp[x+y*slices];
            Vector p2 = temp[(x+1)+y*slices];
            Vector p3 = temp[(x+1)+(y+1)*slices];
            Vector p4 = temp[x+(y+1)*slices];

            float tu1 = x / (float)(slices-1);
            float tv1 = y / (float)(slices-1);
            float tu2 = (x+1) / (float)(slices-1);
            float tv2 = y / (float)(slices-1);
            float tu3 = (x+1) / (float)(slices-1);
            float tv3 = (y+1) / (float)(slices-1);
            float tu4 = x / (float)(slices-1);
            float tv4 = (y+1) / (float)(slices-1);

            buf->addVertex(p1);
            buf->addVertex(p2);
            buf->addVertex(p3);
            buf->addVertex(p4);
            buf->addUV(tu1, tv1);
            buf->addUV(tu2, tv2);
            buf->addUV(tu3, tv3);
            buf->addUV(tu4, tv4);
        }
    }
 
    delete [] temp;

}

void kuutio::draw(float time)
{
    int i;
    int j;

    float a = (float)cos(framePosition * 6) + (float)sin(framePosition*1) - (float)cos(framePosition*4);
    float b = framePosition;//(float)sin(framePosition * 2) + (float)cos(framePosition*8)*(float)cos(framePosition*3);

    static float r = -300 - 150*(float)cosf(time) - framePosition * 100;

    Vector cam = sphereCoord(-300 + 100*sinf(framePosition * 21), a*3.141592f+framePosition*0.7f, b*3.141592f*2);


    glLoadIdentity();
//    glTranslatef(0, 0, -300 - 50*(float)cos(framePosition*17) - framePosition * 100);
//    glRotatef(860 * framePosition, 0.9f,0.6f*(float)cos(framePosition*11),0.4f+framePosition*0.5f);

    gluLookAt(cam.x, cam.y, cam.z, 0, 0, 0, 0, 1, 0);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    for (i=0;i<sivucount;i++)
    {
        int count = sivut[i]->getQuadCount();
        int count2 = sivut[i]->getQuadCount();
        Vector *vert = sivut[i]->getVertexBuffer();
        float *uv = sivut[i]->getUVBuffer();
        int vertoffs = 0;
        //mustat
        glColor4f(0,0,0,1);
        glBegin(GL_QUADS);
        for (j=0;j<count2;j+=2)
        {
            vertoffs = j*4;
            glVertex3fv((float *)&vert[vertoffs++]);
            glVertex3fv((float *)&vert[vertoffs++]);
            glVertex3fv((float *)&vert[vertoffs++]);
            glVertex3fv((float *)&vert[vertoffs++]);
        }
        glEnd();

        //valkoiset
        glColor4f(1,1,1,1);
        glBegin(GL_QUADS);
        for (j=1;j<count2;j+=2)
        {
            vertoffs = j*4;
            glVertex3fv((float *)&vert[vertoffs++]);
            glVertex3fv((float *)&vert[vertoffs++]);
            glVertex3fv((float *)&vert[vertoffs++]);
            glVertex3fv((float *)&vert[vertoffs++]);
        }
        glEnd();
    }

    glColor4f(1,1,1,1);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glLineWidth(2.0f);

    for (i=0;i<sivucount;i++)
    {
        glPushMatrix();

        const float multiply = 1 + 0.3f*(float)sin(beat2->getValue()*3.141592f);
        glTranslatef(kohdat[i].x * multiply, kohdat[i].y * multiply, kohdat[i].z * multiply);
        glRotatef(framePosition*2711, 0.4f, 0.2f, 0.7f);
/*
        glColor4f(1,0,0,1);
        glRotatef(framePosition*711, 0.4f, 0.2f, 0.7f);
        Primitives::wireCube(15.0f);
*/
        int vcount = objekti->getVertexCount();
        Vertex *vbuf = objekti->getVertices();

        int fcount = objekti->getFaceCount();
        Face *faces = objekti->getFaces();

        if (framePosition < 0.52f)
            glBindTexture(GL_TEXTURE_2D, texture->getID());
        else
            glBindTexture(GL_TEXTURE_2D, texture2->getID());


        glEnable(GL_TEXTURE_2D);
	    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	    glEnable(GL_TEXTURE_GEN_S);
	    glEnable(GL_TEXTURE_GEN_T);

        glEnable(GL_DEPTH_TEST);
        glBegin(GL_TRIANGLES);
        for (j=0;j<fcount;j++)
        {
            Vertex *v1 = &vbuf[faces[j].v1];
            Vertex *v2 = &vbuf[faces[j].v2];
            Vertex *v3 = &vbuf[faces[j].v3];

            glNormal3fv((float *)&faces[j].normal);
    //        glNormal3fv((float *)&v1->normal);
            glVertex3fv((float *)&v1->position);
    //        glNormal3fv((float *)&v2->normal);
            glVertex3fv((float *)&v2->position);
    //        glNormal3fv((float *)&v3->normal);
            glVertex3fv((float *)&v3->position);
        }
        glEnd();
	    glDisable(GL_TEXTURE_GEN_S);
	    glDisable(GL_TEXTURE_GEN_T);

        glPopMatrix();

    }
    glDisable(GL_BLEND);
}


bool kuutio::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;
    return true;
}

bool kuutio::free()
{
    return true;
}