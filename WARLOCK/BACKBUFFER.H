#pragma once

#include ".\scitech\INCLUDE\MGRAPH.H"
//#pragma comment (lib, "./scitech/LIB/WIN32/VC/MGLLT.LIB")
#pragma comment(lib, ".\\scitech\\LIB\\WIN32\\VC\\MGLLT.LIB")
#pragma comment(lib, "winmm.lib")

#define LARGE_VIEW 1

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

extern MGLDC *windc;
extern MGLDC *dibdc;

struct Palette
{
	int r;
	int g;
	int b;
};

extern int Screen_Width;
extern int Screen_Height;


extern Palette GamePalette[256];

void Create_Palette();
void Create_BackBuffer();
void Clear_BackBuffer();
void Present_BackBuffer();
void Delete_BackBuffer();
