/****************************************************************************

   File              : sosfnct.h

   Programmer(s)     : Don Fowler

   Date              : 1-16-95 

   Purpose           : Function prototypes for SOSLIB*.DLL  

   Last Updated      : 1-24-95

****************************************************************************
               Copyright(c) 1992, 1995 Human Machine Interfaces 
                            All Rights Reserved
****************************************************************************/

#ifndef  _SOS_DLL_FUNCTIONS
#define  _SOS_DLL_FUNCTIONS

#ifdef __cplusplus
extern "C" {
#endif

DWORD WINAPI sosDIGIInitSystem     ( WORD, PSTR );
DWORD WINAPI sosDIGIUnInitSystem   ( VOID );
DWORD WINAPI sosDIGIInitDriver     ( PSOSDIGIDEVICECAPS, PHSOS );
DWORD WINAPI sosDIGIUnInitDriver   ( HSOS );
DWORD WINAPI sosTIMERInitSystem    ( WORD );
DWORD WINAPI sosTIMERUnInitSystem  ( VOID );
DWORD WINAPI sosTIMERRegisterEvent ( WORD, VOID ( * )( VOID ),
                                     WORD * );
DWORD WINAPI sosTIMERAlterEventRate( HSOS, WORD );
DWORD WINAPI sosTIMERRemoveEvent   ( HSOS );
DWORD WINAPI sosTIMERGetEventRate  ( HSOS, PWORD );
DWORD WINAPI sosTIMERResumeEvent   ( HSOS );
DWORD WINAPI sosTIMERPauseEvent    ( HSOS );
DWORD WINAPI sosDIGIUserService    ( HSOS );
DWORD WINAPI sosDIGISetBufferSkip  ( HSOS, WORD );
DWORD WINAPI sosDIGIPauseOutput    ( HSOS );
DWORD WINAPI sosDIGIResumeOutput   ( HSOS );
DWORD WINAPI sosDIGISetActiveWindow( HSOS, HWND );
PSTR  WINAPI sosGetErrorString     ( DWORD );
DWORD WINAPI sosDIGIDetectInit     ( WORD, PSTR );
DWORD WINAPI sosDIGIDetectUnInit   ( VOID );
DWORD WINAPI sosDIGIDetectFindFirst( PSOSDIGIDEVICECAPS );
DWORD WINAPI sosDIGIDetectFindNext ( PSOSDIGIDEVICECAPS );
DWORD WINAPI sosDIGIAcquireDriver  ( HSOS );
DWORD WINAPI sosDIGIReleaseDriver  ( HSOS, WORD );
DWORD WINAPI sosDIGIStreamAcquire  ( HSOS, PSOSSTREAMINIT, PHSOS );
DWORD WINAPI sosDIGIStreamGetSampleHandle( HSOS, HSOS );
DWORD WINAPI sosDIGIStreamRelease  ( HSOS, HSOS );
DWORD WINAPI sosDIGIStreamPrepareBuffer( HSOS, HSOS, PSOSSTREAMELEMENT );
DWORD WINAPI sosDIGIStreamUnPrepareBuffer( HSOS, HSOS, PSOSSTREAMELEMENT );
DWORD WINAPI sosDIGIStreamAddBuffer( HSOS, HSOS, PSOSSTREAMELEMENT );
DWORD WINAPI sosDIGIStreamStart    ( HSOS, HSOS );
DWORD WINAPI sosDIGIStreamStop     ( HSOS, HSOS );
DWORD WINAPI sosDIGIStreamPause    ( HSOS, HSOS );
DWORD WINAPI sosDIGIStreamResume   ( HSOS, HSOS );
DWORD WINAPI sosDIGIGetDMAPosition ( HSOS );
DWORD WINAPI sosDIGISetSampleVolume( HSOS, HSOS, DWORD );
DWORD WINAPI sosDIGIGetSampleVolume( HSOS, HSOS );
DWORD WINAPI sosDIGIGetBytesProcessed( HSOS, HSOS );
DWORD WINAPI sosDIGIGetLoopCount   ( HSOS, HSOS );
DWORD WINAPI sosDIGISetLoopCount   ( HSOS, HSOS, WORD );
DWORD WINAPI sosDIGISetPanLocation ( HSOS, HSOS, WORD );
DWORD WINAPI sosDIGIGetPanLocation ( HSOS, HSOS );
DWORD WINAPI sosDIGISetSampleRate  ( HSOS, HSOS, WORD );
DWORD WINAPI sosDIGIGetSampleRate  ( HSOS, HSOS );
DWORD WINAPI sosDIGIGetPanSpeed    ( HSOS, HSOS );
DWORD WINAPI sosDIGIGetSampleID    ( HSOS, HSOS );
PSOSSAMPLE WINAPI sosDIGIGetSamplePointer( HSOS, HSOS );
DWORD WINAPI sosDIGIGetSampleHandle( HSOS, WORD );
DWORD WINAPI sosDIGISamplesPlaying ( HSOS );
BOOL  WINAPI sosDIGISampleDone     ( HSOS, HSOS );
DWORD WINAPI sosDIGIStartSample    ( HSOS, PSOSSAMPLE, HSOS * );
DWORD WINAPI sosDIGIStopSample     ( HSOS, HSOS );
DWORD WINAPI sosDIGIStopAllSamples ( HSOS );
DWORD WINAPI sosPrepare32Memory    ( PSTR, DWORD, DWORD * ); 
DWORD WINAPI sosUnPrepare32Memory  ( DWORD );
DWORD WINAPI sosInitDLL            ( VOID );
DWORD WINAPI sosUnInitDLL          ( VOID );
WORD  WINAPI sosUsing32BitDLL      ( VOID );

#ifdef __cplusplus
}
#endif 

#endif                         
