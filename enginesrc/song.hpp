#ifndef _SONG_HPP_
#define _SONG_HPP_

class Song
{
public:
    Song();
    Song(char *filename);
    ~Song();

    void play();

private:
    char *songinfo;


};


#endif