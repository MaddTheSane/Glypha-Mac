
//============================================================================
//----------------------------------------------------------------------------
//						 Glypha III 1.1.0 for MacOS X
//						By John Calhoun & Mark Pazolli
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include <time.h>
#include <stdlib.h>

#define kPrefsVersion			0x0001

static void ReadInPrefs();
static void WriteOutPrefs();

prefsInfo	thePrefs;

static pascal OSErr HandleAEOpenApp(const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefCon);
static pascal OSErr HandleAEQuitApp(const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefCon);
static pascal OSErr HandleAEPrintDoc(const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefCon);
static pascal OSErr HandleAEOpenDoc(const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefCon);

//==============================================================  AppleEvent Responders
//--------------------------------------------------------------  HandleAEOpenApp
pascal OSErr HandleAEOpenApp(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	return noErr;
}

//--------------------------------------------------------------  HandleAEQuitApp
pascal OSErr HandleAEQuitApp(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	playing = FALSE;
	quitting = TRUE;
		
	return noErr;
}

//--------------------------------------------------------------  HandleAEPrintDoc
pascal OSErr HandleAEPrintDoc(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	return noErr;
}

//--------------------------------------------------------------  HandleAEOpenDoc
pascal OSErr HandleAEOpenDoc(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	return noErr;
} 

//==============================================================  Functions
//--------------------------------------------------------------  ReadInPrefs

void ReadInPrefs (void)
{
	short		i;
	
	if (!LoadPrefs(&thePrefs, kPrefsVersion)) {
		thePrefs.prefVersion = kPrefsVersion;
		thePrefs.filler = 0;
		PasStringCopy("\pYour Name", thePrefs.highName);
		for (i = 0; i < 10; i++) {
			PasStringCopy("\pNemo", thePrefs.highNames[i]);
			thePrefs.highScores[i] = 0;
			thePrefs.highLevel[i] = 0;
		}
	}
}

//--------------------------------------------------------------  WriteOutPrefs

void WriteOutPrefs (void)
{
	if (!SavePrefs(&thePrefs, kPrefsVersion))
		SysBeep(1);
}

//--------------------------------------------------------------  main

int main (void)
{
	switchedOut = FALSE;
	srandom(time(NULL));
	OpenMainWindow();
	InitVariables();
	InitSound();
	InitMenubar();
	ReadInPrefs();
	
	CheckMenuItem(optionsMenu, iSoundItem, thePrefs.soundOff);
	
	AEInstallEventHandler(kCoreEventClass, kAEOpenApplication, NewAEEventHandlerUPP(&HandleAEOpenApp), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(&HandleAEQuitApp), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEPrintDocuments, NewAEEventHandlerUPP(&HandleAEPrintDoc), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP(&HandleAEOpenDoc), 0, false);

	do {
		HandleEvent();
		if ((playing) && (!pausing))
			PlayGame();
		else {
			LogNextTick(2);
			evenFrame = !evenFrame;
			DrawTorches();
			CopyAllRects();
			WaitForNextTick();
		}
	}
	while (!quitting);
	
	AERemoveEventHandler(kCoreEventClass, kAEOpenApplication, NewAEEventHandlerUPP(&HandleAEOpenApp), false);
	AERemoveEventHandler(kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(&HandleAEQuitApp), false);
	AERemoveEventHandler(kCoreEventClass, kAEPrintDocuments, NewAEEventHandlerUPP(&HandleAEPrintDoc), false);
	AERemoveEventHandler(kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP(&HandleAEOpenDoc), false);
	KillSound();
	ShutItDown();
	WriteOutPrefs();
	
	return 0;
}
