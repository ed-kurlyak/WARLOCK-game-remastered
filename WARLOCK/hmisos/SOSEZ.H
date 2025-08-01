/****************************************************************************

   File              : sosez.h

   Programmer(s)     : Don Fowler

   Date              : 3-2-95 

   Purpose           : Include file information for sosez.c

   Last Updated      : 3-2-95

****************************************************************************
               Copyright(c) 1992, 1995 Human Machine Interfaces 
                            All Rights Reserved
****************************************************************************/


#ifndef  _SOS_EZ
#define  _SOS_EZ

BOOL  sosEZStopStream         ( HSOS );
BOOL  sosEZFillStreamBuffer   ( HSOS, PSOSSTREAMELEMENT );
BOOL  sosEZStreamFile         ( PSTR, HWND, PHSOS );
BOOL  sosEZInitSystem         ( WORD, WORD, HWND, HWND );
BOOL  sosEZUnInitSystem       ( VOID );
BOOL  sosEZDriverActive       ( VOID );
BOOL  sosEZLoadSample         ( PSTR, PSOSSAMPLE * );
BOOL  sosEZUnLoadSample       ( PSOSSAMPLE );
DWORD sosEZGetError           ( VOID );
VOID  sosEZDisplayError       ( HWND );
HSOS sosEZGetDIGIHandle      ( VOID );
HSOS sosEZGetMIDIHandle      ( VOID );
UINT  sosEZHandleMessages     ( HWND, UINT, UINT, LONG );
BOOL  sosEZLoadFile           ( PSTR, PSTR *, DWORD * );
BOOL  sosEZUnLoadFile         ( PSTR, DWORD );
BOOL  sosEZLoadSong           ( PSTR, PSOSMIDISONG * );
BOOL  sosEZUnLoadSong         ( PSOSMIDISONG );

// define for no driver to be used
#define  _SOSEZ_NO_DRIVER     -1

// .WAV file header structure
#pragma pack( 1 )
typedef struct _tag_wavheader  
{
   BYTE szRIFF[ 4 ];
   DWORD dwFormatLength;
   BYTE szWAVE[ 4 ];
   BYTE szFMT[ 4 ];
   DWORD dwWaveFormatLength;
   short wFormatTag;
   short wChannels;
   DWORD dwSamplesPerSec;
   DWORD dwAvgBytesPerSec;
   short wBlockAlign;
   short wBitsPerSample;
   BYTE szDATA[ 4 ];
   DWORD dwDataLength;

} _WAVHEADER;
#pragma pack()
 
#endif

