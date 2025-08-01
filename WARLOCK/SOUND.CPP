#include "sound.h"

HSOS hDig = NULL;

HSOS sounds[NUM_SOUNDS] = { 0 };

void Init_SoundSystem( HWND hWnd)
{
	if (sosInitDLL() != _ERR_NO_ERROR ||
		sosEZInitSystem(_SOS_MME, _SOS_MME, hWnd, hWnd) != _ERR_NO_ERROR)
	{
		MessageBox(NULL, "Error HMI SOS initialize!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(1);
	}
}

void Load_SoundSample(PSOSSAMPLE& pSample, char* filename)
{
	if(sosEZLoadSample(filename, &pSample) != _ERR_NO_ERROR)
	{
		MessageBox(NULL, "Error HMI SOS initialize!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(1);
	}
}

void Play_SoundSample(PSOSSAMPLE& pSample, DWORD Loop, DWORD Index)
{
	hDig = sosEZGetDIGIHandle();
	sosDIGIAcquireDriver(hDig);
	pSample->dwLoopCount = Loop;
	
	sosDIGIStartSample(hDig, pSample, &sounds[Index]);
}

void Stop_SoundSample(PSOSSAMPLE& pSound, DWORD Index)
{
		if (sounds[Index] != NULL)
		{
			sosDIGIStopSample(hDig, sounds[Index]);
			sounds[Index] = NULL;
		}
}

void UnLoad_SoundSample(PSOSSAMPLE& pSample)
{
	if (pSample)
		sosEZUnLoadSample(pSample);
}

void Destroy_SoundSystem()
{
	    sosEZUnInitSystem();
        sosUnInitDLL();
        PostQuitMessage(0);
}

