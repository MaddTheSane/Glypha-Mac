
//============================================================================
//----------------------------------------------------------------------------
//									Sound.c
//----------------------------------------------------------------------------
//============================================================================

#include "Externs.h"

#define kMaxSounds				17		
#define	kBaseBufferSoundID		1000		
#define kSoundDone				913			
#define kSoundDone2				749			

extern prefsInfo thePrefs;

void PlaySound1 (short, short);
void PlaySound2 (short, short);
pascal void ExternalCallBack (SndChannelPtr, SndCommand *);
pascal void ExternalCallBack2 (SndChannelPtr, SndCommand *);
void LoadAllSounds (void);
OSErr LoadBufferSounds (void);
OSErr DumpBufferSounds (void);
OSErr OpenSoundChannel (void);
OSErr CloseSoundChannel (void);


SndCallBackUPP		externalCallBackUPP, externalCallBackUPP2;
SndChannelPtr		externalChannel, externalChannel2;
Ptr					theSoundData[kMaxSounds];
short				externalPriority, externalPriority2;
Boolean				channelOpen;


//==============================================================  Functions
//--------------------------------------------------------------  PlaySound1
void PlaySound1 (short soundID, short priority)
{
	SndCommand	theCommand;
	OSErr		theErr;
	
	theCommand.cmd = flushCmd;		
	theCommand.param1 = 0;
	theCommand.param2 = 0L;
	theErr = SndDoImmediate(externalChannel, &theCommand);
	
	theCommand.cmd = quietCmd;			
	theCommand.param1 = 0;
	theCommand.param2 = 0L;
	theErr = SndDoImmediate(externalChannel, &theCommand);
	
	externalPriority = priority;		
	
	theCommand.cmd = bufferCmd;			
	theCommand.param1 = 0;				
	theCommand.param2 = (long)(theSoundData[soundID]);
	theErr = SndDoImmediate(externalChannel, &theCommand);
	
	theCommand.cmd = callBackCmd;		
	theCommand.param1 = kSoundDone;		
	theCommand.param2 = SetCurrentA5();
	theErr = SndDoCommand(externalChannel, &theCommand, TRUE);
}

//--------------------------------------------------------------  PlaySound2
void PlaySound2 (short soundID, short priority)
{
	SndCommand	theCommand;
	OSErr		theErr;
	
	theCommand.cmd = flushCmd;			
	theCommand.param1 = 0;
	theCommand.param2 = 0L;
	theErr = SndDoImmediate(externalChannel2, &theCommand);
	
	theCommand.cmd = quietCmd;			
	theCommand.param1 = 0;
	theCommand.param2 = 0L;
	theErr = SndDoImmediate(externalChannel2, &theCommand);
	
	externalPriority2 = priority;		
	
	theCommand.cmd = bufferCmd;			
	theCommand.param1 = 0;				
	theCommand.param2 = (long)(theSoundData[soundID]);
	theErr = SndDoImmediate(externalChannel2, &theCommand);
	
	theCommand.cmd = callBackCmd;	
	theCommand.param1 = kSoundDone2;	
	theCommand.param2 = SetCurrentA5();

	theErr = SndDoCommand(externalChannel2, &theCommand, TRUE);
}

//--------------------------------------------------------  PlayExternalSound
void PlayExternalSound (short soundID, short priority)
{
	if ((soundID >= 0) && (soundID < kMaxSounds))
	{		
		if (thePrefs.soundOff == false) {
			if (externalPriority < externalPriority2)
			{			
				if (priority >= externalPriority)
					PlaySound1(soundID, priority);
			}
			else
			{	
				if (priority >= externalPriority2)
					PlaySound2(soundID, priority);
			}
		}
	}
}

//--------------------------------------------------------  ExternalCallBack
pascal void ExternalCallBack (SndChannelPtr theChannel, SndCommand *theCommand)
{
	long		thisA5, gameA5;
	
	if (theCommand->param1 == kSoundDone)	
	{
		gameA5 = theCommand->param2;
		thisA5 = SetA5(gameA5);
		
		externalPriority = 0;
		
		thisA5 = SetA5(thisA5);
	}
}

//--------------------------------------------------------  ExternalCallBack2
pascal void ExternalCallBack2 (SndChannelPtr theChannel, SndCommand *theCommand)
{
	long		thisA5, gameA5;
	
	if (theCommand->param1 == kSoundDone2)	
	{
		gameA5 = theCommand->param2;		
		thisA5 = SetA5(gameA5);				
		
		externalPriority2 = 0;
		
		thisA5 = SetA5(thisA5);
	}
}

//--------------------------------------------------------  LoadBufferSounds
OSErr LoadBufferSounds (void)
{
	Handle		theSound;
	long		soundDataSize;
	OSErr		theErr;
	short		i;
	
	theErr = noErr;						
		
	for (i = 0; i < kMaxSounds; i++)	
	{									
		theSound = GetResource('snd ', i + kBaseBufferSoundID);
		if (theSound == 0L)				
			return (ResError());		
			
		HLock(theSound);				
		
		soundDataSize = GetHandleSize(theSound) - 20L;
		HUnlock(theSound);				
		
		theSoundData[i] = NewPtr(soundDataSize);
		if (theSoundData[i] == 0L)		
			return (MemError());		
		HLock(theSound);				
		
		BlockMove((Ptr)(*theSound + 20L), theSoundData[i], soundDataSize);
		HUnlock(theSound);				
		ReleaseResource(theSound);		
	}
	
	return (theErr);
}

//--------------------------------------------------------  DumpBufferSounds
OSErr DumpBufferSounds (void)
{
	OSErr		theErr;
	short		i;
	
	theErr = noErr;
	
	for (i = 0; i < kMaxSounds; i++)		
	{
		if (theSoundData[i] != 0L)
			DisposePtr(theSoundData[i]);
		theSoundData[i] = 0L;
	}
	
	return (theErr);
}

//--------------------------------------------------------  OpenSoundChannel
OSErr OpenSoundChannel (void)
{
	OSErr		theErr;
	
	externalCallBackUPP = (SndCallBackUPP) &ExternalCallBack;
	externalCallBackUPP2 = (SndCallBackUPP) &ExternalCallBack2;
	
	theErr = noErr;
	
	if (channelOpen)
		return (theErr);
	
	externalChannel = 0L;
	theErr = SndNewChannel(&externalChannel, 
			sampledSynth, initNoInterp + initMono, 
			(SndCallBackUPP)externalCallBackUPP);
	if (theErr == noErr)
		channelOpen = TRUE;
	
	externalChannel2 = 0L;
	theErr = SndNewChannel(&externalChannel2,
			sampledSynth, initNoInterp + initMono, 
			(SndCallBackUPP)externalCallBackUPP2);
	if (theErr == noErr)
		channelOpen = TRUE;
	
	return (theErr);
}

//--------------------------------------------------------  CloseSoundChannel
OSErr CloseSoundChannel (void)
{
	OSErr		theErr;
	
	theErr = noErr;
	
	if (!channelOpen)
		return (theErr);
	
	if (externalChannel != 0L)
		theErr = SndDisposeChannel(externalChannel, TRUE);
	externalChannel = 0L;
	
	if (externalChannel2 != 0L)
		theErr = SndDisposeChannel(externalChannel2, TRUE);
	externalChannel2 = 0L;
	
	if (theErr == noErr)
		channelOpen = FALSE;
	
	return (theErr);
}

//--------------------------------------------------------  InitSound
void InitSound (void)
{
	OSErr		theErr;
	
	externalChannel = 0L;
	externalChannel2 = 0L;
	externalPriority = 0;
	externalPriority2 = 0;
	
	theErr = LoadBufferSounds();
	if (theErr != noErr)
		RedAlert("\pFailed Loading Sounds");
	theErr = OpenSoundChannel();
	if (theErr != noErr)
		RedAlert("\pFailed To Open Sound Channels");
}

//--------------------------------------------------------  KillSound
void KillSound (void)
{
	OSErr		theErr;
	
	theErr = DumpBufferSounds();
	theErr = CloseSoundChannel();
}

