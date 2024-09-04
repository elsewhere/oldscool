#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "vector.hpp"

class Face
{
public:
	int v1, v2, v3; //indeksit verteksitaulukkoon
	Vector normal;

};

class Vertex
{
public:

	Vector position;
    Vector position2;
	Vector normal;
	Vector color;
	float u, v;
};

class Edge
{
public:
	int v1;
	int v2;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

	Vertex *getVertices() { return vertices; }
	Face *getFaces() { return faces; }
	Edge *getEdges() { return edges; }

	int getVertexCount() { return vertexcount; }
	int getFaceCount() { return facecount; }
	int getEdgeCount() { return edgecount; }
        
    void calculateNormals();
    void makeFaces(int xres, int yres);
    void scale(float max);

    void buildSphere(float radius, int res);
	void buildPlane(float size, int xres, int yres, Vector &rotation);
    void buildTorus(float radius1, float radius2, int xres, int yres);
    void buildEmpty(int xres, int yres);
	void emptyCopy(Mesh *object);

	void destroy();


private:
    int vertexcount;
    int facecount;
    int edgecount;

    Vertex *vertices;
    Face *faces;
	Edge *edges;



};


#endif
