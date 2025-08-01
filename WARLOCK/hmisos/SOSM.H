/****************************************************************************

   File              : sosm.h

   Programmer(s)     : Don Fowler

   Date              : 5/5/95

   Purpose           : Include Files For MIDI System

   Last Updated      : 5/5/95

****************************************************************************
               Copyright(c) 1992,1995 Human Machine Interfaces 
                            All Rights Reserved
****************************************************************************/

#ifndef  _SOS_MIDI_DEFINED   
#define  _SOS_MIDI_DEFINED   

#include <stdlib.h>
#include <windows.h>
#include "sos.h"

#ifdef __cplusplus
extern "C" {
#endif

// define to flag a track in the steal list
#define  _MIDI_TRACK_IN_STEAL_LIST  0x0020       

// _SOS_MIDI_DIGI.wFlags
#define  _SOS_VELOCITY_SENSING      0x0004
  
// window messages
#define  _SOS_SONG_CALLBACK   ( _SOS_WM_BASE + 0x10 )
 
// id for the MIDI_MAPPER device
#define  _MIDI_MAPPER_ID  0xffff

// defines for the loadable driver ids
#define  _MIDI_SOUND_MASTER_II      0xa000
#define  _MIDI_MPU_401              0xa001
#define  _MIDI_FM                   0xa002
#define  _MIDI_OPL2                 0xa002
#define  _MIDI_CALLBACK             0xa003
#define  _MIDI_MT_32                0xa004
#define  _MIDI_DIGI                 0xa005  
#define  _MIDI_INTERNAL_SPEAKER     0xa006
#define  _MIDI_WAVE_TABLE_SYNTH     0xa007  
#define  _MIDI_AWE32                0xa008  
#define  _MIDI_OPL3                 0xa009  
#define  _MIDI_GUS                  0xa00a  

// flags for pausing a song
#define  _MUTE                            0x0001

// flag to indicate that an element has been initialized
#define  _INITIALIZED                     0x4000

// flag to indicate that the pointers have been normalized
#define  _NORMALIZED                      0x2000

// flag to indicate that a driver has been loaded
#define  _LOADED                          0x0100

// define for the number of functions each MIDI driver can have
#define  _MIDI_DRV_FUNCTIONS              13

// define for the maximum and initial volume for the system
#define  _MIDI_MAX_VOLUME                 0x7f
                                              
// maximum number of songs that the system can play at any time
#define  _MIDI_MAX_SONGS                  0x20

// define for the maximum size of a chunk ID string
#define  _MIDI_MAX_CHUNK_ID_SIZE          0x20

// define for the maximum number of drivers that the MIDI system can use
#define  _MIDI_MAX_DRIVERS                0x08

// defines for the volume fading system
#define  _MIDI_FADE_IN                    0x01  
#define  _MIDI_FADE_OUT                   0x02  
#define  _MIDI_FADE_OUT_STOP              0x04  
#define  _MIDI_VOLUME_FADE                0x08

// define for the track/song flags to indicate that the process is paused
#define  _PAUSED                          0x20
  
// flag for a track that is playing, but not sending data out to the 
// driver
#define  _SUPRESSED                       0x10

// flag for a track to indicate that it has been mapped into the dependency
// list
#define  _MAPPED                          0x0080

// define for the channel structure priority to signal that a channel cannot
// be stolen, it is locked
#define  _MIDI_CHANNEL_LOCKED             0x0400

// error return for the MIDI DIGI system
#define  _MIDI_DIGI_ERR                   -1

// pack the note structure for BYTE alignment to save memory
#pragma pack( 1 )
 
// structure for a note in the note list
typedef struct _tag_midi_note
{
   // song that the note is associated to 
   struct _tag_midi_song * psSong;

   // track that the note is associated to
   struct _tag_midi_track * psTrack;

   // channel that the note was started on
   BYTE  bDriverAndChannel;

   // note number
   BYTE  bNote;

   // note delta time
   DWORD dwDelta;

   // pointer to the next note in the list, if the pointer is NULL
   // then the list is terminated
   struct _tag_midi_note * psNext;

} _MIDI_NOTE;
typedef _MIDI_NOTE * PNOTE;

// structure for initializing a digital driver 
typedef struct
{
   // flags for the driver
   // _SOS_VELOCITY_SENSING
   WORD     wFlags;

   // max voices for the driver to use
   WORD     wVoices;

   // handle of the digital driver to use
   HSOS   hDIGIDriver;

} _SOS_MIDI_DIGI_DRIVER;
typedef _SOS_MIDI_DIGI_DRIVER * PSOSMIDIDIGIDRIVER;

// structure for initializing a driver
typedef struct _tag_sos_midi_driver
{  
   // type of driver to use if using a digital driver
   WORD        wDIGIID;

   // pointer to digital driver initialization information
   PSOSMIDIDIGIDRIVER pDIGIInit;

   // miscellaneous WORD parameter for driver
   WORD  wParam;

   // miscellaneous DWORD parameter for driver
   DWORD dwParam;

} _SOS_MIDI_DRIVER;
typedef _SOS_MIDI_DRIVER * PSOSMIDIDRIVER;

// structure for initializing a song
typedef struct _tag_sos_midi_song
{  
   // pointer to song memory 
   BYTE * pSong;

   // temporary pointer to song memory for 32 to 16 bit conversion
   DWORD lp32Data;

   // temporary pointer to init song structure
   DWORD lp32InitSong;

   // pointer to callback function for pertinent song information
   VOID ( * pfnCallback )( HSOS, DWORD );

   // user data field
   DWORD dwUserData; 

   // handle of the window to send all driver messages to
   // _SOS_NO_WINDOW
   HWND  hWnd;
   DWORD dwTemp1[ 2 ];

} _SOS_MIDI_SONG;
typedef _SOS_MIDI_SONG * PSOSMIDISONG;

// device capabilities structure
typedef struct
{
   // flags for a driver to use
   // _SOS_MME          0x0001 
   // _SOS_LOADABLE     0x0002
   // _SOS_AUTO         0x0004
   WORD  wFlags;

   // MIDI driver flags
   // _SOS_MIDI_CONNECTED
   WORD  wMIDIFlags;

   // MIDI driver ID associated to digital driver
   WORD  wMIDIPID;

   // manufacturer ID, MIDIOUTCAPS
   // MM_MICROSOFT
   WORD  wMID;       

   // product ID, MIDIOUTCAPS
   WORD  wPID;        

   // DIGI driver flags
   // _SOS_DIGI_CONNECTED
   WORD  wDIGIFlags;

   // DIGI driver ID associated to midi driver
   WORD  wDIGIPID;

   // version of MME driver, major/minor respectively, MIDIOUTCAPS
   WORD  wVersion;   

   // name of the device given by the device manufacturer, MIDIOUTCAPS
   BYTE  szName[ MAXPNAMELEN ]; 

   // name of driver to open 
   BYTE  szDriverName[ _SOS_DRIVER_NAME_SIZE ];

   // device ID for the MME driver
   // 0 - # of wave drivers
   // 0xffff _MME_MIDI_MAPPER
   // 0x8000 _SOS_INVALID_DRIVER
   HSOS hMMEDeviceID;

   // port for the SOS driver to use
   WORD  wPort;

   // irq for the SOS driver to use
   WORD  wIRQ;

   // extra parameter for the device to use
   WORD  wParam;

   // base SOS driver ID
   WORD  wSOSDriverID;

   // technology for the driver
   WORD  wTechnology;

   // number of voices that the driver has available 
   WORD  wVoices;

   // channel mask
   WORD  wChannelMask;

   // number of notes available
   WORD  wNotes;

   // advanced options
   DWORD dwAdvanced;

   // flag to indicate that MME driver is to be forced,
   // if _SOS_AUTO is specified and this flag is set
   // then use MME driver, else use loadable
   WORD wForceMME;   

   // information for initializing a MIDI DIGI driver
   _SOS_MIDI_DIGI_DRIVER sDIGIDriver;

} _SOS_MIDI_DEVICE_CAPS;
typedef _SOS_MIDI_DEVICE_CAPS *  PSOSMIDIDEVICECAPS;

// structure of all of the MME only data
typedef struct
{
   // handle to be fill in on the call to midiOutOpen
   HSOS hDriver;

} _MIDI_MME_INFO;
typedef _MIDI_MME_INFO * PMIDIMMEINFO;

// structure for a drivers channel
typedef struct _tag_midi_channel
{
   // flags 
   // _ACTIVE
   // _MIDI_CHANNEL_LOCKED
   WORD  wFlags;

   // number of tracks using the channel
   WORD  wTracks;

   // channel number that the channel structure is relative to
   WORD  wChannel;

   // current song that owns the channel, -1 if no owner
   struct _tag_midi_song * psOwnerSong;

   // current priority of owner channel, -1 if no owner
   WORD  wOwnerPriority;

   // volume for the channel
   WORD  wVolume;

   // volume for the driver 
   WORD  wDriverVolume;

   // final volume used to scale channel and drum volumes
   WORD  wVolumeScalar;

   // pointer to dependency list for owner tracks
   struct _tag_midi_track * * psOwnerDependency;

} _MIDI_CHANNEL;
typedef _MIDI_CHANNEL * PCHANNEL;

// structure with all of the information needed for a driver
typedef struct
{
   // flags 
   // _SOS_MME                   0x0001
   // _SOS_INITIALIZED           0x8000
   WORD  wFlags;

   // flags to indicate which systems have been initialized
   // _SOS_MME_DRIVER_INIT          0x0001
   WORD  wSysInitFlags;

   // handle of driver the structure is associated to
   HSOS hDriver;

   // MME specific data 
   _MIDI_MME_INFO sMME;

   // capabilities structure information
   _SOS_MIDI_DEVICE_CAPS  sCaps;
     
   // owner count, this is incremented every time an application 
   // initializes the driver, the driver cannot be deinitialized until
   // the owner count reaches 0
   WORD  wOwners;

   // extra parameters for the driver 
   WORD  wParam;
   DWORD dwParam;

   // handle of opened MME device drivier
   HSOS hMMEOpenedDriver;

   // driver ID 
   WORD  wID;

   // pointer to channel information for the driver
   PCHANNEL psChannel;

   // pointer to the function to send data to the driver
   WORD ( cdecl * pfDataFunction )( PSTR, DWORD, DWORD );

   // array of pointers to function for the system to call a driver with
   WORD ( cdecl * pfFunction[ _MIDI_DRV_FUNCTIONS ] )(
      PSTR, DWORD, DWORD );

} _MIDI_DRIVER;
typedef _MIDI_DRIVER * PMIDIDRIVER;

// structure for a logical channel used by a track
typedef struct _tag_midi_track_channel
{
   // flags 
   // _ACTIVE
   short wFlags;

   // last data sent for each of the following events
   BYTE  bAftertouch;

   // last data sent for each of the following controllers
   BYTE  bController[ 128 ];  

} _MIDI_TRACK_CHANNEL;
typedef _MIDI_TRACK_CHANNEL * PTRACKCHANNEL;

// structure for a tracks channel stealing information
typedef struct _tag_midi_steal_info
{
   // track priority
   short  wPriority;

   // original channel for the track
   short  wRawChannel;
  
   // remapped channel
   short  wRemapChannel;

} _MIDI_STEAL_INFO;
typedef _MIDI_STEAL_INFO * PSTEALINFO;

// structure for a tracks branching information
typedef struct _tag_midi_branch_info
{
   // pointer to the branch list for the track
   PSTR * pBranchList;

   // array of flags to signal which MIDI data to restore on a branch
   DWORD    dwCtrlFlags[ 4 ];

} _MIDI_BRANCH_INFO;
typedef _MIDI_BRANCH_INFO * PBRANCHINFO;

// structure for a track in a song
typedef struct _tag_midi_track
{
   // chunk string for the track "HMI-MIDITRACK"
   BYTE  szHMIChunk[ _MIDI_MAX_CHUNK_ID_SIZE ];

   // flags 
   // _ACTIVE
   // _SUPRESSED
   // _INITIALIZED
   short  wFlags;

   // bit array for instruments that are used
   DWORD dwInsUsed[ 4 ];

   // bit array for drum instruments that are used
   DWORD dwDrumInsUsed[ 4 ];

   // current running status for the track
   BYTE bRunningStatus;

   // pointer to the owner song
   struct _tag_midi_song * psSong;

   // track number in song, this is required to reinsert a track that has
   // finished and is being restarted into the track list in the correct
   // order
   short  wTrackIndex;

   // original track number in song, this is required for the application
   // to reference the tracks based on the original song
   short wOriginalTrackIndex; 

   // pointer to the track name data space
   PSTR pTrackNames;

   // global position in track, in delta ticks
   DWORD dwTotalTicks;
 
   // delta time until the next event on the track
   DWORD dwDelta;

   // initialization pointer to the track data
   BYTE * pDataInit;

   // pointer to the track data
   unsigned char * pData;

   // driver for the track to send data out
   short hDriver;

   // number of events on track
   short wEvents;

   // branch information structure
   _MIDI_BRANCH_INFO sBranch;

   // channel stealing information
   _MIDI_STEAL_INFO  sSteal;
  
   // pointer to the channel information for the track, this is a pointer
   // so that tracks that share the same channel can access the same 
   // channel information structure, this structure is in the song
   BYTE * pTrackChannel;

   // pointer to the channel information for the track, this structure is
   // allocated when the driver that is associated with the channel is
   // initialized
   PCHANNEL psDriverChannel;

   // pointer to the track/channel dependancy list. the list is a pointer
   // for each track to a list of tracks that use the same channel and
   struct _tag_midi_track * * psTrackDependencyList;

   // pointer to the first track in the dependency list
   struct _tag_midi_track * psDependentTrack;

   // pointer to the next and previous track in the list, if the pointer
   // to the next is NULL then the list is terminated, if the pointer
   // to the first is NULL then this is the first track in the list
   struct _tag_midi_track * psNext;
   struct _tag_midi_track * psPrev;

   // pointer to the next track in the tracks that need a channel
   struct _tag_midi_track * psStealNext;

} _MIDI_TRACK;
typedef _MIDI_TRACK * PTRACK;

// structure for a volume fade event
typedef struct _tag_midi_volume
{
   // flags
   // _VOLUME_FADE
   short  wFlags;

   // current volume for the song
   short  wVolume;

   // current volume for the song
   DWORD dwFadeVolume;

   // start volume for the song during a fade
   short  wStartVolume;

   // current fractional amount to adjust volume
   DWORD dwFraction;

   // direction to fade song
   // _MIDI_FADE_OUT_STOP
   // _MIDI_FADE_OUT
   // _MIDI_FADE_IN
   short  wDirection;

   // number of ticks to fade the song over
   short  wTicks;

   // number of ticks to skip between sending out data
   short  wSkip;

   // current copy of ticks to skip between sending data out 
   short  wSkipCurrent;

   // start and end volumes for the song fade
   short  wStart;
   short  wEnd;

} _MIDI_VOLUME;

// structure for a song
typedef struct _tag_midi_song
{
   // chunk string for the song "HMI-MIDISONG"
   BYTE  szHMIChunk[ _MIDI_MAX_CHUNK_ID_SIZE ];

   // flags
   // _ACTIVE
   short  wFlags;

   // pointer to names of patch files, the area contains 
   // [count] - byte, count of patch file names
   // [id] - byte, patch file ID
   // [szPatchName] - null terminated string with patch name
   PSTR pPatchNames;

   // array to store the controller indexes, the system can find the index
   // into the local tracks controller array by indexing this array with
   // the controller value. e.g. controller 7 would index this array
   // at index 7 and find the value 1 which would mean that at index
   // one into the local track controller storage structure the value
   // for controller 7 can be saved or retreived
   BYTE bCtrlrIndexes[ 128 ];

   // init song structure
   _SOS_MIDI_SONG sInit;

   // handle for the timer event used to play the song
   short hEvent;

   // handle for the song
   short hSong;
   
   // total number of ticks gone by in the file, in delta ticks
   DWORD dwTotalTicks;

   // total number of ticks gone by in the file
   DWORD dwGlobalTicks;

   // ticks per quarter note
   short  wTicksPerQuarterNote;

   // ticks per second to play song at
   short  wPlayRate;

   // pointer to the tempo change list
   BYTE * pTempoChangeList;

   // pointer to the time change list
   BYTE * pTimeChangeList;

   // pointer to the note list memory, to be allocated when a song is 
   // initialized
   PNOTE psNoteList;

   // total number of notes used in song
   short wTotalNotes;

   // total number of tracks in the song  
   short wTotalTracks;

   // number of active tracks in the song
   short wActiveTracks; 

   // pointer to the list of track pointers
   PTRACK * psTrackList;

   // pointer to the first and last track in the track list, this is the
   // location to go to when a track traverse is started
   PTRACK psTrackFirst;
   PTRACK psTrackLast;

   // pointer to the channel/track dependency pointers
   BYTE * pDependencyPtrs;

   // pointer to the channel/track dependency pointers
   PTRACK * psChannelTrackDependencyLists;

   // pointer to the first and last element in the note list, this is the 
   // location to go to when a search for a note is started 
   PNOTE psNoteFirst;
   PNOTE psNoteLast;

   // volume fade structure
   _MIDI_VOLUME sVolume;

   // callback function for the song
   VOID ( * pfSongCallback )( HSOS ); 

   // branch callback function
   WORD ( * pfBranchCallback )( HSOS, WORD, WORD, DWORD );

   // loop callback function
   WORD ( * pfLoopCallback )( HSOS, WORD, WORD, DWORD );

   // trigger callback function
   WORD ( * pfTriggerCallback )( HSOS, BYTE, BYTE, DWORD ); 

   // pointer to the next song in the list, if the pointer is NULL
   // then the list is terminated
   struct _tag_midi_song * psNext;

   // handle of the window to send all driver messages to
   // _SOS_NO_WINDOW
   HWND hWnd;

   // user data field
   DWORD dwUserData;

   // temporary spacing for future releases
   DWORD dwTemp[ 0x03 ];

} _MIDI_SONG;
typedef _MIDI_SONG * PSONG;

#pragma pack()

#ifdef __cplusplus
}
#endif 

#include "sosmfnct.h"

#endif

