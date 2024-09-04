#include "picpulse.hpp"

/*

class picpulse : public Event
{
public:
    picpulse();
    ~picpulse();

    bool init(float start, float end);
    bool free();
    void draw(float time);


};

*/


picpulse::picpulse()
{
}

picpulse::~picpulse()
{
}

void picpulse::setMaxPulse(float max)
{
    maxpulse = max;

}
void picpulse::draw(float time)
{
    if (!isActive(time))
        return;

    glLoadIdentity();
    perspective2D(1,1);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    float beatval = (1-maxpulse) + maxpulse*beat->getValue();
    glColor3f(beatval, beatval, beatval);

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

void picpulse::setTexture(Texture *text)
{
    texture = text;
}
bool picpulse::init(float start, float end)
{
    this->starttime = start;
    this->endtime = end;

    return true;
}

bool picpulse::free()
{
    return true;
}