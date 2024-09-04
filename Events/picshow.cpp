#include "picshow.hpp"

/*

class picshow : public Event
{
public:
    picshow();
    ~picshow();

    bool init(float start, float end);
    bool free();
    void draw(float time);


};

*/


picshow::picshow()
{
}

picshow::~picshow()
{
}

void picshow::draw(float time)
{
    if (!isActive(time))
        return;

    glLoadIdentity();
    perspective2D(1,1);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glColor3f(1,1,1);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture->getID());
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

    perspective3D();
    glLoadIdentity();

}

void picshow::setTexture(Texture *text)
{
    texture = text;
}
bool picshow::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;

    return true;
}

bool picshow::free()
{
    return true;
}