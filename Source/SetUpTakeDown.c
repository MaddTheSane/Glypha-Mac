
//============================================================================
//----------------------------------------------------------------------------
//								SetUpTakeDown.c
//----------------------------------------------------------------------------
//============================================================================

#include "Externs.h"

		short		wasDepth;

extern	Rect		mainWindowRect, backSrcRect, workSrcRect, obSrcRect, playerSrcRect;
extern	Rect		numberSrcRect, idleSrcRect, enemyWalkSrcRect, enemyFlySrcRect;
extern	Rect		obeliskRects[4], playerRects[11], numbersSrc[11], numbersDest[11];
extern	Rect		platformRects[6], touchDownRects[6], enemyRects[24], handSrcRect;
extern	Rect		flameSrcRect, flameDestRects[2], flameRects[4], platformCopyRects[9];
extern	Rect		enemyInitRects[5], eggSrcRect, platformSrcRect, helpSrcRect;
extern	Rect		handRects[2], grabZone, eyeSrcRect, eyeRects[4];
extern	GWorldPtr	backSrcMap, workSrcMap, obeliskSrcMap, playerSrcMap;
extern	GWorldPtr	numberSrcMap, idleSrcMap, enemyWalkSrcMap, enemyFlySrcMap;
extern	GWorldPtr	flameSrcMap, eggSrcMap, platformSrcMap, helpSrcMap, handSrcMap;
extern	GWorldPtr	eyeSrcMap;
extern	GWorldPtr		playerMaskMap, enemyWalkMaskMap, enemyFlyMaskMap, eggMaskMap;
extern	GWorldPtr		handMaskMap, eyeMaskMap;
extern	WindowPtr	mainWindow;
extern	RgnHandle	playRgn;
extern	MenuHandle	appleMenu, gameMenu, optionsMenu;
extern	long		theScore, wasTensOfThousands;
extern	short		numLedges, beginOnLevel, levelOn, livesLeft;
extern	Boolean		quitting, playing, pausing, switchedOut, canPlay, whichList;
extern	Boolean		helpOpen, scoresOpen, openTheScores;


//==============================================================  Functions
//--------------------------------------------------------------  OpenMainWindow

void OpenMainWindow (void)
{
	SetRect(&mainWindowRect, 0, 0, 640, 460);
	mainWindow = GetNewWindow(128, NULL, kPutInFront);
	SetPortWindowPort(mainWindow);
	ForeColor(blackColor);
	BackColor(whiteColor);
}

//--------------------------------------------------------------  InitMenubar

void InitMenubar(void)
{
	long	response;
	OSErr	err;
	
	appleMenu = GetMenu(kAppleMenuID);
	if (appleMenu == NULL)
		RedAlert("\pCouldn't Load Menus Error");
	AppendResMenu(appleMenu, 'DRVR');
	InsertMenu(appleMenu, 0);
	
	gameMenu = GetMenu(kGameMenuID);
	if (gameMenu == NULL)
		RedAlert("\pCouldn't Load Menus Error");
	InsertMenu(gameMenu, 0);
	
	optionsMenu = GetMenu(kOptionsMenuID);
	if (optionsMenu == NULL)
		RedAlert("\pCouldn't Load Menus Error");
	InsertMenu(optionsMenu, 0);
	
	err = Gestalt(gestaltMenuMgrAttr, &response);
	if ((err == noErr) && (response & gestaltMenuMgrAquaLayoutMask)) {
		DeleteMenuItems(gameMenu, iQuit - 1, 2);
	}
	
	MenusReflectMode();
	DrawMenuBar();
	
}

//--------------------------------------------------------------  InitVariables

void InitVariables()
{
	short i;
	
	quitting = FALSE;
	playing = FALSE;
	pausing = FALSE;
	canPlay = FALSE;
	whichList = TRUE;
	helpOpen = FALSE;
	scoresOpen = FALSE;
	openTheScores = FALSE;
	
	numLedges = 3;
	beginOnLevel = 1;
	levelOn = 0;
	livesLeft = kInitNumLives;
	theScore = NULL;
	wasTensOfThousands = NULL;
	GenerateLightning(320, 240);
	
	backSrcRect = mainWindowRect;
	ZeroRectCorner(&backSrcRect);
	backSrcMap = NULL;
	CreateOffScreenPixMap(&backSrcRect, &backSrcMap);
	LoadGraphic(kBackgroundPictID);
	// END EDIT
	
	workSrcRect = mainWindowRect;
	ZeroRectCorner(&workSrcRect);
	workSrcMap = NULL;
	CreateOffScreenPixMap(&workSrcRect, &workSrcMap);
	// END EDIT
	
	SetRect(&obSrcRect, 0, 0, 20, 418);
	obeliskSrcMap = NULL;
	CreateOffScreenPixMap(&obSrcRect, &obeliskSrcMap);
	LoadGraphic(kObeliskPictID);
	// END EDIT
	SetRect(&obeliskRects[0], 0, 0, 20, 209);
	OffsetRect(&obeliskRects[0], 0, 0);
	SetRect(&obeliskRects[1], 0, 0, 20, 209);
	OffsetRect(&obeliskRects[1], 0, 209);
	SetRect(&obeliskRects[2], 0, 0, 20, 209);
	OffsetRect(&obeliskRects[2], 161, 250);
	SetRect(&obeliskRects[3], 0, 0, 20, 209);
	OffsetRect(&obeliskRects[3], 457, 250);
	
	SetRect(&playerSrcRect, 0, 0, 48, 436);
	playerSrcMap = NULL;
	CreateOffScreenPixMap(&playerSrcRect, &playerSrcMap);
	LoadGraphic(kPlayerPictID);
	// END EDIT
	playerMaskMap = NULL;
	CreateOffScreenBitMap(&playerSrcRect, &playerMaskMap);
	LoadGraphic(kPlayerMaskID);
	// END EDIT
	
	SetRect(&enemyWalkSrcRect, 0, 0, 48, 576);
	enemyWalkSrcMap = NULL;
	CreateOffScreenPixMap(&enemyWalkSrcRect, &enemyWalkSrcMap);
	LoadGraphic(kEnemyWalkPictID);
	// END EDIT
	enemyWalkMaskMap = NULL;
	CreateOffScreenBitMap(&enemyWalkSrcRect, &enemyWalkMaskMap);
	LoadGraphic(kEnemyWalkMaskID);
	// END EDIT
	SetRect(&enemyFlySrcRect, 0, 0, 64, 480);
	enemyFlySrcMap = NULL;
	CreateOffScreenPixMap(&enemyFlySrcRect, &enemyFlySrcMap);
	LoadGraphic(kEnemyFlyPictID);
	// END EDIT
	enemyFlyMaskMap = NULL;
	CreateOffScreenBitMap(&enemyFlySrcRect, &enemyFlyMaskMap);
	LoadGraphic(kEnemyFlyMaskID);
	// END EDIT
	for (i = 0; i < 12; i++) {
		SetRect(&enemyRects[i], 0, 0, 48, 48);
		OffsetRect(&enemyRects[i], 0, 48 * i);
	}
	
	for (i = 0; i < 12; i++) {
		SetRect(&enemyRects[i + 12], 0, 0, 64, 40);
		OffsetRect(&enemyRects[i + 12], 0, 40 * i);
	}
	SetRect(&enemyInitRects[0], 0, 0, 48, 1);
	OffsetRect(&enemyInitRects[0], 72, 284);
	SetRect(&enemyInitRects[1], 0, 0, 48, 1);
	OffsetRect(&enemyInitRects[1], 520, 284);
	SetRect(&enemyInitRects[2], 0, 0, 48, 1);
	OffsetRect(&enemyInitRects[2], 72, 105);
	SetRect(&enemyInitRects[3], 0, 0, 48, 1);
	OffsetRect(&enemyInitRects[3], 520, 105);
	SetRect(&enemyInitRects[4], 0, 0, 48, 1);
	OffsetRect(&enemyInitRects[4], 296, 190);
	
	SetRect(&eggSrcRect, 0, 0, 24, 24);
	eggSrcMap = NULL;
	CreateOffScreenPixMap(&eggSrcRect, &eggSrcMap);
	LoadGraphic(kEggPictID);
	// END EDIT
	eggMaskMap = NULL;
	CreateOffScreenBitMap(&eggSrcRect, &eggMaskMap);
	LoadGraphic(kEggMaskID);
	// END EDIT
	
	SetRect(&eyeSrcRect, 0, 0, 48, 124);
	eyeSrcMap = NULL;
	CreateOffScreenPixMap(&eyeSrcRect, &eyeSrcMap);
	LoadGraphic(kEyePictID);
	// END EDIT
	eyeMaskMap = NULL;
	CreateOffScreenBitMap(&eyeSrcRect, &eyeMaskMap);
	LoadGraphic(kEyeMaskID);
	// END EDIT
	for (i = 0; i < 4; i++) {
		SetRect(&eyeRects[i], 0, 0, 48, 31);
		OffsetRect(&eyeRects[i], 0, i * 31);
	}
	
	SetRect(&handSrcRect, 0, 0, 56, 114);
	handSrcMap = NULL;
	CreateOffScreenPixMap(&handSrcRect, &handSrcMap);
	LoadGraphic(kHandPictID);
	// END EDIT
	handMaskMap = NULL;
	CreateOffScreenBitMap(&handSrcRect, &handMaskMap);
	LoadGraphic(kHandMaskID);
	// END EDIT
	SetRect(&handRects[0], 0, 0, 56, 57);
	OffsetRect(&handRects[0], 0, 0);
	SetRect(&handRects[1], 0, 0, 56, 57);
	OffsetRect(&handRects[1], 0, 57);
	SetRect(&grabZone, 0, 0, 96, 108);
	OffsetRect(&grabZone, 48, 352);
	
	SetRect(&idleSrcRect, 0, 0, 48, 48);
	idleSrcMap = NULL;
	CreateOffScreenPixMap(&idleSrcRect, &idleSrcMap);
	LoadGraphic(kIdlePictID);
	// END EDIT
	
	SetRect(&flameSrcRect, 0, 0, 16, 64);
	flameSrcMap = NULL;
	CreateOffScreenPixMap(&flameSrcRect, &flameSrcMap);
	LoadGraphic(kFlamePictID);
	// END EDIT
	SetRect(&flameDestRects[0], 0, 0, 16, 16);
	OffsetRect(&flameDestRects[0], 87, 325);
	SetRect(&flameDestRects[1], 0, 0, 16, 16);
	OffsetRect(&flameDestRects[1], 535, 325);
	for (i = 0; i < 4; i++)
	{
		SetRect(&flameRects[i], 0, 0, 16, 16);
		OffsetRect(&flameRects[i], 0, i * 16);
	}
	
	SetRect(&numberSrcRect, 0, 0, 8, 121);
	numberSrcMap = NULL;
	CreateOffScreenPixMap(&numberSrcRect, &numberSrcMap);
	LoadGraphic(kNumberPictID);
	// END EDIT
	for (i = 0; i < 11; i++)
	{
		SetRect(&numbersSrc[i], 0, 0, 8, 11);
		OffsetRect(&numbersSrc[i], 0, 11 * i);
	}
	SetRect(&numbersDest[0], 0, 0, 8, 11);	// # of lives digit 1
	OffsetRect(&numbersDest[0], 229, 443);
	SetRect(&numbersDest[1], 0, 0, 8, 11);	// # of lives digit 2
	OffsetRect(&numbersDest[1], 237, 443);
	SetRect(&numbersDest[2], 0, 0, 8, 11);	// score digit 1
	OffsetRect(&numbersDest[2], 293, 443);
	SetRect(&numbersDest[3], 0, 0, 8, 11);	// score digit 2
	OffsetRect(&numbersDest[3], 301, 443);
	SetRect(&numbersDest[4], 0, 0, 8, 11);	// score digit 3
	OffsetRect(&numbersDest[4], 309, 443);
	SetRect(&numbersDest[5], 0, 0, 8, 11);	// score digit 4
	OffsetRect(&numbersDest[5], 317, 443);
	SetRect(&numbersDest[6], 0, 0, 8, 11);	// score digit 5
	OffsetRect(&numbersDest[6], 325, 443);
	SetRect(&numbersDest[7], 0, 0, 8, 11);	// score digit 6
	OffsetRect(&numbersDest[7], 333, 443);
	SetRect(&numbersDest[8], 0, 0, 8, 11);	// # of level digit 1
	OffsetRect(&numbersDest[8], 381, 443);
	SetRect(&numbersDest[9], 0, 0, 8, 11);	// # of level digit 2
	OffsetRect(&numbersDest[9], 389, 443);
	SetRect(&numbersDest[10], 0, 0, 8, 11);	// # of level digit 3
	OffsetRect(&numbersDest[10], 397, 443);
	
	SetRect(&playerRects[0], 0, 0, 48, 37);
	OffsetRect(&playerRects[0], 0, 0);
	SetRect(&playerRects[1], 0, 0, 48, 37);
	OffsetRect(&playerRects[1], 0, 37);
	SetRect(&playerRects[2], 0, 0, 48, 37);
	OffsetRect(&playerRects[2], 0, 74);
	SetRect(&playerRects[3], 0, 0, 48, 37);
	OffsetRect(&playerRects[3], 0, 111);
	SetRect(&playerRects[4], 0, 0, 48, 48);
	OffsetRect(&playerRects[4], 0, 148);
	SetRect(&playerRects[5], 0, 0, 48, 48);
	OffsetRect(&playerRects[5], 0, 196);
	SetRect(&playerRects[6], 0, 0, 48, 48);
	OffsetRect(&playerRects[6], 0, 244);
	SetRect(&playerRects[7], 0, 0, 48, 48);
	OffsetRect(&playerRects[7], 0, 292);
	SetRect(&playerRects[8], 0, 0, 48, 37);		// falling bones rt.
	OffsetRect(&playerRects[8], 0, 340);
	SetRect(&playerRects[9], 0, 0, 48, 37);		// falling bones lf.
	OffsetRect(&playerRects[9], 0, 377);
	SetRect(&playerRects[10], 0, 0, 48, 22);	// pile of bones
	OffsetRect(&playerRects[10], 0, 414);
	
	MoveTo(0, 0);			// generate clipping region that excludes the obelisks
	playRgn = NewRgn();
	OpenRgn();
	LineTo(0, 450);
	LineTo(161, 450);
	LineTo(161, 269);
	LineTo(172, 250);
	LineTo(182, 269);
	LineTo(182, 450);
	LineTo(457, 450);
	LineTo(457, 269);
	LineTo(468, 250);
	LineTo(478, 269);
	LineTo(478, 450);
	LineTo(640, 450);
	LineTo(640, 0);
	LineTo(0, 0);
	CloseRgn(playRgn);
	MoveHHi((Handle)playRgn);
	HLock((Handle)playRgn);
	
	SetRect(&platformSrcRect, 0, 0, 191, 192);
	platformSrcMap = NULL;
	CreateOffScreenPixMap(&platformSrcRect, &platformSrcMap);
	LoadGraphic(kPlatformPictID);
	// END EDIT
	for (i = 0; i < 6; i++)
	{
		SetRect(&platformCopyRects[i], 0, 0, 191, 32);
		OffsetRect(&platformCopyRects[i], 0, 32 * i);
	}
	SetRect(&platformCopyRects[6], 233, 190, 424, 222);
	SetRect(&platformCopyRects[7], 0, 105, 191, 137);
	SetRect(&platformCopyRects[8], 449, 105, 640, 137);
	
	SetRect(&platformRects[0], 206, 424, 433, 438);		//_______________
	SetRect(&platformRects[1], -256, 284, 149, 298);	//
	SetRect(&platformRects[2], 490, 284, 896, 298);		//--3--     --4--
	SetRect(&platformRects[3], -256, 105, 149, 119);	//     --5--
	SetRect(&platformRects[4], 490, 105, 896, 119);		//--1--     --2--
	SetRect(&platformRects[5], 233, 190, 407, 204);		//_____--0--_____
	
	for (i = 0; i < 6; i++) {
		touchDownRects[i] = platformRects[i];
		touchDownRects[i].left += 23;
		touchDownRects[i].right -= 23;
		touchDownRects[i].bottom = touchDownRects[i].top;
		touchDownRects[i].top = touchDownRects[i].bottom - 11;
	}
	
	SetRect(&helpSrcRect, 0, 0, 231, 398);
	helpSrcMap = NULL;
	CreateOffScreenPixMap(&helpSrcRect, &helpSrcMap);
	LoadGraphic(kHelpPictID);
	// END EDIT
	SetPortWindowPort(mainWindow); // HERE
}

//--------------------------------------------------------------  ShutItDown

void ShutItDown (void)
{
}

