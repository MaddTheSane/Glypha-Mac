
#ifndef Glypha_Externs_h
#define Glypha_Externs_h

//============================================================================
//----------------------------------------------------------------------------
//									Externs.h
//----------------------------------------------------------------------------
//============================================================================

#include <Carbon/Carbon.h>
#include "enums.h"

#define	kPutInFront			(WindowPtr)-1L
#define	kNormalUpdates		TRUE

//--------------------------------------------------------------  Structs

typedef struct {
	Rect		dest, wasDest, wrap;
	short		h, v;
	short		wasH, wasV;
	short		hVel, vVel;
	short		srcNum, mode;
	short		frame;
	Boolean		facingRight, flapping;
	Boolean		walking, wrapping;
	Boolean		clutched;
} playerType;

typedef struct {
	Rect		dest, wasDest;
	short		h, v;
	short		wasH, wasV;
	short		hVel, vVel;
	short		srcNum, mode;
	short		kind, frame;
	short		heightSmell, targetAlt;
	short		flapImpulse, pass;
	short		maxHVel, maxVVel;
	Boolean		facingRight;
} enemyType;

typedef struct {
	Rect		dest;
	short		mode;
} handInfo;

typedef struct {
	Rect		dest;
	short		mode, opening;
	short		srcNum, frame;
	Boolean		killed, entering;
} eyeInfo;

typedef struct {
	short		prefVersion, filler;
	Str255		highName;
	Str15		highNames[10];
	long		highScores[10];
	short		highLevel[10];
	Boolean		soundOff;
} prefsInfo;

//--------------------------------------------------------------  Prototypes

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

extern	short		lightningCount, numEnemies, countDownTimer;
extern	short		lightH, lightV;
extern	Boolean		evenFrame, doEnemyFlapSound, doEnemyScrapeSound;

extern	prefsInfo	thePrefs;

extern	eyeInfo		theEye;
extern	handInfo	theHand;
extern	playerType	thePlayer;
extern	enemyType	theEnemies[kMaxEnemies];
extern	short		numUpdateRects1, numUpdateRects2, numOwls;


// Enemies.c
void GenerateEnemies();
void MoveEnemies();
void InitHandLocation();
void HandleHand();
void InitEye();
void KillOffEye();
void HandleEye();
void CheckPlayerEnemyCollision (void);

// Graphics.c
void DrawPlatforms(short);
void ScrollHelp (short);
void OpenHelp();
void CloseWall (void);
void OpenHighScores (void);
void UpdateLivesNumbers();
void UpdateScoreNumbers (void);
void UpdateLevelNumbers();
void GenerateLightning (short h, short v);
void FlashObelisks(Boolean);
void StrikeLightning();
void DumpBackToWorkMap (void);
void DumpMainToWorkMap();
void AddToUpdateRects (Rect *);
void DrawTorches (void);
void CopyAllRects();
void DrawFrame (void);

// Interface.c
void MenusReflectMode();
void DoMenuChoice(long);
void HandleEvent();

// Play.c
void InitNewGame();
void PlayGame();

// Prefs.c
Boolean SavePrefs(prefsInfo *, short);
Boolean LoadPrefs(prefsInfo *, short);
extern void CheckHighScore(void);

void OpenMainWindow();
void InitMenubar();
void InitVariables();
void ShutItDown();

// Sound.c
void PlayExternalSound(short, short);
void InitSound();
void KillSound();

// Utilities.c
short RandomInt(short);
void RedAlert (StringPtr);
void LoadGraphic (short);
void CreateOffScreenPixMap(Rect *, GWorldPtr *);
void CreateOffScreenBitMap(Rect *, GWorldPtr *);
void ZeroRectCorner (Rect *);
void LogNextTick (long);
void WaitForNextTick();
Boolean DoWeHaveGestalt();
void CenterAlert(short);
short RectWide (Rect *);
short RectTall (Rect *);
void CenterRectInRect(Rect *, Rect *);
void PasStringCopy(StringPtr, StringPtr);
void CenterDialog(short);
void DrawDefaultButton (DialogPtr);
void PasStringCopyNum (StringPtr, StringPtr, short);
void GetDialogString (DialogPtr, short, StringPtr);
void SetDialogString(DialogPtr, short, StringPtr);
void SetDialogNumToStr(DialogPtr, short, long );
void GetDialogNumFromStr (DialogPtr, short, long *);
void CustomDisableControl(DialogPtr, short);

#endif
