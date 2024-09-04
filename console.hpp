#ifndef _CONSOLE_HPP_
#define _CONSOLE_HPP_

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

class Console
{
public:
    Console() { standardOutput = false; };
    ~Console() {};
    void write( int x, int y, const char *text, ...);
    void clear();

private:
    HANDLE standardOutput;
};


#endif
