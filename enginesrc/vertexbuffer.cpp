#include "Vertexbuffer.hpp"

Vertexbuffer::Vertexbuffer()
{
    containsVertices = false;
    containsUV = false;
    containsNormals = false;
    containsColors = false;
}

Vertexbuffer::~Vertexbuffer()
{
    if (containsVertices)
        delete [] vertices;
    if (containsUV)
        delete [] uv;
    if (containsNormals)
        delete [] normals;
    if (containsColors)
        delete [] colors;
}

void Vertexbuffer::startVertices()
{
    this->vertexlist = new Lista();
    this->vertexcount = 0;
    containsVertices = true;
}
void Vertexbuffer::startNormals()
{
    this->normallist = new Lista();
    containsNormals = true;
}
void Vertexbuffer::startColors()
{
    this->colorlist = new Lista();
    containsColors = true;
}
void Vertexbuffer::startUV()
{
    this->ulist = new Lista();
    this->vlist = new Lista();
    containsUV = true;
}

void Vertexbuffer::addUV(float u, float v)
{
    FloatContainer *ucont = new FloatContainer();
    FloatContainer *vcont = new FloatContainer();

    ucont->value = u;
    vcont->value = v;

    ulist->add(ucont);
    vlist->add(vcont);
}

void Vertexbuffer::addVertex(Vector &vertex)
{
    Vector *vert = new Vector(vertex.x, vertex.y, vertex.z);
    this->vertexlist->add(vert);
}
void Vertexbuffer::addNormal(Vector &normal)
{
    Vector *n = new Vector(normal.x, normal.y, normal.z);
    this->normallist->add(n);
}
void Vertexbuffer::addColor(Vector &color)
{
    Vector *c = new Vector(color.x, color.y, color.z);
    this->colorlist->add(c);
}

void Vertexbuffer::finalize()
{
    if (containsVertices)
        finalizeVertices();

    if (containsUV)
        finalizeUV();

    if (containsNormals)
        finalizeNormals();

    if (containsColors)
        finalizeColors();

}

void Vertexbuffer::finalizeVertices()
{
    int i;
    this->vertexlist->goToStart();
    int count = this->vertexlist->getNodeCount();

    this->vertexcount = count;

    vertices = new Vector[count];
    for (i=0;i<count;i++)
    {
        vertices[i] = *(Vector *) this->vertexlist->getCurrent();
        this->vertexlist->goForward();
    }

    delete this->vertexlist;
}

void Vertexbuffer::finalizeUV()
{
    int i;

    this->ulist->goToStart();
    this->vlist->goToStart();

    int count = this->ulist->getNodeCount();
    this->uv = new float[count*2]; //sekä u että v

    int offs = 0;
    for (i=0;i<count;i++)
    {
        FloatContainer *c1 = (FloatContainer *)ulist->getCurrent();
        FloatContainer *c2 = (FloatContainer *)vlist->getCurrent();
        this->uv[offs++] = c1->value;
        this->uv[offs++] = c2->value;

        ulist->goForward();
        vlist->goForward();
    }
    delete this->ulist;
    delete this->vlist;
}

void Vertexbuffer::finalizeNormals()
{
    int i;
    this->normallist->goToStart();
    int count = this->normallist->getNodeCount();

    normals = new Vector[count];
    for (i=0;i<count;i++)
    {
        normals[i] = *(Vector *) this->normallist->getCurrent();
        this->normallist->goForward();
    }

    delete this->normallist;
}

void Vertexbuffer::finalizeColors()
{
    int i;
    this->colorlist->goToStart();
    int count = this->colorlist->getNodeCount();

    colors = new Vector[count];
    for (i=0;i<count;i++)
    {
        colors[i] = *(Vector *) this->colorlist->getCurrent();
        this->colorlist->goForward();
    }

    delete this->colorlist;
}
