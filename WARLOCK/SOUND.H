#include ".\hmisos\sos.h"
#include ".\hmisos\sosm.h"
#include ".\hmisos\sosez.h"

#pragma comment (lib, ".\\hmisos\\SOSWM32.LIB")

#define NUM_SOUNDS 5

void Init_SoundSystem(HWND hWnd);
void Load_SoundSample(PSOSSAMPLE &pSound, char* filename);
void Play_SoundSample(PSOSSAMPLE& pSound, DWORD Loop, DWORD Index);
void Stop_SoundSample(PSOSSAMPLE& pSound, DWORD Index);
void UnLoad_SoundSample(PSOSSAMPLE& pSample);
void Destroy_SoundSystem();


