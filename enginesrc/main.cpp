#define STRICT
#define WIN32_LEAN_AND_MEAN

#pragma warning(disable: 4786)

#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

//STL
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "bass.h"

//engine
#include "lista.hpp"
#include "console.hpp"
#include "timer.hpp"
#include "event.hpp"
#include "image.hpp"
#include "texture.hpp"

//eventit
#include "events/picshow.hpp"
#include "events/picpulse.hpp"
#include "events/picfadein.hpp"
#include "events/picfadeout.hpp"
#include "events/pallo.hpp"
#include "events/munkki.hpp"
#include "events/munkki2.hpp"
#include "events/munkki3.hpp"
#include "events/tunneli.hpp"
#include "events/tunneli2.hpp"
#include "events/kuutio.hpp"
#include "events/vilkku.hpp"
#include "events/levy.hpp"
#include "events/twister.hpp"
#include "events/wobble.hpp"

using namespace std;

//------------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HDC	      g_hDC       = NULL;
HGLRC     g_hRC       = NULL;
HWND      g_hWnd      = NULL;
HINSTANCE g_hInstance = NULL;

//default res
int XRES = 800;
int YRES = 600;

string songname;

Console *console;
Timer *timer;
Lista *texturelist;
Lista *eventlist;
BPM *beat;
BPM *beat2;
BPM *beat4;

int endtime = 0xFFFFFFF;
int starttime = 0;

bool debugmode = false;

DWORD chan;


//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE g_hInstance,HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND g_hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void initOpenGL(void);

void renderFrame(float time);
void shutDown(void);
void initMusic();
void freeMusic();

void readScript();

Texture *findTexture(const char *name);


Texture *findTexture(const char *name)
{
    int i;
    int count = texturelist->getNodeCount();
    texturelist->goToStart();

    for (i=0;i<count;i++)
    {
        Texture *t = (Texture *)texturelist->getCurrent();
        if (strcmp(t->getName(), name)==0)
        {
            return t;
        }
        texturelist->goForward();
    }
    //pitäisi kai palauttaa null.. 
    return 0;//(Texture *)texturelist->getFirst();

}



void Error(char *es)
{
}

void initMusic()
{
	QWORD pos;

    if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		printf("An incorrect version of BASS was loaded");
		return;
	}
	if (!BASS_Init(-1,44100,0,0,NULL))
		Error("Can't initialize device");

	if (chan=BASS_StreamCreateFile(FALSE,songname.c_str(),0,0,BASS_SAMPLE_LOOP))
    {
		pos=BASS_ChannelGetLength(chan);
    }
	BASS_ChannelPlay(chan,FALSE);
    int value = BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, starttime*0.001f));
}
void freeMusic()
{
	BASS_Free();
}

void readScript()
{
    ifstream tiedosto("script.txt", ios::in);
    ofstream loki("log.txt", ios::out);
    
    string rivi;
    if (tiedosto.is_open())
    {
        while (!tiedosto.eof())
        {
            getline(tiedosto,rivi);
            //tyhjä tai kommenttirivi
            if (rivi.empty() || rivi[0]=='#')
                continue;

            //isot kirjaimet pieniksi
            transform(rivi.begin(), rivi.end(), rivi.begin(), tolower);

            string datadir = "data\\\\";
            //pilkotaan pieniksi palasiksi ja tallennetaan vectoriin

            stringstream ss(rivi);
            vector<string> tokens;
            string buffer;
            while (ss >> buffer)
            {
                tokens.push_back(buffer);
            }

            //tokens-vectorissa nyt kaikki palaset
            //parsitaan komennot kokoon

            if (tokens[0]=="loadsong")
            {
                songname = datadir+tokens[1];
                continue;
            }
            //TODO: jpeg-lataukset sun muut initin jälkeen

            if (tokens[0]=="loadjpg")
            {
                string filename = tokens[1];
                Texture *temp = new Texture();
                if (!temp->loadJPG(filename.c_str()))
                {
                    loki << "Ei voi ladata tiedostoa " << filename << endl;
                }
                else
                {
                    texturelist->add(temp);
                }

                continue;
            }

            if (tokens[0]=="xres")
            {
                XRES = atoi(tokens[1].c_str());
                continue;
            }
            if (tokens[0]=="yres")
            {
                YRES = atoi(tokens[1].c_str());
                continue;
            }
            if (tokens[0]=="starttime")
            {
                starttime = atoi(tokens[1].c_str());
                continue;
            }


            if (tokens[0]=="endtime")
            {
                endtime = atoi(tokens[1].c_str());
                continue;
            }
            if (tokens[0]=="debug")
            {
                debugmode = true;
                continue;
            }
            //don't read anything else after this
            if (tokens[0]=="endscript")
            {
                break;
            }

            //effects past this point

            float alkuaika = (float)atoi(tokens[1].c_str());
            float loppuaika = (float)atoi(tokens[2].c_str());
            if (tokens[0]=="picshow")
            {
                string kuvannimi = tokens[3];
                picshow *e = new picshow();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="picpulse")
            {
                string kuvannimi = tokens[3];
                float maxpulse = (float)atof(tokens[4].c_str());

                picpulse *e = new picpulse();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(kuvannimi.c_str()));
                e->setMaxPulse(maxpulse);
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="picfadein")
            {
                string kuvannimi = tokens[3];
                float fadeaika = (float)atof(tokens[4].c_str());

                picfadein *e = new picfadein();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(kuvannimi.c_str()));
                e->setFadeTime(fadeaika);
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="picfadeout")
            {
                string kuvannimi = tokens[3];
                float fadeaika = (float)atof(tokens[4].c_str());

                picfadeout *e = new picfadeout();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(kuvannimi.c_str()));
                e->setFadeTime(fadeaika);
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="pallo")
            {
                pallo *e = new pallo();
                e->init(alkuaika, loppuaika);
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="munkki")
            {
                string kuvannimi = tokens[3];

                munkki *e = new munkki();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="munkki2")
            {
                string kuvannimi = tokens[3];

                munkki2 *e = new munkki2();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="munkki3")
            {
                string kuvannimi = tokens[3];

                munkki3 *e = new munkki3();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                
                continue;
            }
            if (tokens[0]=="tunneli")
            {
                tunneli *e = new tunneli();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(tokens[3].c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="tunneli2")
            {
                tunneli2 *e = new tunneli2();
                e->init(alkuaika, loppuaika);
                e->setTexture(findTexture(tokens[3].c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="kuutio")
            {
                kuutio *e = new kuutio();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                string kuvannimi2 = tokens[4];
                e->setTexture(findTexture(kuvannimi.c_str()), findTexture(kuvannimi2.c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="vilkku")
            {
                vilkku *e = new vilkku();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="levy")
            {
                levy *e = new levy();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="twister")
            {
                twister *e = new twister();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="wobble")
            {
                wobble *e = new wobble();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                continue;
            }

            
            loki << "moskaa!!!!" << endl;
        }
    }
    else
    {
        //script.txt puuttuu

    }
    tiedosto.close();
    loki.close();
}







int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	WNDCLASSEX winClass;
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
    winClass.hIcon	       = NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
    winClass.hIconSm	   = NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;
	
	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL,"MY_WINDOWS_CLASS",
						    "Brainstorm -:- Old's Cool",
							WS_OVERLAPPEDWINDOW,
					 	    0,0, XRES,YRES, NULL, NULL, g_hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

    texturelist = new Lista();
    eventlist = new Lista();

    initOpenGL();
    readScript();

    console = new Console();
    timer = new Timer();
    beat = new BPM(120, 0);
    beat2 = new BPM(60, 0);
    beat4 = new BPM(30, 0);

    timer->init();
    unsigned int flags = 0;
    initMusic();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
        {
            int bassPosition = BASS_ChannelGetPosition(chan);
            int bassTime = BASS_ChannelBytes2Seconds(chan, bassPosition)*1000;
            int time = bassTime;//timer->getMilliseconds();

            if (time >= endtime)
            {
                goto done;
            }

            console->clear();
            timer->update();

            beat->update(time);
            beat2->update(time);
            beat4->update(time);
            char title[80];
//            sprintf(title, "time = %d, bassTime = %d, diff = %d", time, bassTime, bassTime - time);
            if (debugmode)
            {
                sprintf(title, "time = %d", time);
                SetWindowText(g_hWnd, title);
            }

		    renderFrame(time);
        }
	}

done:

    delete console;
    delete timer;
    delete beat;
    delete beat2;
    delete beat4;
	shutDown();

    delete texturelist;
    delete eventlist;

    UnregisterClass( "MY_WINDOWS_CLASS", g_hInstance );

	return uMsg.wParam;
}

LRESULT CALLBACK WindowProc( HWND   g_hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{

    int bassPosition = BASS_ChannelGetPosition(chan);
    float bassTime = BASS_ChannelBytes2Seconds(chan, bassPosition);
    
    switch( msg )
	{
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

                case VK_UP:
                break;

                case VK_DOWN:

                break;

                case VK_RIGHT:
                    if (debugmode)
                    {
                        BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, bassTime+1.000f));
                    }

                break;

                case VK_LEFT:
                    if (debugmode)
                    {
                        BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, bassTime-1.000f));
                    }

                break;


			}
		}
        break;

        case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}

        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;
		
		default:
		{
			return DefWindowProc( g_hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

void initOpenGL( void )
{
	GLuint PixelFormat;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 16;
    pfd.cDepthBits = 16;
	
	g_hDC = GetDC( g_hWnd );
	PixelFormat = ChoosePixelFormat( g_hDC, &pfd );
	SetPixelFormat( g_hDC, PixelFormat, &pfd );
	g_hRC = wglCreateContext( g_hDC );
	wglMakeCurrent( g_hDC, g_hRC );

	glClearColor(0,0,0, 1.0f );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, (float)XRES / (float)YRES, 0.1f, 3000.0f );
    glMatrixMode(GL_MODELVIEW);
}

void shutDown( void )	
{
	if( g_hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;							
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}
}
void renderFrame(float time)
{
    int i;
	// Clear the screen and the depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    int count = eventlist->getNodeCount();
    eventlist->goToStart();

    for (i=0;i<count;i++)
    {
        Event *e = (Event *)eventlist->getCurrent();

        if (e->isActive(time))
        {
            e->frameUpdate(time);
            e->draw(time);
        }
        eventlist->goForward();
    }

    SwapBuffers( g_hDC );
}

