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
#include "events/picunwrap.hpp"
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
#include "events/pink.hpp"
#include "events/sirpale.hpp"
#include "events/spiraali.hpp"

using namespace std;

//------------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
/*
HDC	      g_hDC       = NULL;
HGLRC     g_hRC       = NULL;
HWND      g_hWnd      = NULL;
HINSTANCE g_hInstance = NULL;
*/
HWND hwnd;
HDC hdc;
HGLRC hrc;
PIXELFORMATDESCRIPTOR p;
int pixelformat;

DEVMODE screenSettings;
HINSTANCE hInstance;

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
bool fullscreen = true;
bool music;

DWORD chan;


//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE g_hInstance,HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND g_hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool initOpenGL(void);
void freeOpenGL(void);

void renderFrame(float time);
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
    ifstream tiedosto("data\\script.txt", ios::in);
//    ofstream loki("log.txt", ios::out);
    
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
                music = true;
                songname = datadir+tokens[1];
                continue;
            }
            //TODO: jpeg-lataukset sun muut initin jälkeen

            if (tokens[0]=="loadjpg")
            {
                string filename = datadir + tokens[1];
                Texture *temp = new Texture();
                if (!temp->loadJPG(filename.c_str()))
                {
//                    loki << "Ei voi ladata tiedostoa " << filename << endl;
                }
                else
                {
                    //remove the datadir from the name
                    temp->setName(tokens[1].c_str());
                    texturelist->add(temp);
                }

//                loki << "temp = "  << temp;

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
            if (tokens[0]=="picunwrap")
            {
                picunwrap *e = new picunwrap();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                e->setTexture(findTexture(kuvannimi.c_str()));
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
            if (tokens[0]=="pink")
            {
                pink *e = new pink();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="sirpale")
            {
                sirpale *e = new sirpale();
                e->init(alkuaika, loppuaika);
                string kuvannimi = tokens[3];
                e->setTexture(findTexture(kuvannimi.c_str()));
                eventlist->add(e);
                continue;
            }
            if (tokens[0]=="spiraali")
            {
                spiraali *e = new spiraali();
                e->init(alkuaika, loppuaika);
                eventlist->add(e);
                continue;
            }

            
//            loki << "moskaa!!!!" << endl;
        }
    }
    else
    {
        //script.txt puuttuu

    }
    tiedosto.close();
//    loki.close();
}







int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
    texturelist = new Lista();
    eventlist = new Lista();
	MSG        uMsg;
    memset(&uMsg,0,sizeof(uMsg));

    initOpenGL();

	glClearColor(0,0,0, 1.0f );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, (float)XRES / (float)YRES, 0.1f, 3000.0f );
    glMatrixMode(GL_MODELVIEW);

    readScript();

    console = new Console();
    timer = new Timer();
    beat = new BPM(120, 0);
    beat2 = new BPM(60, 0);
    beat4 = new BPM(240, 0);

    timer->init();
    unsigned int flags = 0;

    if (music)
    {
        initMusic();
    }

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
        {
            int time;
            if (music)
            {
                int bassPosition = BASS_ChannelGetPosition(chan);
                int bassTime = BASS_ChannelBytes2Seconds(chan, bassPosition)*1000;
                time = bassTime;//timer->getMilliseconds();
            }
            else
            {
                time = timer->getMilliseconds();
            }

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
                SetWindowText(hwnd, title);
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
	freeOpenGL();
    if (music)
    {
        freeMusic();

    }

    delete texturelist;
    delete eventlist;

    UnregisterClass( "MY_WINDOWS_CLASS", hInstance);

	return uMsg.wParam;
}

LRESULT CALLBACK WindowProc( HWND   g_hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{

    int bassPosition = BASS_ChannelGetPosition(chan);
    float bassTime = BASS_ChannelBytes2Seconds(chan, bassPosition);

    if (debugmode)
    {
        float speed = 0.300f;
        if (GetAsyncKeyState(VK_SHIFT))
        {
            speed *= 2;

        }
        if (GetAsyncKeyState(VK_RIGHT))
        {
            BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, bassTime+speed));
        }
        if (GetAsyncKeyState(VK_LEFT))
        {
            BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, bassTime-speed));
        }
    }

    
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

                break;

                case VK_LEFT:

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

bool initOpenGL( void )
{
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;	

    WNDCLASS wc;
		
	hInstance = GetModuleHandle(NULL);
	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	if(!RegisterClass(&wc)) return false;

	RtlZeroMemory(&screenSettings, sizeof(screenSettings));
	p.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	p.nVersion = 1;
	p.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	p.iPixelType=PFD_TYPE_RGBA;
	p.cColorBits=32;
	p.cDepthBits=16;
	p.iLayerType=PFD_MAIN_PLANE;

	RtlZeroMemory(&screenSettings, sizeof(screenSettings));
	screenSettings.dmSize = sizeof(screenSettings);
	screenSettings.dmPelsWidth = XRES;
	screenSettings.dmPelsHeight = YRES;
	screenSettings.dmBitsPerPel = 32;
	screenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	
	if (fullscreen)
	{
		DEVMODE dmScreenSettings;					// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= XRES;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= YRES;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= 32;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			return false;

		dwExStyle=WS_EX_APPWINDOW;					// Window Extended Style
		dwStyle=WS_POPUP;						// Windows Style
	}
	
	hwnd = CreateWindowEx(dwExStyle, "OpenGL", "Brainstorm -:- Old's cool", dwStyle, 0, 0, XRES, YRES, NULL, NULL, hInstance, NULL);
	hdc = GetDC(hwnd);
		
	pixelformat = ChoosePixelFormat(hdc, &p);
	SetPixelFormat(hdc, pixelformat, &p);
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);
	if (fullscreen)
		ShowCursor(FALSE);						// Hide Mouse Pointer
	else
		ShowCursor(true);

	ShowWindow(hwnd, SW_SHOW);			// iCmdShow kertoo ikkunan tilasta (koot yms);	
	SetForegroundWindow(hwnd);			// aseta päälimmäiseksi ikkunaksi, lisäämällä sen prioriteettia
	SetFocus(hwnd);						// asettaa näppiksen keskittymään tähän ikkunaan

    return true;
}

void freeOpenGL( void )	
{
	ChangeDisplaySettings(NULL, 0);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);
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

    SwapBuffers( hdc);
}

