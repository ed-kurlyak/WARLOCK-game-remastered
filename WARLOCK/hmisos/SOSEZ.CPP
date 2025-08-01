/****************************************************************************

   File              :  sosez.c

   Purpose           :  Module for EZ SOS functions

   Date              :  3-2-95 

   Programmer(s)     :  Don Fowler

   Last Updated      :  3-17-95

****************************************************************************
               Copyright(c) 1992,1995 Human Machine Interfaces 
                            All Rights Reserved
****************************************************************************/

#include <stdlib.h>
#include <windows.h>
#ifdef __BORLANDC__
#else
#include <lmaccess.h>
#endif
#include "sosm.h"
#include "sos.h"
#include "sosez.h"

// rate to call the fill handler at
#define  _MIXER_RATE   120

// sample rate to use
#define  _SAMPLE_RATE         _SOS_11025

// sample size to use 
#define  _SAMPLE_SIZE         _SOS_8_BIT

// channels to use            
#define  _CHANNELS            _SOS_STEREO
            
// the total buffer size used by the digital system is 
// the buffer size * max buffers, for the MME drivers
// the buffer size is the size of buffers to send to the MME
// driver and max buffers is the number of these smaller 
// buffers to use
#define  _BUFFER_SIZE         512   // the maximum buffer size that SOSDLL
                                    // can handle is 16K 
#define  _MAX_BUFFERS         64    // the maximum number of buffers SOSDLL
                                    // can handle is 64

// path to SOSDLL driver files
#define  _DRIVER_PATH         NULL

// number of buffers to skip when using an MME driver
#define  _BUFFER_SKIP         24

// capabilities for the digital system
static _SOS_DIGI_DEVICE_CAPS sDIGICaps;

// capabilities for the midi system
static _SOS_MIDI_DEVICE_CAPS sMIDICaps;

// handle of the digital driver
static HSOS             hDIGIDriver;

// handle of the MIDI driver
static HSOS             hMIDIDriver;

// internal SOS error message
static DWORD             dwError = _ERR_NO_ERROR;

// flag to indicate that a digital driver has been initialized
static WORD              wDIGIDriverInit = FALSE;  

// flag to indicate that a MIDI driver has been initialized
static WORD              wMIDIDriverInit = FALSE;  

// flags to indicate whether or not a system was requested to be initialized
static WORD              wInitDIGI       = FALSE;
static WORD              wInitMIDI       = FALSE; 

// handle of application window
static HWND              hAppWnd;

// near pointers to the instrument files for the OPL2 and OPL3 drivers
static PSTR              pMelodic;
static PSTR              pDrums;

// 16 bit far pointers to the instrument files for the OPL2 and OPL3 drivers
static DWORD             lp32Melodic;
static DWORD             lp32Drums;

/****************************************************************************

   Function       : sosEZInitSystem
            
   Parameter(s)   : WORD wDIGISystem
                        _SOS_NATIVE
                        _SOS_MME   
                    WORD wMIDISystem
                        _SOS_NATIVE
                        _SOS_MME
                        _SOS_AUTO
                    HWND hWnd
                        Handle of window to send all messages to.
                    HWND hDirectSoundWnd
                        Handle of window to allow direct sound to track
                        the application

   Actions        : Initialize the EZ system

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZInitSystem( WORD wDIGISystem, WORD wMIDISystem, HWND hWnd,
   HWND hDirectSoundWnd )
{
   // store the state of the digi initialization flag
   if( wDIGISystem == ( WORD )-1 )
      wInitDIGI = FALSE;
   else
      wInitDIGI = TRUE;

   // store the state of the midi initialization flag
   if( wMIDISystem == ( WORD )-1 )
      wInitMIDI = FALSE;
   else
      wInitMIDI = TRUE;

   // verify that the MIDI system is to be initialized, if so
   // then autodetect the MIDI device before the DIGITAL system
   // has a chance to shut it down
   if( !wMIDIDriverInit && wInitMIDI )
   {
      // initialize the detection system
      if( ( dwError = sosMIDIDetectInit( wMIDISystem, NULL ) ) )
         return( TRUE );

      // find the first driver
      if( ( dwError = sosMIDIDetectFindFirst( &sMIDICaps ) ) )
      {
         // uninitialize the detection system
         sosMIDIDetectUnInit();

         // return an error
         return( TRUE );
      }

      // free the detection system
      if( ( dwError = sosMIDIDetectUnInit() ) )
         return( TRUE );
   }

   // verify that the digital driver has not been initialized
   if( !wDIGIDriverInit && wInitDIGI )
   {
      // initialize the detection system
      if( ( dwError = sosDIGIDetectInit( wDIGISystem, NULL ) ) )
         return( TRUE );

      // find the first driver
      if( ( dwError = sosDIGIDetectFindFirst( &sDIGICaps ) ) )
      {
         // uninitialize the detection system
         sosDIGIDetectUnInit();

         // return an error
         return( TRUE );
      }

      // free the detection system
      if( ( dwError = sosDIGIDetectUnInit() ) )
         return( TRUE );

      // initialize the digital system
      sosDIGIInitSystem( _SOS_DEBUG_NORMAL, _DRIVER_PATH );

      // initialize the timer system, and return any error code if
      // it is not from being initialized while still initialized
      if( ( dwError = sosTIMERInitSystem( 0x00 ) ) )
         if( HIWORD( dwError ) != _ERR_SYSTEM_INITIALIZED )
            return( TRUE );    

      // store the NATIVE/MME system in the capabilities
      sDIGICaps.wFlags = wDIGISystem;
      sDIGICaps.wAdvancedFlags = _SOS_MME_USE_THREAD;
 
      // verify that the digital format required is available
      if( ( sDIGICaps.wSOSAvaFormats & ( _SAMPLE_SIZE | _CHANNELS |
         _SAMPLE_RATE ) ) != ( _SAMPLE_SIZE | _CHANNELS | _SAMPLE_RATE ) )    
      {
         // set the error code
         dwError = _ME( _ERR_INVALID_FORMAT, sDIGICaps.wSOSAvaFormats );

         // return invalid format
         return( dwError );
      }

      // set the format to use to the specified parameters
      sDIGICaps.wFormatToUse = _SAMPLE_SIZE | _CHANNELS | _SAMPLE_RATE;

      // set the callback window to the program main window
      sDIGICaps.hWnd = hWnd;

      // set the main application window to allow direct sound to track the
      // application
      sDIGICaps.hDirectSoundWnd = hDirectSoundWnd;

      // set the fill handler rate to 120, this is the most common rate
      sDIGICaps.wMixerRate = _MIXER_RATE;

      // set the individual buffer size to 512 bytes, and the number of
      // small buffers to 64, this will provide a DMA buffer of 32K.
      sDIGICaps.wBufferSize    = _BUFFER_SIZE;
      sDIGICaps.wBuffers       = _MAX_BUFFERS;

      // set the number of buffers to skip before filling to 8, this 
      // is the minimum skip on a SOUND BLASTER MME driver 
      sDIGICaps.wBufferSkip = _BUFFER_SKIP;

      // initialize the driver
      if( ( dwError = sosDIGIInitDriver( &sDIGICaps, &hDIGIDriver ) ) )
      {
         // reset the driver initialized flag
         wDIGIDriverInit = FALSE; 

         // return the error
         return( TRUE );
      }
      else
      {
         // store the application window handle
         hAppWnd = hWnd;

         // set the active window for the digital system to send messages
         sosDIGISetActiveWindow( hDIGIDriver, hAppWnd );

         // set the driver initialized flag
         wDIGIDriverInit = TRUE; 
      }
   }

   // verify that the MIDI system is to be initialized
   if( !wMIDIDriverInit && wInitMIDI )
   {
      // check to see if the digital system is active
      if( !wInitDIGI )
      {
         // initialize the timer system, and return any error code if
         // it is not from being initialized while still initialized
         if( ( dwError = sosTIMERInitSystem( 0x00 ) ) )
            if( HIWORD( dwError ) != _ERR_SYSTEM_INITIALIZED )
               return( TRUE );    
      }

      // initialize the MIDI system
      sosMIDIInitSystem( _SOS_DEBUG_NORMAL, _DRIVER_PATH );

      // store the NATIVE/MME system in the capabilities
      sMIDICaps.wFlags = wMIDISystem;
 
      // initialize the driver
      if( ( dwError = sosMIDIInitDriver( &sMIDICaps, &hMIDIDriver ) ) )
      {
         // reset the driver initialized flag
         wMIDIDriverInit = FALSE; 

         // return the error
         return( TRUE );
      }
      else
      {
         // check to see if the instrument files for the OPL2, OPL3 drivers
         // are neccessary
         if( ( sMIDICaps.wSOSDriverID == _MIDI_OPL2 ||
            sMIDICaps.wSOSDriverID == _MIDI_OPL3 ) && 
            sMIDICaps.wFlags & _SOS_NATIVE ) 
         {
            // load the instrument files
            sosEZLoadFile( "MELODIC.BNK", &pMelodic, &lp32Melodic );
            sosEZLoadFile( "DRUM.BNK", &pDrums, &lp32Drums );

            // set the instrument data
            sosMIDISetInsData( hMIDIDriver, ( PSTR )lp32Melodic, 0x00 );
            sosMIDISetInsData( hMIDIDriver, ( PSTR )lp32Drums,   0x01 );
         }

         // store the application window
         hAppWnd = hWnd;

         // set the driver initialized flag
         wMIDIDriverInit = TRUE; 
      }
   }

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZUnInitSystem
            
   Parameter(s)   : VOID

   Actions        : Uninitialize the EZ system

   Returns        : BOOL 

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZUnInitSystem( VOID )
{
   // verify that the driver has been initialized
   if( wDIGIDriverInit )
   {
      // reset the driver initialized flag
      wDIGIDriverInit = FALSE; 

      // stop all of the samples on the driver
      sosDIGIStopAllSamples( hDIGIDriver );

      // set the active window for the digital system
      sosDIGISetActiveWindow( hDIGIDriver, _SOS_NO_WINDOW );

      // uninitiailize the driver
      if( ( dwError = sosDIGIUnInitDriver( hDIGIDriver ) ) )
         return( TRUE );

      // uninitialize the timer system
      sosTIMERUnInitSystem();

      // uninitialize the digital system
      sosDIGIUnInitSystem();
   }

   // verify that the driver has been initialized
   if( wMIDIDriverInit )
   {
      // reset the driver initialized flag
      wMIDIDriverInit = FALSE; 

      // stop all of the songs
      sosMIDIStopAllSongs();

      // set the active window for the digital system
      sosMIDISetActiveWindow( hMIDIDriver, _SOS_NO_WINDOW );

      // reset the MIDI driver
      sosMIDIResetDriver( hMIDIDriver );

      // check to see if the instrment files for the OPL2, OPL3 drivers
      // were loaded
      if( ( sMIDICaps.wSOSDriverID == _MIDI_OPL2 ||
         sMIDICaps.wSOSDriverID == _MIDI_OPL3 ) && 
         sMIDICaps.wFlags & _SOS_NATIVE ) 
      {
         // unload the instrument files
         sosEZUnLoadFile( pDrums, lp32Drums );
         sosEZUnLoadFile( pMelodic, lp32Melodic );
      }

      // uninitiailize the driver
      if( ( dwError = sosMIDIUnInitDriver( hMIDIDriver ) ) )
         return( TRUE );

      // check to see if digital is initialized
      if( !wInitDIGI )
         sosTIMERUnInitSystem();

      // uninitialize the midi system
      sosMIDIUnInitSystem();
   }

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZDisplayError
            
   Parameter(s)   : HWND hWnd
                     Handle of window to display error over

   Actions        : Displays the current error 

   Returns        : VOID

   Programmer(s)  : Don Fowler

****************************************************************************/

VOID sosEZDisplayError( HWND hWnd )
{
   // display the error
   MessageBox( hWnd, sosGetErrorString( dwError ),
      NULL, MB_OK | MB_ICONEXCLAMATION ); 
}

/****************************************************************************

   Function       : sosEZGetError
            
   Parameter(s)   : VOID

   Actions        : Returns the current EZ error

   Returns        : DWORD

   Programmer(s)  : Don Fowler

****************************************************************************/

DWORD sosEZGetError( VOID )
{
   // return the current error
   return( dwError );
}

/****************************************************************************

   Function       : sosEZDriverActive
            
   Parameter(s)   : VOID

   Actions        : Returns the active state of the digital driver

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZDriverActive( VOID )
{
   // return the active state of the drivers
   return( ( wDIGIDriverInit & wInitDIGI ) | ( wMIDIDriverInit & wInitMIDI ) );
}

/****************************************************************************

   Function       : sosEZGetDIGIHandle
            
   Parameter(s)   : VOID

   Actions        : Return the driver handle

   Returns        : HSOS

   Programmer(s)  : Don Fowler

****************************************************************************/

HSOS sosEZGetDIGIHandle( VOID )
{
   // return the driver handle
   return( hDIGIDriver );
}

/****************************************************************************

   Function       : sosEZGetMIDIHandle
            
   Parameter(s)   : VOID

   Actions        : Return the driver handle

   Returns        : HSOS

   Programmer(s)  : Don Fowler

****************************************************************************/

HSOS sosEZGetMIDIHandle( VOID )
{
   // return the driver handle
   return( hMIDIDriver );
}

/****************************************************************************

   Function       : sosEZLoadSample
            
   Parameter(s)   : PSTR pzName
                     Near pointer to the name of the sample
                    PSOSSAMPLE * pSample
                     Near pointer to address of pointer to sample structure

   Actions        : Loads a sample into memory with the start sample structure

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZLoadSample( PSTR pzName, PSOSSAMPLE * psSample )
{
   DWORD dwSize;
   HFILE hFile;
   HGLOBAL hMem;     
   PSTR pData;
   _WAVHEADER * pWaveHeader;
      
   // attempt to open the file 
   if( ( hFile = _lopen( pzName, OF_READ ) ) != HFILE_ERROR )
   {
      // seek to the end of the file to get the size, then seek back 
      // to the begining    
      dwSize = _llseek( hFile, 0L, SEEK_END );
      _llseek( hFile, 0L, SEEK_SET );
 
      // allocate the memory for the file, _SOS_SAMPLE, and the
      // memory handle. The structure is _SOS_SAMPLE then the
      // handle for the memory then the sample data
      if( ! ( hMem = GlobalAlloc( GPTR, dwSize +
         sizeof( _SOS_SAMPLE ) + sizeof( HGLOBAL ) ) ) )
         return( TRUE );      

      // lock the sample memory to get a pointer to it
      pData = ( PSTR )GlobalLock( hMem );

      // load the sample into memory
      _hread( hFile, pData + sizeof( _SOS_SAMPLE ) + sizeof( HGLOBAL ),
         dwSize );

      // close the file
      _lclose( hFile );

      // clear out the _SOS_SAMPLE structure
      memset( pData, 0x00, sizeof( _SOS_SAMPLE ) );

      // store the memory handle after the start sample structure
      *( HGLOBAL * )( pData + sizeof( _SOS_SAMPLE ) ) = hMem;

      // check to see if this is a .WAV file, if so then grab the settings
      // out of the file
      if( !strncmp( pData + sizeof( _SOS_SAMPLE ) + sizeof( HGLOBAL ),
         "RIFF", 0x04 ) )
      {
         // setup a pointer to the wave header
         pWaveHeader = ( _WAVHEADER * )( pData + sizeof( _SOS_SAMPLE )
            + sizeof( HGLOBAL ) );

         // store the size of the sample
         ( ( PSOSSAMPLE )pData )->dwLength = pWaveHeader->dwDataLength - 
            sizeof( _WAVHEADER );

         // store the bit size of the sample
         ( ( PSOSSAMPLE )pData )->dwBitsPerSample = pWaveHeader->wBitsPerSample;

         // store the channels for the sample
         ( ( PSOSSAMPLE )pData )->dwChannels = pWaveHeader->wChannels;

         // set the sample format
         if( pWaveHeader->wBitsPerSample == 0x08 )
            ( ( PSOSSAMPLE )pData )->dwFormat = _PCM_UNSIGNED;
         else
            ( ( PSOSSAMPLE )pData )->dwFormat = 0x00;
            
         // set the sample rate
         ( ( PSOSSAMPLE )pData )->dwRate = pWaveHeader->dwSamplesPerSec;

         // setup a pointer to the sample
         ( ( PSOSSAMPLE )pData )->pSample = pData + 
            sizeof( _SOS_SAMPLE ) + sizeof( HGLOBAL ) + sizeof( _WAVHEADER );
      }
      else
      {
         // store the size of the sample
         ( ( PSOSSAMPLE )pData )->dwLength = dwSize;

         // store the bit size of the sample
         ( ( PSOSSAMPLE )pData )->dwBitsPerSample = 0x08;

         // set the sample format
         ( ( PSOSSAMPLE )pData )->dwFormat = _PCM_UNSIGNED;

         // store the channels for the sample
         ( ( PSOSSAMPLE )pData )->dwChannels = 0x01;

         // set the sample rate
         ( ( PSOSSAMPLE )pData )->dwRate = 11025;

         // setup a pointer to the sample
         ( ( PSOSSAMPLE )pData )->pSample = pData + 
            sizeof( _SOS_SAMPLE ) + sizeof( HGLOBAL );
      }

      // store the pan position of the sample
      ( ( PSOSSAMPLE )pData )->dwPanPosition = _PAN_CENTER;

      // store the volume to play the sample at
      ( ( PSOSSAMPLE )pData )->dwVolume = MK_VOLUME( 0x7fff, 0x7fff );

      // prepare the memory for use with the 16 bit dll
      sosPrepare32Memory( ( ( PSOSSAMPLE )pData )->pSample, dwSize,
         &( ( PSOSSAMPLE )pData )->lp32Sample );

      // return the pointer to the start sample structure
      *psSample = ( PSOSSAMPLE )pData;
   }
   else
      return( TRUE );

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZUnLoadSample
            
   Parameter(s)   : PSTR psSample
                     Near pointer to sample data

   Actions        : Free a previously allocated sample

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZUnLoadSample( PSOSSAMPLE psSample )
{
   // verify that the sample memory is valid
   if( psSample )
   {
      // unprepare the memory from use with the 16 bit dll
      sosUnPrepare32Memory( ( ( PSOSSAMPLE )psSample )->lp32Sample );

      // unlock the sample memory
      GlobalUnlock( *( HGLOBAL * )( ( LPSTR )psSample + 
         sizeof( _SOS_SAMPLE ) ) );
  
      // free the sample memory
      GlobalFree( *( HGLOBAL * )( ( PSTR )psSample + 
         sizeof( _SOS_SAMPLE ) ) );
   }

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZLoadFile
            
   Parameter(s)   : PSTR pzName
                     Near pointer to the name of the file
                    PSTR * pSample
                     Near pointer to address of pointer to file memory
                    DWORD *lp32Data
                     Near pointer to 32 bit far pointer for 16 bit dll

   Actions        : Loads a file into memory

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZLoadFile( PSTR pzName, PSTR * psData, DWORD *lp32Data )
{
   DWORD dwSize;
   HFILE hFile;
   HGLOBAL hMem;     
   PSTR pData;
      
   // attempt to open the file 
   if( ( hFile = _lopen( pzName, OF_READ ) ) != HFILE_ERROR )
   {
      // seek to the end of the file to get the size, then seek back 
      // to the begining    
      dwSize = _llseek( hFile, 0L, SEEK_END );
      _llseek( hFile, 0L, SEEK_SET );
 
      // allocate the memory for the file, dwSize
      if( !( hMem = GlobalAlloc( GPTR, dwSize ) ) )
         return( TRUE );      

      // lock the memory to get a  pointer to it
      pData = ( PSTR )GlobalLock( hMem );

      // load the file into memory
      _hread( hFile, pData, dwSize );

      // close the file
      _lclose( hFile );

      // prepare the memory for use with the 16 bit dll
      sosPrepare32Memory( pData, dwSize, lp32Data );

      // return the pointer to the file memory
      *psData = ( PSTR )pData;
   }
   else
      return( TRUE );

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZUnLoadFile
            
   Parameter(s)   : PSTR psData
                     Near pointer to file data
                    DWORD lp32Data
                     Far pointer to memory for 16 bit dll

   Actions        : Free previously allocated file memory

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZUnLoadFile( PSTR psData, DWORD lp32Data )
{
   // unprepare the memory from use with the 16 bit dll 
   sosUnPrepare32Memory( lp32Data );

   // unlock and free the file memory, if the memory is valid
   if( psData )
      GlobalFreePtr( psData );

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZLoadSong
            
   Parameter(s)   : PSTR pzName
                     Near pointer to the name of the song
                    PSOSINITSONG * pSong
                     Near pointer to address of pointer to init song structure

   Actions        : Loads a song into memory with the init song structure

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZLoadSong( PSTR pzName, PSOSMIDISONG * psSong )
{
   DWORD dwSize;
   HFILE hFile;
   HGLOBAL hMem;     
   PSTR pData;
      
   // attempt to open the file 
   if( ( hFile = _lopen( pzName, OF_READ ) ) != HFILE_ERROR )
   {
      // seek to the end of the file to get the size, then seek back 
      // to the begining    
      dwSize = _llseek( hFile, 0L, SEEK_END );
      _llseek( hFile, 0L, SEEK_SET );
 
      // allocate the memory for the file, _SOS_MIDI_SONG, and the
      // memory handle. The structure is _SOS_MIDI_SONG then the
      // handle for the memory then the sample data
      if( ! ( hMem = GlobalAlloc( GPTR, dwSize +
         sizeof( _SOS_MIDI_SONG ) + sizeof( HGLOBAL ) + 0x10 ) ) )
         return( TRUE );      

      // lock the song memory to get a  pointer to it
      pData = ( PSTR )GlobalLock( hMem );

      // load the song into memory
      _hread( hFile, pData + sizeof( _SOS_MIDI_SONG ) + sizeof( HGLOBAL ),
         dwSize );

      // close the file
      _lclose( hFile );

      // clear out the _SOS_MIDI_SONG structure
      _fmemset( pData, 0x00, sizeof( _SOS_MIDI_SONG ) );

      // store the memory handle after the init song structure
      *( HGLOBAL  * )( pData + sizeof( _SOS_MIDI_SONG ) ) = hMem;

      // setup a pointer to the song data
      ( ( PSOSMIDISONG )pData )->pSong = ( BYTE * )pData + 
         sizeof( _SOS_MIDI_SONG ) + sizeof( HGLOBAL );

      // prepare the memory for use with the 16 bit dll
      sosPrepare32Memory( ( PSTR )( ( PSOSMIDISONG )pData )->pSong, dwSize,
         &( ( PSOSMIDISONG )pData )->lp32Data );

      // prepare the INIT_SONG_STRUCTURE for use with the 16 bit dll
      sosPrepare32Memory( ( PSTR )pData, sizeof( _SOS_MIDI_SONG ),  
         &( ( PSOSMIDISONG )pData )->lp32InitSong );

      // return the pointer to the _SOS_MIDI_SONG structure
      *psSong = ( PSOSMIDISONG )pData;
   }
   else
      return( TRUE );

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZUnLoadSong
            
   Parameter(s)   : PSTR psSong
                     Near pointer to song data

   Actions        : Free a previously allocated song

   Returns        : BOOL

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZUnLoadSong( PSOSMIDISONG psSong )
{
   // verify that the song memory is valid
   if( psSong )
   {
      // unprepare the memory from use with the 16 bit dll
      sosUnPrepare32Memory( psSong->lp32Data );

      // unprepare the init song structure from use with the 16 bit dll 
      sosUnPrepare32Memory( psSong->lp32InitSong );

      // unlock the song memory
      GlobalUnlock( *( HGLOBAL * )( ( PSTR )psSong + 
         sizeof( _SOS_MIDI_SONG ) ) );
  
      // free the song memory
      GlobalFree( *( HGLOBAL * )( ( PSTR )psSong + 
         sizeof( _SOS_MIDI_SONG ) ) );
   }

   // return no error
   return( FALSE );
}


// STREAMING SYSTEM
//
// maximum number of streams that can be active
#define _MAX_STREAMS           0x20

// number of stream buffers to use
#define _MAX_STREAM_BUFFERS    0x08

// size of each stream buffer
#define _STREAM_BUFFER_SIZE    0x8000

// handles for the stream memory
static HGLOBAL hMemory[ _MAX_STREAMS ][ _MAX_STREAM_BUFFERS ];

// handle for the stream files
static HFILE hFile[ _MAX_STREAMS ]; 

// stream initialization structure
static _SOS_STREAM_INIT sStreamInit;
 
// stream element structures for all of the stream buffers
static _SOS_STREAM_ELEMENT sStreamBuffers[ _MAX_STREAMS ][ _MAX_STREAM_BUFFERS ];

/****************************************************************************

   Function       : sosEZStreamFile
            
   Parameter(s)   : PSTR szName  
                     Pointer to the file name to stream 
                    HWND hWnd
                     Handle of window to send stream messages to
                    HSOS * pStream
                     Near pointer to the return for the stream handle

   Actions        : Initialize all of the primary buffers for the stream

   Returns        : BOOL 

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZStreamFile( PSTR szName, HWND hWnd, PHSOS pStream )
{
   WORD  wIndex;
   //WORD  wError;
   HSOS hStream;

   // attempt to open a stream to use
   sStreamInit.wFlags = 0x00;

   // set the window function to send the streaming messages to 
   sStreamInit.hWnd = hWnd;

   // setup the start sample structure that is to be used for 
   // the stream sample template
   sStreamInit.sSample.dwFlags = 0x00;
   sStreamInit.sSample.dwPanPosition = _PAN_CENTER;
   sStreamInit.sSample.dwBitsPerSample = 0x08;
   sStreamInit.sSample.dwChannels = 0x01;
   sStreamInit.sSample.dwVolume = MK_VOLUME( 0x7fff, 0x7fff );
   sStreamInit.sSample.dwFormat = _PCM_UNSIGNED;
   sStreamInit.sSample.dwRate = 11025;

   // attempt to acquire a stream to use
   if( sosDIGIStreamAcquire( hDIGIDriver, &sStreamInit, &hStream ) )
      return( TRUE );

   // open the file to stream from disk
   if( ( hFile[ hStream ] = _lopen( szName, OF_READ ) ) == HFILE_ERROR )
      return( TRUE ); 

   // initialize all of the stream buffers
   for( wIndex = 0x00; wIndex < _MAX_STREAM_BUFFERS; wIndex++ )
   {
      // allocate the memory for the stream element
      if( ! ( hMemory[ hStream ][ wIndex ] = GlobalAlloc( GPTR,
         _STREAM_BUFFER_SIZE ) ) )
         return( TRUE );      

      // lock the sample memory
      ( PSTR )sStreamBuffers[ hStream ][ wIndex ].pData = ( PSTR )GlobalLock( 
         hMemory[ hStream ][ wIndex ] );

      // load the sample into memory
      sStreamBuffers[ hStream ][ wIndex ].dwSize = _hread( hFile[ hStream ],
         sStreamBuffers[ hStream ][ wIndex ].pData, _STREAM_BUFFER_SIZE );
   }

   // loop through all of the samples, sending them to the streaming system
   for( wIndex = 0x00; wIndex < _MAX_STREAM_BUFFERS; wIndex++ )
   {
      // verify that there is data to send to the streaming system
      if( sStreamBuffers[ hStream ][ wIndex ].dwSize )
      {
         // prepare the memory for use with the 16 bit dll
         sosPrepare32Memory( sStreamBuffers[ hStream ][ wIndex ].pData, 
            sStreamBuffers[ hStream ][ wIndex ].dwSize,
            &sStreamBuffers[ hStream ][ wIndex ].lp32Data );

         // prepare the memory for use with the 16 bit dll
         sosPrepare32Memory( ( PSTR )&sStreamBuffers[ hStream ][ wIndex ], 
            sizeof( _SOS_STREAM_ELEMENT ),
            &sStreamBuffers[ hStream ][ wIndex ].lp32Element );

         // prepare the buffer for the streaming system
         sosDIGIStreamPrepareBuffer( hDIGIDriver, hStream, 
            &sStreamBuffers[ hStream ][ wIndex ] );

         // add the buffer to the stream
         sosDIGIStreamAddBuffer( hDIGIDriver, hStream, 
            &sStreamBuffers[ hStream ][ wIndex ] );
      }
   }

   // start the stream playing
   sosDIGIStreamStart( hDIGIDriver, hStream );
   
   // return the stream handle
   *pStream = hStream;

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZFillStreamBuffer
            
   Parameter(s)   : HSOS hStream  
                     Handle for the stream to affect
                    PSOSSTREAMELEMENT
                     Near pointer to the stream element to use

   Actions        : Fill a buffer with data for the streaming system to use

   Returns        : BOOL 

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZFillStreamBuffer( HSOS hStream, PSOSSTREAMELEMENT psStream )
{
   //WORD  wIndex;
   //DWORD dwError;

   // load the sample memory into the stream element memory
   psStream->dwSize = _hread( hFile[ hStream ], ( PSTR )psStream->pData,
      _STREAM_BUFFER_SIZE );

   // add the buffer to the streaming system if there is any data for 
   // the element
   if( psStream->dwSize ) 
      sosDIGIStreamAddBuffer( hDIGIDriver, hStream, psStream );

   // return no error 
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZStopStream
            
   Parameter(s)   : HSOS hStream
                     Handle of the stream to stop

   Actions        : DeInitialize all of the streaming buffers

   Returns        : BOOL 

   Programmer(s)  : Don Fowler

****************************************************************************/

BOOL sosEZStopStream( HSOS hStream )
{
   WORD  wIndex;

   // stop the stream
   sosDIGIStreamStop( hDIGIDriver, hStream );

   // release the stream
   sosDIGIStreamRelease( hDIGIDriver, hStream );

   // close the file
   _lclose( hFile[ hStream ] );

   // loop through all of the samples
   for( wIndex = 0x00; wIndex < _MAX_STREAM_BUFFERS; wIndex++ )
   {
      // unprepare the sample
      sosDIGIStreamUnPrepareBuffer( hDIGIDriver, hStream, 
         &sStreamBuffers[ hStream ][ wIndex ] );

      // unprepare the memory for use with the 16 bit dll
      sosUnPrepare32Memory( sStreamBuffers[ hStream ][ wIndex ].lp32Element ); 
      sosUnPrepare32Memory( sStreamBuffers[ hStream ][ wIndex ].lp32Data ); 

      // unlock the memory
      GlobalUnlock( hMemory[ hStream ][ wIndex ] );
 
      // free the memory
      GlobalFree( hMemory[ hStream ][ wIndex ] );
   }

   // return no error
   return( FALSE );
}

/****************************************************************************

   Function       : sosEZHandleMessages
            
   Parameter(s)   : 

   Actions        : Handle all of the messages neccesary for the EZ system 

   Returns        : BOOL 

   Programmer(s)  : Don Fowler

****************************************************************************/

UINT sosEZHandleMessages( HWND hDlg, UINT wMsg, UINT wParam, LONG lParam )
{
   switch( wMsg )
   {
      case WM_INITDIALOG:

         // begin timer messages being sent to application window
         // these message are neccessary so that the SOS can send
         // MME waveOutWrite messages in the foreground and keep
         // the system running, the secondary use is to allow for
         // retrying initiailization of the EZ system if the MME driver
         // is busy the first few times initialization is tried.
         SetTimer( hDlg, 1, 1, NULL );

         break;

      case WM_DESTROY:

         // stop timer messages from being sent to the window
         KillTimer( hDlg, 1 );

         break;
   
      case WM_TIMER:

            // call the digital system to allow processing of 
            // waveOutWrite functions
            if( sosEZDriverActive() )
               sosDIGIUserService( hDIGIDriver );

         break;

      case _SOS_COMMAND:
         switch( LOWORD( wParam ) )
         {
            case _SOS_STREAM_DONE:

               // release all of the buffers associated to the stream
               sosEZStopStream( ( HSOS )LOWORD( lParam ) );

               break;

            case _SOS_STREAM_BUFFER_DONE:
                  
               // call the stream system to fill and send the next buffer
               sosEZFillStreamBuffer( ( ( PSOSSTREAMELEMENT )lParam )
                  ->hStream, ( PSOSSTREAMELEMENT )lParam );

               break;

            case _SOS_SAMPLE_CALLBACK:
               break;
         }
         break;
   }

   // return no error
   return( FALSE );
}

