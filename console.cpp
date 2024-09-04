#include "console.hpp"

//#define USE_CONSOLE

void Console::write( int x, int y, const char *text, ...)
{
#ifdef USE_CONSOLE
    va_list ap;
	char buf[1024] = {0};
	
	if(!text) return;
				
	va_start(ap, text);
    vsprintf(buf, text, ap);
	va_end(ap);

	if( !standardOutput )
	{
		AllocConsole();
		standardOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	}

	COORD cursor;
	cursor.X = x;
	cursor.Y = y;
	
	SetConsoleCursorPosition( standardOutput, cursor );
	WriteConsole( standardOutput, buf, strlen( buf ), NULL, NULL );
#endif
}

void Console::clear()
{
#ifdef USE_CONSOLE

	COORD cursor;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

	if( !standardOutput )
	{
		AllocConsole();
		standardOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	}
	
	GetConsoleScreenBufferInfo( standardOutput, &consoleInfo );
	
	cursor.X = 0;
	cursor.Y = 0;

	FillConsoleOutputCharacter( standardOutput, ' ', consoleInfo.srWindow.Right * consoleInfo.srWindow.Bottom, cursor, NULL );
#endif
}
