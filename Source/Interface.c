
//============================================================================
//----------------------------------------------------------------------------
//									Interface.c
//----------------------------------------------------------------------------
//============================================================================

#include "Externs.h"

#define iAbout				1
#define iNewGame			1
#define iPauseGame			2
#define iEndGame			3
#define iScoreReset			4
#define iHelp				1
#define iHighScores			3
#define iAboutSource		6

void DoAppleMenu (short);
void DoGameMenu (short);
void DoOptionsMenu (short);
void UpdateMainWindow (void);
void HandleMouseEvent (EventRecord *);
void HandleKeyEvent (EventRecord *);
void HandleUpdateEvent (EventRecord *);
void HandleOSEvent (EventRecord *);
void HandleHighLevelEvent (EventRecord *);
void DoAbout (void);
void DoAboutSource (void);
void DoScoreReset (void);

extern void CheckHighScore (void);

		Rect		mainWindowRect;
		WindowPtr	mainWindow;
		MenuHandle	appleMenu, gameMenu, optionsMenu;
		Boolean		switchedOut, quitting, canPlay, openTheScores;

extern	prefsInfo	thePrefs;
extern	Rect		backSrcRect, workSrcRect;
extern	GWorldPtr	backSrcMap, workSrcMap;
extern	Boolean		pausing, playing, helpOpen, scoresOpen;


//==============================================================  Functions
//--------------------------------------------------------------  MenusReflectMode

void MenusReflectMode(void)
{
	if (playing)
	{
		DisableMenuItem(gameMenu, iNewGame);
		EnableMenuItem(gameMenu, iPauseGame);
		if (pausing) {
			ShowCursor();
			SetMenuItemText(gameMenu, iPauseGame, "\pResume Game");
			SetItemCmd(gameMenu, iPauseGame, (short)'R');
		}
		else {
#ifdef OBSCURE_CURSOR_DURING_PLAY
			ObscureCursor();
#endif
			SetMenuItemText(gameMenu, iPauseGame, "\pPause Game");
			SetItemCmd(gameMenu, iPauseGame, (short)'P');
		}
		EnableMenuItem(gameMenu, iEndGame);
		DisableMenuItem(optionsMenu, 0);
	}
	else
	{
		EnableMenuItem(gameMenu, iNewGame);
		SetMenuItemText(gameMenu, iPauseGame, "\pPause Game");
		DisableMenuItem(gameMenu, iPauseGame);
		DisableMenuItem(gameMenu, iEndGame);
		EnableMenuItem(optionsMenu, 0);
	}
}

//--------------------------------------------------------------  DoAppleMenu

void DoAppleMenu (short theItem)
{
	switch (theItem) {
		case iAbout:
			if ((scoresOpen) || (helpOpen)) {
				CloseWall();
				scoresOpen = FALSE;
				helpOpen = FALSE;
				CheckMenuItem(optionsMenu, iHelp, helpOpen);
				CheckMenuItem(optionsMenu, iHighScores, scoresOpen);
			}
			DoAbout();
			break;
			
		default:
			// Apple menu item handling routines previously kept here
			break;
	}
}

//--------------------------------------------------------------  DoGameMenu

void DoGameMenu(short theItem)
{
	switch (theItem) {
		case iNewGame:
			if ((scoresOpen) || (helpOpen)) {
				CloseWall();
				scoresOpen = FALSE;
				helpOpen = FALSE;
				CheckMenuItem(optionsMenu, iHelp, helpOpen);
				CheckMenuItem(optionsMenu, iHighScores, scoresOpen);
			}
			InitNewGame();
			MenusReflectMode();
			break;
			
		case iPauseGame:
			if (pausing) {
				pausing = FALSE;
				DrawFrame();
			}
			break;
			
		case iEndGame:
			pausing = FALSE;
			playing = FALSE;
			DrawFrame();
			PlayExternalSound(kMusicSound, kMusicPriority);
			CheckHighScore();
			ShowCursor();
			MenusReflectMode();
			FlushEvents(everyEvent, 0);
			break;
			
		case iQuit:
			quitting = TRUE;
			break;
	}
}

//--------------------------------------------------------------  DoOptionsMenu

void DoOptionsMenu(short theItem)
{
	switch (theItem) {
		case iScoreReset:
			if ((scoresOpen) || (helpOpen))
			{
				CloseWall();
				scoresOpen = FALSE;
				helpOpen = FALSE;
				CheckMenuItem(optionsMenu, iHelp, helpOpen);
				CheckMenuItem(optionsMenu, iHighScores, scoresOpen);
			}
			DoScoreReset();
			break;
			
		case iHelp:
			if (helpOpen)
			{
				CloseWall();
				helpOpen = FALSE;
			}
			else
			{
				if (scoresOpen)
				{
					CloseWall();
					scoresOpen = FALSE;
					CheckMenuItem(optionsMenu, iHighScores, scoresOpen);
				}
				OpenHelp();
			}
			CheckMenuItem(optionsMenu, iHelp, helpOpen);
			break;
			
		case iHighScores:
			if (scoresOpen)
			{
				CloseWall();
				scoresOpen = FALSE;
			}
			else
			{
				if (helpOpen)
				{
					CloseWall();
					helpOpen = FALSE;
					CheckMenuItem(optionsMenu, iHelp, helpOpen);
				}
				OpenHighScores();
			}
			CheckMenuItem(optionsMenu, iHighScores, scoresOpen);
			break;
			
		case iAboutSource:
			DoAboutSource();
			break;
		case iSoundItem:
			thePrefs.soundOff = !thePrefs.soundOff;
			CheckMenuItem(optionsMenu, iSoundItem, thePrefs.soundOff);
			break;
	}
}

//--------------------------------------------------------------  DoMenuChoice

void DoMenuChoice (long menuChoice)
{
	short		theMenu, theItem;
	
	if (menuChoice == 0)
		return;
	
	theMenu = HiWord(menuChoice);
	theItem = LoWord(menuChoice);
	
	switch (theMenu) {
		case kAppleMenuID:
			DoAppleMenu(theItem);
			break;
			
		case kGameMenuID:
			DoGameMenu(theItem);
			break;
			
		case kOptionsMenuID:
			DoOptionsMenu(theItem);
			break;
	}
	
	HiliteMenu(0);
}

//--------------------------------------------------------------  UpdateMainWindow

void UpdateMainWindow(void)
{
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &mainWindowRect, &mainWindowRect,
			 srcCopy, NULL);
}

//--------------------------------------------------------------  HandleMouseEvent

void HandleMouseEvent(EventRecord *theEvent)
{
	WindowPtr	whichWindow;
	BitMap		screenbits;
	long		menuChoice;
	short		thePart;
	
	thePart = FindWindow(theEvent->where, &whichWindow);
	
	switch (thePart) {
		case inMenuBar:
			menuChoice = MenuSelect(theEvent->where);
			if (canPlay)
				DoMenuChoice(menuChoice);
			break;
			
		case inDrag:
			GetQDGlobalsScreenBits(&screenbits);
			DragWindow(whichWindow, theEvent->where, &screenbits.bounds);
			break;
			
		case inGoAway:
		case inGrow:
		case inZoomIn:
		case inZoomOut:
			break;
			
		case inContent:
			GlobalToLocal(&(theEvent->where));
			FlashObelisks(TRUE);
			LogNextTick(3);
			GenerateLightning(theEvent->where.h, theEvent->where.v);
			StrikeLightning();
			WaitForNextTick();
			StrikeLightning();
			LogNextTick(2);
			WaitForNextTick();
			PlayExternalSound(kLightningSound, kLightningPriority);
			LogNextTick(3);
			GenerateLightning(theEvent->where.h, theEvent->where.v);
			StrikeLightning();
			WaitForNextTick();
			StrikeLightning();
			LogNextTick(2);
			WaitForNextTick();
			LogNextTick(3);
			GenerateLightning(theEvent->where.h, theEvent->where.v);
			StrikeLightning();
			WaitForNextTick();
			StrikeLightning();
			LogNextTick(2);
			WaitForNextTick();
			PlayExternalSound(kLightningSound, kLightningPriority);
			LogNextTick(3);
			GenerateLightning(theEvent->where.h, theEvent->where.v);
			StrikeLightning();
			WaitForNextTick();
			StrikeLightning();
			LogNextTick(2);
			WaitForNextTick();
			FlashObelisks(FALSE);
			break;
	}
}

//--------------------------------------------------------------  HandleKeyEvent

void HandleKeyEvent (EventRecord *theEvent)
{
	char		theChar;
	Boolean		commandDown;
	
	theChar = theEvent->message & charCodeMask;
	commandDown = ((theEvent->modifiers & cmdKey) != 0);
	
	if ((commandDown) && (theEvent->what == keyDown))
	{
		if (canPlay)
			DoMenuChoice(MenuKey(theChar));
	}
	else
	{
		if (helpOpen)
		{
			if (theChar == kUpArrowKeyASCII)
			{
				if (theEvent->what == autoKey)
					ScrollHelp(-3);
				else
					ScrollHelp(-1);
			}
			else if (theChar == kDownArrowKeyASCII)
			{
				if (theEvent->what == autoKey)
					ScrollHelp(3);
				else
					ScrollHelp(1);
			}
			else if (theChar == kPageDownKeyASCII)
			{
				ScrollHelp(199);
			}
			else if (theChar == kPageUpKeyASCII)
			{
				ScrollHelp(-199);
			}
			else if ((theChar == kHelpKeyASCII) && (!playing))
			{
				CloseWall();
				helpOpen = FALSE;
				CheckMenuItem(optionsMenu, iHelp, helpOpen);
			}
		}
		else if ((theChar == kHelpKeyASCII) && (!playing))
		{
			if (scoresOpen)
			{
				CloseWall();
				scoresOpen = FALSE;
				CheckMenuItem(optionsMenu, iHighScores, scoresOpen);
			}
			OpenHelp();
			CheckMenuItem(optionsMenu, iHelp, helpOpen);
		}
	}
}

//--------------------------------------------------------------  HandleUpdateEvent

void HandleUpdateEvent(EventRecord *theEvent)
{
	if ((WindowPtr)theEvent->message == mainWindow) {
		SetPortWindowPort(mainWindow);
		BeginUpdate(mainWindow);
		UpdateMainWindow();
		EndUpdate(mainWindow);
		canPlay = TRUE;
	}
}

//--------------------------------------------------------------  HandleOSEvent

void HandleOSEvent(EventRecord *theEvent)
{	
	if (theEvent->message & 0x01000000) {		// suspend or resume event
		if (theEvent->message & 0x00000001) {	// resume event
			switchedOut = FALSE;
		} else {								// suspend event
			switchedOut = TRUE;
		}
	}
}

//--------------------------------------------------------------  HandleHighLevelEvent

void HandleHighLevelEvent(EventRecord *theEvent)
{	
	AEProcessAppleEvent(theEvent);
}

//--------------------------------------------------------------  HandleEvent

void HandleEvent()
{
	EventRecord	theEvent;
	long		sleep = 1L;
	Boolean		itHappened;
	
	itHappened = WaitNextEvent(everyEvent, &theEvent, sleep, NULL);
	
	if (itHappened) {
		switch (theEvent.what) {
			case mouseDown:
				HandleMouseEvent(&theEvent);
				break;
				
			case keyDown:
			case autoKey:
				HandleKeyEvent(&theEvent);
				break;
				
			case updateEvt:
				HandleUpdateEvent(&theEvent);
				break;
				
			case osEvt:
				HandleOSEvent(&theEvent);
				break;
				
			case kHighLevelEvent:
				HandleHighLevelEvent(&theEvent);
				break;
		}
	} else if (openTheScores) {
		openTheScores = FALSE;
		OpenHighScores();
	}
}

//--------------------------------------------------------------  DoAbout

void DoAbout (void)
{
	Rect		aboutRect;
	WindowPtr	aboutWindow;
	BitMap		screenBits;
	EventRecord theEvent;
	Boolean 	isEvent, windowDone = false;
	
	HiliteMenu(0);
	SetRect(&aboutRect, 0, 0, 325, 318);
	GetQDGlobalsScreenBits(&screenBits);
	CenterRectInRect(&aboutRect, &screenBits.bounds);
	aboutWindow = GetNewWindow(129, NULL, kPutInFront);
	SetPortWindowPort(aboutWindow);
	LoadGraphic(kAboutPictID);
	
	do {
		isEvent = WaitNextEvent(everyEvent, &theEvent, 120, NULL);
		if (isEvent) {
			switch (theEvent.what) {
				case kHighLevelEvent:
					HandleHighLevelEvent(&theEvent);
					break;
					
				case updateEvt:
					LoadGraphic(kAboutPictID);
					break;
					
				case mouseDown:
					windowDone = true;
					break;
			}
		}
	} while (windowDone == false);
	
	SetPortWindowPort(mainWindow);
	if (aboutWindow != NULL)
		DisposeWindow(aboutWindow);
}

//--------------------------------------------------------------  DoAboutSource

void DoAboutSource()
{
	DialogPtr	theDial;
	short		item;
	Boolean		leaving = false;
	
	HiliteMenu(0);
	theDial = GetNewDialog(kAboutSourceDialog, NULL, kPutInFront);
	SetDialogDefaultItem(theDial, 1);
	FlushEvents(everyEvent, 0);
	
	while (!leaving) {
		ModalDialog(NULL, &item);
		
		switch (item) {
			case 1:
				leaving = true;
				break;
		}
	}
	
	DisposeDialog(theDial);
}

//--------------------------------------------------------------  DoScoreReset

void DoScoreReset()
{
	DialogPtr	theDial;
	short		i, item;
	Boolean		leaving = false;
	
	HiliteMenu(0);
	theDial = GetNewDialog(kScoreResetDialog, NULL, kPutInFront);
	SetDialogDefaultItem(theDial, 1);
	FlushEvents(everyEvent, 0);
	
	while (!leaving)
	{
		ModalDialog(NULL, &item);
		
		switch (item) {
			case 1:
				for (i = 0; i < 10; i++)
				{
					PasStringCopy("\pNemo", thePrefs.highNames[i]);
					thePrefs.highScores[i] = NULL;
					thePrefs.highLevel[i] = 0;
					openTheScores = TRUE;
				}
				leaving = true;
			break;
			case 2:
				leaving = true;
			break;
		}
	}
	
	DisposeDialog(theDial);
}

