/****************************************************************************

   File              : sos.h

   Programmer(s)     : Don Fowler

   Date              : 1-16-95 

   Purpose           : General defines for SOSLIB*.DLL

   Last Updated      : 1-24-95

****************************************************************************
               Copyright(c) 1992, 1995 Human Machine Interfaces 
                            All Rights Reserved
****************************************************************************/


#ifndef  _SOS_DLL_SOS
#define  _SOS_DLL_SOS

// include files for WINDOWS definitions
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdlib.h>

// define for the SOS handle type
typedef WORD       HSOS;
typedef HSOS *     PHSOS;   

// defines for the driver types
#define _SOS_MME                       0x0001
#define _SOS_NATIVE                    0x0002
#define _SOS_AUTO                      0x0004

// define for the release driver function to indicate that it is to 
// wait for the waves to all be played before releasing
#define  _SOS_WAIT_FOR_WAVES     0x1000

// macro for creating a left/right volume value for the mixer
#define MK_VOLUME( l, r ) ( ( short )r << 16 | ( short )l )

// mixer channels default
#define  _SOS_MIXER_CHANNELS  32

// active flag for sos system
#define  _SOS_ACTIVE          0x8000

// sample flags
#define  _SACTIVE             0x8000
#define  _SPROCESSED          0x4000
#define  _SDONE               0x2000
#define  _SASR_LOOP           0x1000

// sample formats
#define  _PCM_UNSIGNED        0x8000

// driver format
#define  _DRV_UNSIGNED        0x8000
#define  _DRV_SWAP_CHANNELS   0x4000

// pcm defines
#define  _PCM_16_BIT       16
#define  _PCM_8_BIT        8
#define  _PCM_MONO         1
#define  _PCM_STEREO       2

// channel equates
#define  _PAN_LEFT         0x00000000
#define  _PAN_CENTER       0x00008000
#define  _PAN_RIGHT        0x0000ffff

// volume max
#define  _VOLUME_MAX       0x7fff7fff

// SOSDLL window messages 
// SOSDLL base window message value  
#define  _SOS_WM_BASE                  0xa000

// SOS command define
#define  _SOS_COMMAND                  _SOS_WM_BASE

// streaming system window messages
#define  _SOS_STREAM_BUFFER_DONE       _SOS_WM_BASE
#define  _SOS_STREAM_STARVED           ( _SOS_WM_BASE + 1 )
#define  _SOS_STREAM_DONE              ( _SOS_WM_BASE + 2 )

// sample callback messages sent to the applications window 
#define _SOS_SAMPLE_CALLBACK           ( _SOS_WM_BASE + 3 )

// error creation macro 
#define _SOS_MAKE_ERROR( h, l )  ( ( DWORD )( ( DWORD )h << 0x10 ) | l )
#define _ME( h, l )              ( ( DWORD )( ( DWORD )h << 0x10 ) | l )

// error types 
enum
{
   _ERR_NO_ERROR,                // _ERR_NO_ERROR, 0x00  
   _ERR_SYSTEM_INITIALIZED,      // _ERR_SYSTEM_INITIALIZED, 0x00
   _ERR_SYSTEM_NOT_INITIALIZED,  // _ERR_SYSTEM_NOT_INITIALIZED, 0x00
   _ERR_NO_HANDLES,              // _ERR_NO_HANDLES, 0x00
   _ERR_INVALID_HWND,            // _ERR_INVALID_HWND, lpsCaps->hWnd
   _ERR_DRIVER_NOT_INITIALIZED,  // _ERR_DRIVER_NOT_INITIALIZED, hDriver
   _ERR_DRIVER_NOT_MME,          // _ERR_DRIVER_NOT_MME, hDriver
   _ERR_NO_DRIVER_FOUND,         // _ERR_NO_DRIVER_FOUND, 0x00
   _ERR_INVALID_POINTER,         // _ERR_INVALID_POINTER, 0x00
   _ERR_SETTINGS_NOT_FOUND,      // _ERR_SETTINGS_NOT_FOUND, 0x00
   _ERR_NO_HARDWARE_SETTINGS,    // _ERR_NO_HARDWARE_SETTINGS, 0x00
   _ERR_INVALID_PORT,            // _ERR_INVALID_PORT, port
   _ERR_INVALID_DMA,             // _ERR_INVALID_DMA, dma 
   _ERR_INVALID_IRQ,             // _ERR_INVALID_IRQ, irq
   _ERR_INVALID_WPARAM,          // _ERR_INVALID_WPARAM, wparam
   _ERR_MEMORY_FAIL,             // _ERR_MEMORY_FAIL, 0x00
   _ERR_INVALID_DRIVER_ID,       // _ERR_INVALID_DRIVER_ID, error
   _ERR_CLOSING_DRIVER,          // _ERR_CLOSING_DRIVER, MMEError
   _ERR_STOP_WAVES,              // _ERR_STOP_WAVES, error
   _ERR_SERVICE,                 // _ERR_SERVICE, error   
   _ERR_PAUSE,                   // _ERR_PAUSE, error
   _ERR_NOT_PAUSED,              // _ERR_NOT_PAUSED, 0x00
   _ERR_RESUME,                  // _ERR_RESUME, error
   _ERR_NO_SLOTS,                // _ERR_NO_SLOTS, _ERR_NO_SLOTS
   _ERR_INVALID_HANDLE,          // _ERR_INVALID_HANDLE, 0x00
   _ERR_STREAM_PLAYING,          // _ERR_STREAM_PLAYING, 0x00
   _ERR_STREAM_EMPTY,            // _ERR_STREAM_EMPTY, 0x00
   _ERR_STREAM_PAUSED,           // _ERR_STREAM_PAUSED, 0x00
   _ERR_INVALID_FORMAT,          // _ERR_INVALID_FORMAT, 0x00
   _ERR_TIME_GET_CAPS,           // _ERR_TIME_GET_CAPS, wError
   _ERR_TIME_KILL_EVENT,         // _ERR_TIME_KILL_EVENT, wError
   _ERR_TIME_END_PERIOD,         // _ERR_TIME_END_PERIOD, wError
   _ERR_TIME_BEGIN_PERIOD,       // _ERR_TIME_BEGIN_PERIOD, wError
   _ERR_DMA_BUFFER,              // _ERR_DMA_BUFFER, type
   _ERR_WAVE_OUT_PAUSE,          // _ERR_WAVE_OUT_PAUSE, wError
   _ERR_WAVE_OUT_RESTART,        // _ERR_WAVE_OUT_RESTART, wError
   _ERR_WAVE_OUT_PREPARE,        // _ERR_WAVE_OUT_PREPARE, wError
   _ERR_WAVE_OUT_START,          // _ERR_WAVE_OUT_START, wError
   _ERR_WAVE_OUT_CLOSE,          // _ERR_WAVE_OUT_CLOSE, wError
   _ERR_WAVE_OUT_RESET,          // _ERR_WAVE_OUT_RESET, wError
   _ERR_WAVE_OUT_WRITE,          // _ERR_WAVE_OUT_WRITE, wError
   _ERR_WAVE_OUT_UNPREPARE,      // _ERR_WAVE_OUT_UNPREPARE, wError
   _ERR_CLOSE_MME_DRIVER,        // _ERR_CLOSE_MME_DRIVER, hMMEDriver
   _ERR_DISABLE_MME_DRIVER,      // _ERR_DISABLE_MME_DRIVER, hMMEDriver
   _ERR_DRIVER_PATH,             // _ERR_DRIVER_PATH, wError 
   _ERR_FREE_MME_DRIVER,         // _ERR_FREE_MME_DRIVER, hMMEDriver
   _ERR_SELECTOR_FAIL,           // _ERR_SELECTOR_FAIL, wError
   _ERR_VDS_BUFFER,              // _ERR_MEMORY_FREE, _ERR_VDS_BUFFER
   _ERR_STREAM_NOT_PAUSED,       // _ERR_STREAM_NOT_PAUSED, 0x00
   _ERR_INVALID_DATA,            // _ERR_INVALID_DATA, 0x00
   _ERR_WAVE_OUT_OPEN,           // _ERR_WAVE_OUT_OPEN, wError
   _ERR_INI_FILE,                // _ERR_INI_FILE, 0x00
   _ERR_MISSING_PRODUCT_ID,      // _ERR_MISSING_PRODUCT_ID, wPid
   _ERR_INVALID_DRIVER_NAME,     // _ERR_INVALID_DRIVER_NAME, 0x00
   _ERR_MIDI_OUT_OPEN,           // _ERR_MIDI_OUT_OPEN, wError
   _ERR_MIDI_OUT_CLOSE,          // _ERR_MIDI_OUT_CLOSE, wError
   _ERR_INVALID_VERSION,         // _ERR_INVALID_VERSION, wVersion
   _ERR_INVALID_DLL              // _ERR_INVALID_DLL, 0x00
};

// _SOS_DIGI_DEVICE_CAPS.hMMEDeviceID
// WAVEOUTCAPS.hDriver
#define _SOS_WAVE_MAPPER               0xffff

// _SOS_DIGI_DEVICE_CAPS.wFormatToUse
// _SOS_DIGI_DEVICE_CAPS.wSOSAvaFormats
#define _SOS_8_BIT                     0x0001
#define _SOS_16_BIT                    0x0002
#define _SOS_MONO                      0x0004
#define _SOS_STEREO                    0x0008
#define _SOS_11025                     0x0010
#define _SOS_22050                     0x0020
#define _SOS_44100                     0x0040
#define _SOS_INVALID                   0x0080

// _SOS_DETECT.wFlags
#define _SOS_MME                       0x0001
#define _SOS_INITIALIZED               0x1000

// maximum number of samples each driver can use
#define _SOS_MAX_VOICES                0x20
  
// _SOS_DIGI_DEVICE_CAPS.wFormatToUse
// _SOS_DIGI_DEVICE_CAPS.wSOSAvaFormats
#define _SOS_8_BIT                     0x0001
#define _SOS_16_BIT                    0x0002
#define _SOS_MONO                      0x0004
#define _SOS_STEREO                    0x0008
#define _SOS_11025                     0x0010
#define _SOS_22050                     0x0020
#define _SOS_44100                     0x0040
#define _SOS_INVALID                   0x0080

// _SOS_DIGI_DEVICE_CAPS.wAdvancedFlags
#define _SOS_MME_USE_USER_SERVICE      0x0000 
#define _SOS_MME_USE_THREAD            0x0001  
#define _SOS_MME_USE_CALLBACK          0x0002

// maximum number of timer events that can be registered with the timer
// system 
#define  _TIMER_MAX_EVENTS    0x10  

// flags for the debugging system
#define  _SOS_DEBUG_NORMAL       0x0000
#define  _SOS_DEBUG_NO_TIMER     0x0001

// max number of buffers that the MME DMA system can use for buffers
#define _SOS_MME_MAX_BUFFERS           0x40

// maximum number of drivers that the system can use at any instant 
#define _SOS_MAX_DRIVERS               0x01

// error retrieval macros
#define _SOS_ERROR_MAJOR( error )  ( ( WORD )( error >> 16 ) )
#define _SOS_ERROR_MINOR( error )  ( ( WORD )error )

// _SOS_STREAM.wFlags
#define  _SOS_STREAM_FREE              0x00
#define  _SOS_STREAM_ACTIVE            0x01
#define  _SOS_STREAM_AUTO_START        0x02
#define  _SOS_STREAM_PLAYING           0x04
#define  _SOS_STREAM_PAUSED            0x08

// _SOS_SAMPLE.wSysFlags
#define  _SOS_SAMPLE_PREPARED          0x8000
#define  _SOS_SAMPLE_LOCKED            0x4000

// flag types for driver
#define  _FLAGS_SIGNED              0x8000

// flag to indicate that no window callback back is to be used
#define  _SOS_NO_WINDOW             0x00

// size of driver name to open
// _SOS_DEVICE_INFO.szDriverName[ _SOS_DRIVER_NAME_SIZE ] 
#define  _SOS_DRIVER_NAME_SIZE      0x20

// pack on a byte alignment
#pragma pack( 1 )

// new sample type
typedef  struct   _tag_sos_sample
{
   PSTR  pSample;                      // pointer to sample data
   DWORD lp32Sample;                   // 32 bit pointer to sample for WIN32S
   PSTR  pSampleCurrent;               // pointer to current location
   PSTR  pSampleLoop;                  // loop point if any

   DWORD dwLength;                     // length of sample
   DWORD dwLoopLength;                 // length of sample loop
   DWORD dwLoopEndLength;              // length of loop end
   DWORD dwLoopStage;                  // loop stage: Attack, Sustain, Release

   DWORD dwID;                         // sample ID
   DWORD dwFlags;                      // sample flags
   DWORD dwPriority;                   // priority of sample
   DWORD hSample;                      // handle to sample

   DWORD dwVolume;                     // volume
   DWORD dwLoopCount;                  // loop count

   DWORD dwRate;                       // sample rate
   DWORD dwBitsPerSample;              // sample size 8/16
   DWORD dwChannels;                   // channels per sample 1/2, mono/stereo
   DWORD dwFormat;                     // sample format: signed, unsigned, etc..

   DWORD dwPanPosition;                // pan position 0x8000 == center
   DWORD dwPanSpeed;                   // speed of auto pan
   DWORD dwPanStart;                   // start of pan 
   DWORD dwPanEnd;                     // end of pan
   DWORD dwPanMode;                    // mode: ONCE, PING-PONG, LOOP

   DWORD dwTotalBytesProcessed;        // total bytes actually played

   VOID  ( __cdecl * pfnSampleProcessed )( struct _tag_sos_sample * );  // sample complete callback
   VOID  ( __cdecl * pfnSampleDone )( struct _tag_sos_sample * );       // sample complete callback
   VOID  ( __cdecl * pfnSampleLoop )( struct _tag_sos_sample * );       // sample loop call back
   VOID  ( __cdecl * pfnSampleDoneSystem )( struct _tag_sos_sample * ); 

   DWORD dwBufferIndex;                 // buffer index that the sample finished filling into 

   DWORD dwSystem  [ 16 ];              // system data
   DWORD dwUser    [ 16 ];              // user data

   struct _tag_sos_sample *  pLink;         // link to next buffer/sample
   struct _tag_sos_sample *  pNext;         // next sample in chain

} _SOS_SAMPLE;
typedef _SOS_SAMPLE * PSOSSAMPLE;

// device capabilities structure
typedef struct
{
   // flags for a driver to use
   // _SOS_MME          0x0001 
   // _SOS_LOADABLE     0x0002
   WORD  wFlags;

   // manufacturer ID, WAVEOUTCAPS
   // MM_MICROSOFT
   WORD  wMID;       

   // product ID, WAVEOUTCAPS
   // MM_SNDBLST_WAVEOUT
   // MM_SNDBLST_WAVEIN
   WORD  wPID;        

   // version of MME driver, major/minor respectively, WAVEOUTCAPS
   WORD  wVersion;   

   // name of the device given by the device manufacturer, WAVEOUTCAPS
   BYTE  szName[ MAXPNAMELEN ]; 

   // name of driver to open 
   BYTE  szDriverName[ _SOS_DRIVER_NAME_SIZE ];

   // MME format driver abilities
   // WAVE_FORMAT_1M08
   // WAVE_FORMAT_1S08
   // WAVE_FORMAT_1M16
   // WAVE_FORMAT_1S16
   // WAVE_FORMAT_2M08
   // WAVE_FORMAT_2S08
   // WAVE_FORMAT_2M16
   // WAVE_FORMAT_2S16
   // WAVE_FORMAT_4M08
   // WAVE_FORMAT_4S08
   // WAVE_FORMAT_4M16
   // WAVE_FORMAT_4S16
   DWORD dwMMEAvaFormats;

   // SOS format driver abilities
   // _SOS_8_BIT     0x01
   // _SOS_16_BIT    0x02
   // _SOS_MONO      0x04
   // _SOS_STEREO    0x08
   // _SOS_11025     0x10
   // _SOS_22050     0x20
   // _SOS_44100     0x40
   // _SOS_INVALID   0x80
   WORD  wSOSAvaFormats;

   // format for the system to try to initialize the driver as
   // _SOS_8_BIT     0x01
   // _SOS_16_BIT    0x02
   // _SOS_MONO      0x04
   // _SOS_STEREO    0x08
   // _SOS_11025     0x10
   // _SOS_22050     0x20
   // _SOS_44100     0x40
   // _SOS_INVALID   0x80
   WORD  wFormatToUse;

   // advanced features that the MME device supports
   // WAVECAPS_PITCH          pitch shifting         
   // WAVECAPS_PLAYBACKRATE   playback rate changing
   // WAVECAPS_VOLUME         volume control
   // WAVECAPS_LRVOLUME       separate left and right volume control
   DWORD dwAdvanced;

   // device ID for the MME driver
   // 0 - # of wave drivers
   // 0xffff _MME_WAVE_MAPPER
   // 0x8000 _SOS_INVALID_DRIVER
   HSOS hMMEDeviceID;

   // handle of the window to send all driver messages to
   // _SOS_NO_WINDOW
   HWND  hWnd;

   // handle of the main application window to allow direct sound to track
   // the application
   HWND  hDirectSoundWnd;

   // port for the SOS driver to use
   WORD  wPort;

   // irq for the SOS driver to use
   WORD  wIRQ;

   // dma channel for the sos driver to use
   WORD  wDMA;

   // extra parameter for the device to use
   WORD  wParam;

   // base SOS driver ID
   WORD  wSOSDriverID;

   // rate to register the fill handler at
   // default: 120
   WORD wMixerRate;

   // total buffers to use if a MME device is initialized
   WORD  wBuffers;

   // total number of buffers to skip if a MME device is initialized
   WORD  wBufferSkip;

   // size of the dma buffers to use for the MME system
   WORD  wBufferSize;

   // flags field for advanced flags
   // _SOS_MME_USE_THREAD
   WORD  wAdvancedFlags;

} _SOS_DIGI_DEVICE_CAPS;
typedef _SOS_DIGI_DEVICE_CAPS * PSOSDIGIDEVICECAPS;

// stream initialization structure
typedef struct
{
   // flags for the streaming system
   // _SOS_STREAM_AUTO_START
   WORD  wFlags;

   // structure template for the type of sample to play
   _SOS_SAMPLE sSample;

   // handle of the stream
   HSOS hStream;   

   // pointer to the buffer done callback, do not use under WIN32S 
   VOID ( __cdecl * pfnBufferDone )( HSOS, HSOS, struct _tagSOSStreamElement *  );  

   // pointer to the stream done callback, do not use under WIN32S
   VOID ( __cdecl * pfnStreamDone )( HSOS, HSOS );

   // handle of the window to send stream messages to
   HWND  hWnd;

} _SOS_STREAM_INIT;
typedef _SOS_STREAM_INIT * PSOSSTREAMINIT;
 
// stream element 
typedef struct _tagSOSStreamElement
{
   // flags
   WORD  wFlags;

   // pointer to the buffer
   PSTR pData;

   // temporary variable for 32 to 16 bit conversions
   DWORD lp32Data;

   // temporary variable for 32 to 16 bit conversions
   DWORD lp32Element;

   // size of the buffer
   DWORD dwSize;

   // number of buffers remaining
   WORD  wBuffers;

   // stream handle
   HSOS hStream;

   // driver handle
   HSOS hDriver;

   // sample handle
   HSOS hSample;

   // pointer to the next stream element
   struct _tagSOSStreamElement * psNext;

} _SOS_STREAM_ELEMENT;
typedef _SOS_STREAM_ELEMENT * PSOSSTREAMELEMENT;

// structure for streaming system to handle streams
typedef struct 
{
   // flags
   // _SOS_STREAM_ACTIVE
   // _SOS_STREAM_PLAYING
   // _SOS_STREAM_PAUSED
   WORD              wFlags;     

   // handle for the sample returned by
   // sosDIGIStartSample
   HSOS            hSample;  
  
   // handle for the driver to play the stream on
   HSOS            hDriver;
   
   // total number of buffers left in                           
   // the stream
   WORD              wBuffers;   

   // handle of window to send callback                                  
   // messages to 
   HWND              hWnd;       

   // start sample structure to access all of the streams sample information
   // from
   _SOS_SAMPLE sSample;

   // far pointer to first
   // structure in the stream list
   PSOSSTREAMELEMENT  psFirst;    

   // far pointer to last 
   // structure in the stream list
   PSOSSTREAMELEMENT  psLast;     

   // pointer to the buffer done callback
   VOID ( * pfnBufferDone )( HSOS, HSOS, struct _tagSOSStreamElement *  );  

   // pointer to the stream done callback
   VOID ( * pfnStreamDone )( HSOS, HSOS );

} _SOS_STREAM;
typedef _SOS_STREAM * PSOSSTREAM;

#pragma pack()

// SOSLIB*.DLL includes so that the modules that include this file
// will have the other SOS includes files automatically  
#include "sosfnct.h"

#endif

