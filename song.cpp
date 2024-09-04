#include "song.hpp"

Song::Song()
{
    songinfo = 0;
}

Song::Song(char *filename)
{
    songinfo = 0;
}

Song::~Song()
{
    if (songinfo != 0)
        delete [] songinfo;
}

