/****************************************************************************

   File              : sosmfnct.h

   Programmer(s)     : Don Fowler

   Date              : 5/5/95

   Purpose           : Function prototypes for the MIDI system

   Last Updated      : 5/5/95

****************************************************************************
               Copyright(c) 1992,1995 Human Machine Interfaces 
                            All Rights Reserved
****************************************************************************/

#ifndef  _SOS_MIDI_FUNCTIONS_DEFINED   
#define  _SOS_MIDI_FUNCTIONS_DEFINED   

#ifdef __cplusplus
extern "C" {
#endif

DWORD WINAPI sosMIDIBranchToSongID        ( HSOS, BYTE );
DWORD WINAPI sosMIDIBranchToTrackID       ( HSOS, WORD, BYTE );
DWORD WINAPI sosMIDIRegisterLoopFunction  ( HSOS, WORD ( * )( HSOS, WORD,
                                            WORD, DWORD ) );
DWORD WINAPI sosMIDIRegisterBranchFunction( HSOS, WORD ( * )( HSOS, WORD, WORD, DWORD ) );
DWORD WINAPI sosMIDIRegisterTriggerFunction( HSOS, WORD ( * )( HSOS, BYTE, BYTE, DWORD ) );
DWORD WINAPI sosMIDISetDriverVolume        ( HSOS, WORD );
BOOL  WINAPI sosMIDIProcessEvent          ( PSONG, PTRACK, WORD, WORD, WORD,
                                            WORD, BYTE );
DWORD WINAPI sosMIDIMuteSong              ( HSOS );
DWORD WINAPI sosMIDIUnMuteSong            ( HSOS );
DWORD WINAPI sosMIDIPauseSong             ( HSOS, WORD );
DWORD WINAPI sosMIDIResumeSong            ( HSOS );
DWORD WINAPI sosMIDIStartSong             ( HSOS );
DWORD WINAPI sosMIDIStopSong              ( HSOS );
DWORD WINAPI sosMIDIStopAllSongs          ( VOID );
BOOL  WINAPI sosMIDISongDone              ( HSOS );
DWORD WINAPI sosMIDISendMIDIData          ( HSOS, PSTR, DWORD );
DWORD WINAPI sosMIDIInitSong              ( PSOSMIDISONG, HSOS * );
DWORD WINAPI sosMIDIUnInitSong            ( HSOS );
DWORD WINAPI sosMIDIResetSong             ( HSOS, PSOSMIDISONG );
DWORD WINAPI sosMIDISongAlterTempo        ( HSOS, WORD );
DWORD WINAPI sosMIDISetSongVolume         ( HSOS, WORD );
DWORD WINAPI sosMIDIGetSongVolume         ( HSOS );
DWORD WINAPI sosMIDIFadeSong              ( HSOS, WORD, WORD, WORD, WORD, WORD );
DWORD WINAPI sosMIDISetMasterVolume       ( WORD );
DWORD WINAPI sosMIDISetInsData            ( HSOS, PSTR, WORD );
DWORD WINAPI sosMIDISendMIDIEvent         ( HSOS, WORD, WORD, WORD, WORD );
DWORD WINAPI sosMIDIInitSystem            ( WORD, PSTR );
DWORD WINAPI sosMIDIUnInitSystem          ( VOID );
DWORD WINAPI sosMIDIInitDriver            ( PSOSMIDIDEVICECAPS, PHSOS );
DWORD WINAPI sosMIDIUnInitDriver          ( HSOS );
DWORD WINAPI sosMIDIResetDriver           ( HSOS );
DWORD WINAPI sosMIDISetActiveWindow       ( HSOS, HWND );
DWORD WINAPI sosMIDIDetectInit            ( WORD, PSTR );
DWORD WINAPI sosMIDIDetectUnInit          ( VOID );
DWORD WINAPI sosMIDIDetectFindFirst       ( PSOSMIDIDEVICECAPS );
DWORD WINAPI sosMIDIDetectFindNext        ( PSOSMIDIDEVICECAPS );
DWORD WINAPI sosMIDIGetSongNotesOn        ( HSOS );
DWORD WINAPI sosMIDIGetSongLocation       ( HSOS, WORD *, WORD *,
                                            WORD * );
DWORD WINAPI sosMIDIGetTrackLocation      ( HSOS, WORD, WORD *, 
                                            WORD *, WORD * );
DWORD WINAPI sosMIDICalculateLocation     ( PSONG, DWORD, WORD *,
                                            WORD *, WORD * );
DWORD WINAPI sosMIDIAcquireDrivers        ( VOID );
DWORD WINAPI sosMIDIReleaseDrivers        ( VOID );
DWORD WINAPI sosMIDISetDigitalPatchFile   ( HSOS, PSTR );
DWORD WINAPI sosMIDIReleaseDigitalPatchFile( HSOS );
DWORD WINAPI sosMIDIFlushDigitalPatches   ( HSOS );

#ifdef __cplusplus
}
#endif 

#endif
