#ifndef _kuutio_HPP_
#define _kuutio_HPP_

#include "../global.h"

class kuutio : public Event
{
public:
    kuutio();
    ~kuutio();

    bool init(float start, float end);
    bool free();
    void draw(float time);
    void setTexture(Texture *text, Texture *text2);

private:
    Mesh *objekti;
    int sivucount;
    Vertexbuffer **sivut;
    Vector *kohdat;
    Texture *texture;
    Texture *texture2;
    void generateChess(Vector &v1, Vector &v2, Vector &v3, Vector &v4, Vertexbuffer *buf);

};


#endif