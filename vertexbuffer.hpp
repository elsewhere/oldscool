#ifndef _VertexBUFFER_HPP_
#define _VertexBUFFER_HPP_

#include "global.h"

class FloatContainer
{
public:
    float value;
};

class Vertexbuffer
{
public:

    Vertexbuffer();
    ~Vertexbuffer();

    void startVertices();
    void startUV();
    void startNormals();
    void startColors();

    void addUV(float u, float v);
    void addVertex(Vector &vertex);
    void addNormal(Vector &normal);
    void addColor(Vector &colors);

    void finalize();

    const int getVertexCount() { return this->vertexcount; };
    const int getQuadCount() { return this->vertexcount / 4; };
    const int getTriangleCount() { return this->vertexcount / 3; };
    Vector *getVertexBuffer() { return this->vertices; };
    Vector *getNormalBuffer() { return this->normals; };
    Vector *getColorBuffer() { return this->colors; };
    float *getUVBuffer() { return this->uv; };

    Lista *getVertexList() { return this->vertexlist; };
    Lista *getUList() { return this->ulist; };
    Lista *getVList() { return this->vlist; };
    Lista *getNormalList() { return this->normallist; };
    Lista *getColorList() { return this->colorlist; };

private:
    void finalizeUV();
    void finalizeVertices();
    void finalizeNormals();
    void finalizeColors();

    Lista *vertexlist;
    Lista *normallist;
    Lista *colorlist;
    Lista *ulist;
    Lista *vlist;

    Vector *vertices;
    Vector *normals;
    Vector *colors;
    float *uv;
    int vertexcount;

    bool containsVertices;
    bool containsUV;
    bool containsNormals;
    bool containsColors;


};

#endif