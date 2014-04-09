
//============================================================================
//----------------------------------------------------------------------------
//								Glypha IV 1.0
//								by  Scheherazade
//----------------------------------------------------------------------------
//============================================================================

// Here is the "main" file for Glypha.  Here is where the game begins and ends.
// Also included are the preference calls.

#include <Carbon/Carbon.h>

#include "G4Externs.h"


#define kPrefsVersion			0x0002


static void ReadInPrefs (void);
static void WriteOutPrefs (void);

prefsInfo	thePrefs;
static long		wasVolume;

//extern	Boolean		quitting, playing, pausing, evenFrame;
void UpdateMainWindow (void);

//==============================================================  AppleEvent Responders
//--------------------------------------------------------------  HandleAEOpenApp
static pascal OSErr HandleAEOpenApp(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	return noErr;
}

//--------------------------------------------------------------  HandleAEQuitApp
static pascal OSErr HandleAEQuitApp(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	playing = FALSE;
	quitting = TRUE;
		
	return noErr;
}

//--------------------------------------------------------------  HandleAEPrintDoc
static pascal OSErr HandleAEPrintDoc(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	return noErr;
}

//--------------------------------------------------------------  HandleAEOpenDoc
static pascal OSErr HandleAEOpenDoc(const AppleEvent *theAppleEvent, AppleEvent* reply, long handlerRefCon) {
	return noErr;
} 

//==============================================================  Functions
//--------------------------------------------------------------  ReadInPrefs

//	This function loads up the preferences.  If the preferences 
//	aren't found, all settings are set to their defaults.

void ReadInPrefs (void)
{
	short		i;
							// Call LoadPrefs() function - returns TRUE if it worked.
	if (LoadPrefs(&thePrefs, kPrefsVersion))
		SetDefaultOutputVolume(thePrefs.wasVolume);
	else					// If LoadPrefs() failed, set defaults.
	{
		thePrefs.prefVersion = kPrefsVersion;				// version of prefs
		thePrefs.filler = 0;								// just padding
		PasStringCopy("\pYour Name", thePrefs.highName);	// last highscores name
		for (i = 0; i < 10; i++)							// loop through scores
		{
			PasStringCopy("\pNemo", thePrefs.highNames[i]);	// put "Nemo" in name
			thePrefs.highScores[i] = 0L;					// set highscore to 0
			thePrefs.highLevel[i] = 0;						// level attained = 0
		}
		GetDefaultOutputVolume (&thePrefs.wasVolume);			// get new volume
//		GetSoundVol(&thePrefs.wasVolume);
	}
							// Get sound volume so we can restore it.
	GetDefaultOutputVolume (&wasVolume);			// get new volume
//	GetSoundVol(&wasVolume);
}

//--------------------------------------------------------------  WriteOutPrefs

//	This function writes out the preferences to disk and restores 
//	the sound volume to its setting before Glypha was launched.

void WriteOutPrefs (void)
{
	if (!SavePrefs(&thePrefs, kPrefsVersion))
		SysBeep(1);
	SetDefaultOutputVolume(wasVolume);
}

//--------------------------------------------------------------  main

//	This is the main function.  Every C program has one of these.
//	First it initializes our program and then falls into a loop
//	until the user chooses to quit.  At that point, it cleans up
//	and exits.

int main (void)
{
	long		tickWait;
	OSStatus	theError;
	Boolean		wasPlaying = false;
	
	// startup DrawSprocket
	theError = DSpStartup();
	if( theError )
		RedAlert("\punable to init DrawSprocket!");
	// DSpSetDebugMode( true );
	
	ToolBoxInit();			// Call function that initializes the ToolBox managers.
	CheckEnvirons();		// Check the Mac we're on to see if we can run.
	OpenMainWindow();		// Open up the main window - it will fill the monitor.
	InitVariables();		// Initialize Glypha's variables.
	InitSound();			// Create sound channels and load up sounds.
	InitMenubar();			// Set up the game's menubar.
	ReadInPrefs();			// Load up the preferences.
	
	UpdateMainWindow();
	
	theError = DSpContext_FadeGammaIn( NULL, NULL );
	if( theError )
		RedAlert("\pUnable to unfade the display!");
	
	AEInstallEventHandler(kCoreEventClass, kAEOpenApplication, NewAEEventHandlerUPP(&HandleAEOpenApp), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(&HandleAEQuitApp), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEPrintDocuments, NewAEEventHandlerUPP(&HandleAEPrintDoc), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP(&HandleAEOpenDoc), 0, false);
	
	do {						// Here begins the main loop.
		HandleEvent();		// Check for events.
		
		if ((playing) && (!pausing))
		{
			wasPlaying = true;
			HideCursor();
			PlayGame();		// If user began game, drop in game loop. (play mode)
			ShowCursor();
		}
		else if (pausing && playing) {
		}
		else				// If no game, animate the screen. (idle mode)
		{
			if (Button())
			{
				Point pt;
				
				GetMouse(&pt);
				pt.h += (Random() % 21) - 10;
				pt.v += (Random() % 21) - 10;
				StartPixelShatter(pt.h, pt.v, 0, 0, kShatterLightningDust);
			}
			
			if (wasPlaying)
			{
				GameQuitGraphics();
				wasPlaying = false;
			}
			tickWait = TickCount() + 2L;
			GameIdleAnimation();
			
			evenFrame = !evenFrame;
		}
	} while (!quitting);
	
	AERemoveEventHandler(kCoreEventClass, kAEOpenApplication, NewAEEventHandlerUPP(&HandleAEOpenApp), false);
	AERemoveEventHandler(kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(&HandleAEQuitApp), false);
	AERemoveEventHandler(kCoreEventClass, kAEPrintDocuments, NewAEEventHandlerUPP(&HandleAEPrintDoc), false);
	AERemoveEventHandler(kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP(&HandleAEOpenDoc), false);
	
	KillSound();			// Dispose of sound channels.
	ShutItDown();			// Dispose of other structures.
	WriteOutPrefs();		// Save preferences to disk.
	
	return 0;
}

