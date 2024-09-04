#include "mesh.hpp"
#include "matikka.h"

Mesh::Mesh()
{
	vertexcount = 0;
	facecount = 0;
	edgecount = 0;

	vertices = 0;
	faces = 0;
	edges = 0;
}

Mesh::~Mesh()
{
	destroy();
}
void Mesh::destroy()
{
	if (vertices)
		delete [] vertices;
	if (faces)
		delete [] faces;
	if (edges)
		delete [] edges;

	vertexcount = 0;
	edgecount = 0;
	facecount = 0;
}

void Mesh::calculateNormals()
{
	int i;
	//facenormaalit
	for (i=0;i<facecount;i++)
	{
		Vector v1 = vertices[faces[i].v1].position;
		Vector v2 = vertices[faces[i].v2].position;
		Vector v3 = vertices[faces[i].v3].position;

//		Vector normal = (v1-v2).crossProduct(v1-v3);
		Vector normal = (v2-v1).crossProduct(v3-v1);

		faces[i].normal = normal.normalize();
	}
	//verteksinormaalit (samaan looppiin?)

	for (i=0;i<vertexcount;i++)
	{
		vertices[i].normal = Vector(0, 0, 0);
	}
	for (i=0;i<facecount;i++)
	{
		Vector normal = faces[i].normal;
		vertices[faces[i].v1].normal += normal;
		vertices[faces[i].v2].normal += normal;
		vertices[faces[i].v3].normal += normal;
	}
	for (i=0;i<vertexcount;i++)
	{
		vertices[i].normal.normalize();
	}
}

void Mesh::scale(float max)
{
    int i;

    float m = 0.0f;
    for (i=0;i<vertexcount;i++)
    {
        float l = vertices[i].position.length();
        if (l > m)
            m = l;
    }

    if (m > 0.0001f)
    {
        for (i=0;i<vertexcount;i++)
        {
            vertices[i].position *= (1.0f/m)*max;
        }
    }
}

void Mesh::buildEmpty(int xres, int yres)
{
	vertexcount = xres*yres;
	facecount = xres*yres*2;

    vertices = new Vertex[vertexcount];
	faces = new Face[facecount];
}

void Mesh::buildSphere(float radius, int res)
{
	int x, y;
	vertexcount = res*res;
	facecount = res*res*2;
	vertices = new Vertex[vertexcount];
	faces = new Face[facecount];

	for (y=0;y<res;y++)
	{
		for (x=0;x<res;x++)
		{
			const float xkohta = x / (float)res;
			const float ykohta = y / (float)res;

			Vector pos = sphereCoord(radius, ykohta*3.141592f, xkohta*2*3.141592f);
			vertices[x+y*res].position = pos;
			vertices[x+y*res].u = xkohta;
			vertices[x+y*res].v = ykohta;
		}
	}

    makeFaces(res, res);


}

void Mesh::makeFaces(int xres, int yres)
{
    int x, y;
	int index=0;
	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+(y%yres)*xres;
			faces[index].v3 = ((x+1)%xres)+((y+1)%yres)*xres;
			index++;
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+((y+1)%yres)*xres;
			faces[index].v3 = ((x)%xres)+((y+1)%yres)*xres;
			index++;
		}
	}

}
void Mesh::buildPlane(float size, int xres, int yres, Vector &rotation)
{
	int x, y;
	Matrix rotate;

	vertexcount = xres*yres;
	facecount = xres*yres*2;
	vertices = new Vertex[vertexcount];
	faces = new Face[facecount];
	rotate.makeRotation(rotation.x, rotation.y, rotation.z);

	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			const float xkohta = x / (float)xres;
			const float ykohta = y / (float)yres;

			vertices[x+y*xres].position = (Vector(xkohta-0.5f, 0, ykohta-0.5f)*size)*rotate;
		}
	}
	int index=0;
	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+(y%yres)*xres;
			faces[index].v3 = ((x+1)%xres)+((y+1)%yres)*xres;
			index++;
			faces[index].v1 = (x%xres)+(y%yres)*xres;
			faces[index].v2 = ((x+1)%xres)+((y+1)%yres)*xres;
			faces[index].v3 = ((x)%xres)+((y+1)%yres)*xres;
			index++;
		}
	}
}
void Mesh::buildTorus(float radius1, float radius2, int xres, int yres)
{
    int x, y;
	vertexcount = xres*yres;
	facecount = xres*yres*2;
	vertices = new Vertex[vertexcount];
	faces = new Face[facecount];

    //ulompi kehä
    for (y=0;y<yres;y++)
    {
        float ykohta = y / (float)yres;
        float yangle = ykohta*2*3.141592f;

        //torus tulee xz-tasoon
        Matrix rotation;
        rotation.makeRotation(0, yangle, 0);

        Vector centerpoint = Vector(0, 0, radius1);

        //sisempi kehä
        for (x=0;x<xres;x++)
        {
            float xkohta = x / (float)xres;
            float xangle = xkohta*2*3.141592f;

            Vector edgepoint = Vector(0, (float)sin(xangle), (float)cos(xangle))*radius2;
            vertices[x+y*xres].position = (centerpoint + edgepoint) * rotation;
        }
    }
    makeFaces(xres, yres);

}

void Mesh::emptyCopy(Mesh *Mesh)
{
	vertexcount = Mesh->getVertexCount();
	edgecount = Mesh->getEdgeCount();
	facecount = Mesh->getFaceCount();

	vertices = new Vertex[vertexcount];
	edges = new Edge[edgecount];
	faces = new Face[facecount];
}
