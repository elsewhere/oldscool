#include <stdio.h>
#include <windows.h>
#include "timer.hpp"

extern int originalaika;

void Timer::init()
{
	LONGLONG freq;
	LONGLONG ticks;	

	//	High Resolution Timerin taajuutta ei voi muuttaa niin kauan
	//	kuin systeemi on päällä. Taajuus haetaan vain kerran.
		
	HRT = true;
	startTime = 0;
	addTime = 0.0f;
	frames = 0;
	lastTime = 0;	
	fps = 0;
	time = 0;	
			
	if( !QueryPerformanceFrequency( ( LARGE_INTEGER * )&freq ) )
	{				
		// Jos epäonnistui käytetään MultimediaTimeria		
		HRT = false;		
		startTime = (float)timeGetTime();
	}
	else
	{		
		QueryPerformanceCounter( ( LARGE_INTEGER * )&ticks );
		startTime = ( float ) ticks;
		ticksPerSec = 1.0f / ( float )freq;
	}		

}

void Timer::update()
{
	if( HRT )
	{
		LONGLONG ticks;

		// onnistuu
		QueryPerformanceCounter( ( LARGE_INTEGER * )&ticks );
		
		// jaetaan taajuudella jotta saadaan sekunnit
		//time = ((float)(__int64)ticks)/((float)(__int64)ticksPerSec);
		//time = ( ( float )ticks ) / ( ( float )ticksPerSec );
		time = ( ( float ) ticks - startTime ) * ticksPerSec;
	}
	else
	{
		// Jos ei ole HRT-ajastinta, käytetään Multimedia timeria
		// vaatii WINMM.LIB:n				
		time = ( float )timeGetTime() - startTime;		
	}	
	
	deltaTime = ( time - lastTime );
	lastTime = time;
	
	frames++;		
	
	fps = ( float ) frames / time;
}

void Timer::add(float ptime)
{
	addTime += ptime;
}

float Timer::getSeconds()
{
	return time;
}
float Timer::getMilliseconds()
{
	return time * 1000.0f;
}

void Timer::reset()
{
    this->init();
}
