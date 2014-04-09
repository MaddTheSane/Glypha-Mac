
//============================================================================
//----------------------------------------------------------------------------
//									Externs.h
//----------------------------------------------------------------------------
//============================================================================

#include "DrawSprocket.h"
#include <ConditionalMacros.h>
#include <Dialogs.h>
#include <Types.h>
#include <Quickdraw.h>

#define TRUE 1
#define FALSE 0

#define	kPutInFront			(WindowPtr)-1L
#define	kNormalUpdates		TRUE

#define kHelpKeyASCII				0x05
#define kPageUpKeyASCII				0x0B
#define kPageDownKeyASCII			0x0C
#define	kUpArrowKeyASCII			0x1E
#define kDownArrowKeyASCII			0x1F


#define kDownArrowKeyMap			122		// key map offset for down arrow
#define kRightArrowKeyMap			123		// key map offset for right arrow
#define kLeftArrowKeyMap			124		// key map offset for left arrow

#define kAKeyMap					7
#define	kEKeyMap					9
#define	kPKeyMap					36
#define	kQKeyMap					11
#define kSKeyMap					6
#define kColonMap					0x2E
#define kQuoteMap					0x20
#define	kCommandKeyMap				48
#define	kEscKeyMap					50
#define kSpaceBarMap				54

#define kBirdSound					1
#define kBirdPriority					80
#define kBonusSound					2
#define kBonusPriority					85
#define kBoom1Sound					3
#define kBoom1Priority					115
#define kBoom2Sound					4
#define kBoom2Priority					110
#define kSplashSound				5
#define kSplashPriority					75
#define kFlapSound					6
#define kFlapPriority					70
#define kGrateSound					8
#define kGratePriority					40
#define kLightningSound				9
#define kLightningPriority				100
#define kMusicSound					10
#define kMusicPriority					120
#define kScreechSound				12
#define kScreechPriority				50
#define kSpawnSound					13
#define kSpawnPriority					90
#define kWalkSound					14
#define kWalkPriority					30
#define kFlap2Sound					15
#define kFlap2Priority					20
#define kScrape2Sound				16
#define kScrape2Priority				10

#define kLavaHeight					456
#define kRoofHeight					2
#define kGravity					4

#define kIdle						-1	// enemy & player mode
#define kFlying						0	// enemy & player mode
#define kWalking					1	// enemy & player mode
#define kSinking					2	// player mode
#define kSpawning					3	// enemy mode
#define kFalling					4	// enemy mode & player mode
#define kEggTimer					5	// enemy mode
#define kDeadAndGone				6	// enemy mode
#define kBones						7	// player mode
#define kLurking					10	// hand mode
#define kOutGrabeth					11	// hand mode
#define kClutching					12	// hand mode
#define kWaiting					15	// eye mode
#define kStalking					16	// eye mode

#define kInitNumLives				5
#define kMaxEnemies					8
#define kDontFlapVel				8

#define kOwl						0
#define kWolf						1
#define kJackal						2

#define kMaxPixelShatter		4000
#define kMaxScoreFloater		   8
//--------------------------------------------------------------  Structs


typedef struct
{
	Rect		dest, wasDest, wrap;
	short		h, v;
	short		wasH, wasV;
	short		hVel, vVel;
	short		srcNum, mode;
	short		frame;
	Boolean		facingRight, flapping;
	Boolean		walking, wrapping;
	Boolean		clutched;
	short		electrical;		// should we be hitting him with lightning ? 
} playerType;

typedef struct
{
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

typedef struct
{
	Rect		dest;
	short		mode;
} handInfo;

typedef struct
{
	Rect		dest;
	short		mode, opening;
	short		srcNum, frame;
	Boolean		killed, entering;
} eyeInfo;

typedef struct
{
	short		prefVersion, filler;
	Str255		highName;
	Str15		highNames[10];
	long		highScores[10];
	short		highLevel[10];
	long		wasVolume;
} prefsInfo;

typedef struct
{
	unsigned long score;
	Point location;
	short duration;
} scoreFloaterType;

#define kShatterNormal 				0
#define kShatterPlayerDeath			1
#define kShatterEnemyDeath			2
#define kShatterPlatformHit			3
#define kShatterPlatformLightning	4
#define kShatterPlatformScrape		5
#define kShatterLavaBubble			6
#define kShatterLavaSplash			7
#define kShatterLavaBurst			8
#define kShatterPlayerEnemyScrape	9
#define kShatterObeliskTip			10
#define kShatterLightningDust		11

typedef struct
{
	short h,v;
	short hVel, vVel;
	short type;
	short duration;
	unsigned char color;
} pixelShatter;

//--------------------------------------------------------------  Prototypes


void GenerateEnemies (void);			// Enemies.c
void MoveEnemies (void);
void InitHandLocation (void);
void HandleHand (void);
void InitEye (void);
void KillOffEye (void);
void HandleEye (void);
void CheckPlayerEnemyCollision (void);
void HandlePixelShatter(void);
void StartPixelShatter(short h, short v, short hVel, short vVel, short type);
void StartPixelShatterRect(Rect *r, short dH, short dV, short type);

void DrawPlatforms (short);				// Graphics.c
void DrawBanner(void);
void DrawPlayer(void);
void ScrollHelp (short);
void OpenHelp (void);
void CloseWall (void);
void OpenHighScores (void);
void UpdateLivesNumbers (void);
void UpdateScoreNumbers (void);
void UpdateLevelNumbers (void);
void GenerateLightning (short h, short v);
void FlashObelisks (Boolean);
void DrawObelisks (void);
void StrikeLightningWork (void);
void DumpBackToWorkMap (void);
void DrawTorches (void);
void DrawPixelShatter (void);
void DrawFrame (void);
void GameQuitGraphics(void);

void MenusReflectMode (void);			// Interface.c
void DoMenuChoice (long);
void HandleEvent (void);

void InitNewGame (void);				// Play.c
void PlayGame (void);

Boolean SavePrefs (prefsInfo *, short);	// Prefs.c
Boolean LoadPrefs (prefsInfo *, short);


void ToolBoxInit (void);				// SetUpTakeDown.c
void CheckEnvirons (void);
void OpenMainWindow (void);
void InitMenubar (void);
void InitVariables (void);
void ShutItDown (void);

void PlayExternalSound (short, short);	// Sound.c
void InitSound (void);
void KillSound (void);

short RandomInt (short);				// Utilities.c
void RedAlert (StringPtr);
void FindOurDevice (void);
void LoadGraphic (short);
void CreateOffScreenPixMap (Rect *, CGrafPtr *);
void CreateOffScreenBitMap (Rect *, GrafPtr *);
void ZeroRectCorner (Rect *);
void FlashShort (short);
void LogNextTick (long);
void WaitForNextTick (void);
Boolean TrapExists (short);
Boolean DoWeHaveGestalt (void);
void CenterAlert (short);
short RectWide (Rect *);
short RectTall (Rect *);
void CenterRectInRect (Rect *, Rect *);
void PasStringCopy (StringPtr, StringPtr);
void CenterDialog (short);
void DrawDefaultButton (DialogPtr);
void PasStringCopyNum (StringPtr, StringPtr, short);
void GetDialogString (DialogPtr, short, StringPtr);
void SetDialogString (DialogPtr, short, StringPtr);
void SetDialogNumToStr (DialogPtr, short, long );
void GetDialogNumFromStr (DialogPtr, short, long *);
void DisableControl (DialogPtr, short);
void GameIdleAnimation(void);

// pixel shatter control variables
extern short numPixelShatter;
extern pixelShatter thePixelShatter[kMaxPixelShatter];

// score floater control variables
void HandleScoreFloaters(void);
void StartScoreFloater(unsigned long score, Point where);
extern short numScoreFloater;
extern scoreFloaterType theScoreFloater[kMaxScoreFloater];

// lightning control variables
extern short lightH, lightV, lightningCount;

#ifdef __powerc
	extern DSpContextReference gTheContext;
#endif

#ifdef powerc
	extern pascal void SetSoundVol(short level);		// for old Sound Manager
	extern pascal void GetSoundVol(short *level)
	THREEWORDINLINE(0x4218, 0x10B8, 0x0260);
#endif


