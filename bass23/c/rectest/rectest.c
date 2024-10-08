// BASS Recording example, copyright (c) 2002-2006 Ian Luck.

#include <windows.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include "bass.h"

HWND win=NULL;

#define BUFSTEP 200000	// memory allocation unit

int input;				// current input source
char *recbuf=NULL;		// recording buffer
DWORD reclen;			// recording length

HRECORD rchan=0;		// recording channel
HSTREAM chan=0;			// playback channel

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

// messaging macros
#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)w,(LPARAM)l)
#define DLGITEM(id) GetDlgItem(win,id)

// buffer the recorded data
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, DWORD user)
{
	// increase buffer size if needed
	if ((reclen%BUFSTEP)+length>=BUFSTEP) {
		recbuf=realloc(recbuf,((reclen+length)/BUFSTEP+1)*BUFSTEP);
		if (!recbuf) {
			rchan=0;
			Error("Out of memory!");
			MESS(10,WM_SETTEXT,0,"Record");
			return FALSE; // stop recording
		}
	}
	// buffer the data
	memcpy(recbuf+reclen,buffer,length);
	reclen+=length;
	return TRUE; // continue recording
}

void StartRecording()
{
	WAVEFORMATEX *wf;
	if (recbuf) { // free old recording
		BASS_StreamFree(chan);
		chan=0;
		free(recbuf);
		recbuf=NULL;
		EnableWindow(DLGITEM(11),FALSE);
		EnableWindow(DLGITEM(12),FALSE);
	}
	// allocate initial buffer and make space for WAVE header
	recbuf=malloc(BUFSTEP);
	reclen=44;
	// fill the WAVE header
	memcpy(recbuf,"RIFF\0\0\0\0WAVEfmt \20\0\0\0",20);
	memcpy(recbuf+36,"data\0\0\0\0",8);
	wf=(WAVEFORMATEX*)(recbuf+20);
	wf->wFormatTag=1;
	wf->nChannels=2;
	wf->wBitsPerSample=16;
	wf->nSamplesPerSec=44100;
	wf->nBlockAlign=wf->nChannels*wf->wBitsPerSample/8;
	wf->nAvgBytesPerSec=wf->nSamplesPerSec*wf->nBlockAlign;
	// start recording @ 44100hz 16-bit stereo
	if (!(rchan=BASS_RecordStart(44100,2,0,&RecordingCallback,0))) {
		Error("Couldn't start recording");
		free(recbuf);
		recbuf=0;
		return;
	}
	MESS(10,WM_SETTEXT,0,"Stop");
}

void StopRecording()
{
	BASS_ChannelStop(rchan);
	rchan=0;
	MESS(10,WM_SETTEXT,0,"Record");
	// complete the WAVE header
	*(DWORD*)(recbuf+4)=reclen-8;
	*(DWORD*)(recbuf+40)=reclen-44;
	// create a stream from the recording
	if (chan=BASS_StreamCreateFile(TRUE,recbuf,0,reclen,0)) {
		// enable "play" & "save" buttons
		EnableWindow(DLGITEM(11),TRUE);
		EnableWindow(DLGITEM(12),TRUE);
	}
}

// write the recorded data to disk
void WriteToDisk()
{
	FILE *fp;
	char file[MAX_PATH]="";
	OPENFILENAME ofn={0};
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=win;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFile=file;
	ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
	ofn.lpstrFilter="WAV files\0*.wav\0All files\0*.*\0\0";
	ofn.lpstrDefExt="wav";
	if (!GetSaveFileName(&ofn)) return;
	if (!(fp=fopen(file,"wb"))) {
		Error("Can't create the file");
		return;
	}
	fwrite(recbuf,reclen,1,fp);
	fclose(fp);
}

void UpdateInputInfo()
{
	char *type;
	int it=BASS_RecordGetInput(input); // get info on the input
	MESS(14,TBM_SETPOS,TRUE,LOWORD(it)); // set the level slider
	switch (it&BASS_INPUT_TYPE_MASK) {
		case BASS_INPUT_TYPE_DIGITAL:
			type="digital";
			break;
		case BASS_INPUT_TYPE_LINE:
			type="line-in";
			break;
		case BASS_INPUT_TYPE_MIC:
			type="microphone";
			break;
		case BASS_INPUT_TYPE_SYNTH:
			type="midi synth";
			break;
		case BASS_INPUT_TYPE_CD:
			type="analog cd";
			break;
		case BASS_INPUT_TYPE_PHONE:
			type="telephone";
			break;
		case BASS_INPUT_TYPE_SPEAKER:
			type="pc speaker";
			break;
		case BASS_INPUT_TYPE_WAVE:
			type="wave/pcm";
			break;
		case BASS_INPUT_TYPE_AUX:
			type="aux";
			break;
		case BASS_INPUT_TYPE_ANALOG:
			type="analog";
			break;
		default:
			type="undefined";
	}
	MESS(15,WM_SETTEXT,0,type); // display the type
}

BOOL CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_TIMER:
			{ // update the recording/playback counter
				char text[30]="";
				if (rchan) // recording
					sprintf(text,"%I64d",BASS_ChannelGetPosition(rchan));
				else if (chan) {
					if (BASS_ChannelIsActive(chan)) // playing
						sprintf(text,"%I64d / %I64d",BASS_ChannelGetPosition(chan),BASS_ChannelGetLength(chan));
					else
						sprintf(text,"%I64d",BASS_ChannelGetLength(chan));
				}
				MESS(20,WM_SETTEXT,0,text);
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;
				case 10:
					if (!rchan)
						StartRecording();
					else
						StopRecording();
					break;
				case 11:
					BASS_ChannelPlay(chan,TRUE); // play the recorded data
					break;
				case 12:
					WriteToDisk();
					break;
				case 13:
					if (HIWORD(w)==CBN_SELCHANGE) { // input selection changed
						int i;
						input=MESS(13,CB_GETCURSEL,0,0); // get the selection
						// enable the selected input
						for (i=0;BASS_RecordSetInput(i,BASS_INPUT_OFF);i++) ; // 1st disable all inputs, then...
						BASS_RecordSetInput(input,BASS_INPUT_ON); // enable the selected
						UpdateInputInfo(); // update info
					}
					break;
			}
			break;

		case WM_HSCROLL:
			if (l) { // set input source level
				int level=SendMessage((HWND)l,TBM_GETPOS,0,0);
				BASS_RecordSetInput(input,BASS_INPUT_LEVEL|level);
			}
			break;

		case WM_INITDIALOG:
			win=h;
			// setup recording and output devices (using default devices)
			if (!BASS_RecordInit(-1) || !BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				DestroyWindow(win);
			} else { // get list of inputs
				int c;
				char *i;
				MESS(14,TBM_SETRANGE,FALSE,MAKELONG(0,100)); // initialize input level slider
				for (c=0;i=BASS_RecordGetInputName(c);c++) {
					MESS(13,CB_ADDSTRING,0,i);
					if (!(BASS_RecordGetInput(c)&BASS_INPUT_OFF)) { // this 1 is currently "on"
						input=c;
						MESS(13,CB_SETCURSEL,input,0);
						UpdateInputInfo(); // display info
					}
				}
				SetTimer(h,0,200,0); // timer to update the position display
				return 1;
			}
			break;

		case WM_DESTROY:
			// release all BASS stuff
			BASS_RecordFree();
			BASS_Free();
			break;
	}
	return 0;
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	{ // enable trackbar support (for the level control)
		INITCOMMONCONTROLSEX cc={sizeof(cc),ICC_BAR_CLASSES};
		InitCommonControlsEx(&cc);
	}

	DialogBox(hInstance,(char*)1000,0,&dialogproc);

	return 0;
}
