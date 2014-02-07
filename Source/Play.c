
//============================================================================
//----------------------------------------------------------------------------
//									Play.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"

#define kFlapImpulse			48
#define kGlideImpulse			12
#define kAirResistance			2
#define kMaxHVelocity			192
#define kMaxVVelocity			512
#define kNumLightningStrikes	5

extern Boolean canPlay;

void SetUpLevel (void);
void ResetPlayer (Boolean);
void OffAMortal (void);
void DoCommandKey (void);
void GetPlayerInput (void);
void HandlePlayerIdle (void);
void HandlePlayerFlying (void);
void HandlePlayerWalking (void);
void HandlePlayerSinking (void);
void HandlePlayerFalling (void);
void HandlePlayerBones (void);
void MovePlayer (void);
void CheckTouchDownCollision (void);
void CheckPlatformCollision (void);
void KeepPlayerOnPlatform (void);
void CheckLavaRoofCollision (void);
void SetAndCheckPlayerDest (void);
void HandleLightning (void);
void FinishLightning (void);
void HandleCountDownTimer (void);
void CheckHighScore (void);


playerType	thePlayer;
enemyType	theEnemies[kMaxEnemies];
KeyMap		theKeys;
Rect		platformRects[6], touchDownRects[6], enemyRects[24];
Rect		enemyInitRects[5];
long		theScore, wasTensOfThousands;
short		numLedges, beginOnLevel, levelOn, livesLeft, lightH, lightV;
short		lightningCount, numEnemies, countDownTimer;
Boolean		playing, pausing, flapKeyDown, evenFrame;
Boolean		doEnemyFlapSound, doEnemyScrapeSound;

extern	handInfo	theHand;
extern	prefsInfo	thePrefs;
extern	Rect		playerRects[11], mainWindowRect;
extern	short		numUpdateRects1, numUpdateRects2, numOwls;
extern	Boolean		quitting, openTheScores;
extern	WindowPtr	mainWindow;


//==============================================================  Functions
//--------------------------------------------------------------  InitNewGame

void InitNewGame()
{
	countDownTimer = 0;
	numLedges = 3;
	beginOnLevel = 1;
	levelOn = 0;
	livesLeft = kInitNumLives;
	theScore = 0L;
	playing = TRUE;
	pausing = FALSE;
	evenFrame = TRUE;
	wasTensOfThousands = 0L;
	numOwls = 4;
	
	numUpdateRects1 = 0;
	numUpdateRects2 = 0;
	
	InitHandLocation();
	theHand.mode = kLurking;
	
	SetUpLevel();
	
	DumpBackToWorkMap();
	
	UpdateLivesNumbers();
	UpdateScoreNumbers();
	UpdateLevelNumbers();
	
	GenerateEnemies();
	ResetPlayer(TRUE);
}

//--------------------------------------------------------------  SetUpLevel

void SetUpLevel()
{
	short wasLedges, waveMultiple;
	
	KillOffEye();
	
	wasLedges = numLedges;
	waveMultiple = levelOn % 5;
	
	switch (waveMultiple) {
		case 0:
			numLedges = 5;
			break;
			
		case 1:
			numLedges = 6;
			break;
			
		case 2:
			numLedges = 5;
			break;
			
		case 3:
			numLedges = 3;
			break;
			
		case 4:
			numLedges = 6;
			break;
	}
	
	if (wasLedges != numLedges)
		DrawPlatforms(numLedges);
	
	FlashObelisks(TRUE);
	
	GenerateLightning(320, 429);	// platform 0
	StrikeLightning();
	LogNextTick(2);
	WaitForNextTick();
	StrikeLightning();
	
	GenerateLightning(95, 289);		// platform 1
	StrikeLightning();
	LogNextTick(2);
	WaitForNextTick();
	StrikeLightning();
	
	GenerateLightning(95, 110);		// platform 3
	StrikeLightning();
	LogNextTick(2);
	WaitForNextTick();
	StrikeLightning();
	
	GenerateLightning(320, 195);	// platform 5
	StrikeLightning();
	LogNextTick(2);
	WaitForNextTick();
	StrikeLightning();
	
	GenerateLightning(545, 110);	// platform 4
	StrikeLightning();
	LogNextTick(2);
	WaitForNextTick();
	StrikeLightning();
	
	GenerateLightning(545, 289);	// platform 2
	StrikeLightning();
	LogNextTick(2);
	WaitForNextTick();
	StrikeLightning();
	
	FlashObelisks(FALSE);
	PlayExternalSound(kLightningSound, kLightningPriority);
	
	UpdateLevelNumbers();
}

//--------------------------------------------------------------  ResetPlayer

void ResetPlayer(Boolean initialPlace)
{
	short location;
	
	thePlayer.srcNum = 5;
	thePlayer.frame = 320;
	
	if (initialPlace)
		location = 0;
	else
		location = RandomInt(numLedges);
	
	switch (location) {
		case 0:
			thePlayer.h = 296 << 4;		// bottom center
			thePlayer.v = 377 << 4;
			break;
			
		case 1:
			thePlayer.h = 102 << 4;
			thePlayer.v = 237 << 4;
			break;
			
		case 2:
			thePlayer.h = 489 << 4;
			thePlayer.v = 237 << 4;
			break;
			
		case 3:
			thePlayer.h = 102 << 4;
			thePlayer.v = 58 << 4;
			break;
			
		case 4:
			thePlayer.h = 489 << 4;
			thePlayer.v = 58 << 4;
			break;
			
		case 5:
			thePlayer.h = 296 << 4;
			thePlayer.v = 143 << 4;
			break;
	}
	
	thePlayer.dest = playerRects[thePlayer.srcNum];
	ZeroRectCorner(&thePlayer.dest);
	OffsetRect(&thePlayer.dest, thePlayer.h >> 4, thePlayer.v >> 4);
	thePlayer.wasDest = thePlayer.dest;
	
	thePlayer.hVel = 0;
	thePlayer.vVel = 0;
	thePlayer.facingRight = TRUE;
	thePlayer.flapping = FALSE;
	thePlayer.wrapping = FALSE;
	thePlayer.clutched = FALSE;
	thePlayer.mode = kIdle;
	if (lightningCount == 0) {
		lightH = thePlayer.dest.left + 24;
		lightV = thePlayer.dest.bottom - 24;
		lightningCount = kNumLightningStrikes;
	}
}

//--------------------------------------------------------------  OffAMortal

void OffAMortal (void)
{
	livesLeft--;
	
	if (livesLeft > 0)
	{
		ResetPlayer(FALSE);
		UpdateLivesNumbers();
	}
	else
	{
		playing = FALSE;
	}
}

//--------------------------------------------------------------  DoCommandKey

void DoCommandKey()
{
	if (BitTst(&theKeys, kEKeyMap)) {
		playing = FALSE;
	} else if (BitTst(&theKeys, kPKeyMap)) {
		pausing = TRUE;
		MenusReflectMode();
		DumpMainToWorkMap();
		FlushEvents(everyEvent, 0);
	} else if (BitTst(&theKeys, kQKeyMap)) {
		playing = FALSE;
		quitting = TRUE;
	}
}

//--------------------------------------------------------------  GetPlayerInput

void GetPlayerInput()
{
	thePlayer.flapping = FALSE;
	thePlayer.walking = FALSE;
	
	GetKeys(theKeys);
	if (BitTst(&theKeys, kCommandKeyMap)) {
		DoCommandKey();
	}
	else
	{
		if ((BitTst(&theKeys, kSpaceBarMap)) || (BitTst(&theKeys, kDownArrowKeyMap)))
		{
			if (thePlayer.mode == kIdle)
			{
				thePlayer.mode = kWalking;
				thePlayer.frame = 0;
			}
			else if ((thePlayer.mode == kFlying) || (thePlayer.mode == kWalking))
			{
				if (!flapKeyDown)
				{
					thePlayer.vVel -= kFlapImpulse;
					flapKeyDown = TRUE;
					PlayExternalSound(kFlapSound, kFlapPriority);
					thePlayer.flapping = TRUE;
				}
			}
		}
		else
			flapKeyDown = FALSE;
		
		if ((BitTst(&theKeys, kRightArrowKeyMap) ||
			 BitTst(&theKeys, kSKeyMap) ||
			 BitTst(&theKeys, kQuoteMap)) &&
			(thePlayer.hVel < kMaxHVelocity))
		{
			if (thePlayer.mode == kIdle)
			{
				thePlayer.mode = kWalking;
				thePlayer.frame = 0;
			}
			else if ((thePlayer.mode == kFlying) || (thePlayer.mode == kWalking))
			{
				if (!thePlayer.facingRight)
				{
					thePlayer.facingRight = TRUE;
					if (thePlayer.clutched)
					{
						thePlayer.dest.left += 18;
						thePlayer.dest.right += 18;
						thePlayer.h = thePlayer.dest.left << 4;
						thePlayer.wasH = thePlayer.h;
						thePlayer.wasDest = thePlayer.dest;
					}
				}
				else
				{
					if (thePlayer.mode == kFlying)
						thePlayer.hVel += kGlideImpulse;
					else
						thePlayer.walking = TRUE;
				}
			}
		}
		else if ((BitTst(&theKeys, kLeftArrowKeyMap) ||
				  BitTst(&theKeys, kAKeyMap) ||
				  BitTst(&theKeys, kColonMap)) &&
				 (thePlayer.hVel > -kMaxHVelocity))
		{
			if (thePlayer.mode == kIdle)
			{
				thePlayer.mode = kWalking;
				thePlayer.frame = 0;
			}
			else if ((thePlayer.mode == kFlying) || (thePlayer.mode == kWalking))
			{
				if (thePlayer.facingRight)
				{
					thePlayer.facingRight = FALSE;
					if (thePlayer.clutched)
					{
						thePlayer.dest.left -= 18;
						thePlayer.dest.right -= 18;
						thePlayer.h = thePlayer.dest.left << 4;
						thePlayer.wasH = thePlayer.h;
						thePlayer.wasDest = thePlayer.dest;
					}
				}
				else
				{
					if (thePlayer.mode == kFlying)
						thePlayer.hVel -= kGlideImpulse;
					else
						thePlayer.walking = TRUE;
				}
			}
		}
	}
}

//--------------------------------------------------------------  HandlePlayerIdle

void HandlePlayerIdle (void)
{
	thePlayer.frame--;
	if (thePlayer.frame == 0)
		thePlayer.mode = kWalking;
	
	SetAndCheckPlayerDest();
}

//--------------------------------------------------------------  HandlePlayerFlying

void HandlePlayerFlying (void)
{
	if (thePlayer.hVel > 0)
	{
		thePlayer.hVel -= kAirResistance;
		if (thePlayer.hVel < 0)
			thePlayer.hVel = 0;
	}
	else if (thePlayer.hVel < 0)
	{
		thePlayer.hVel += kAirResistance;
		if (thePlayer.hVel > 0)
			thePlayer.hVel = 0;
	}
	
	thePlayer.vVel += kGravity;
	
	if (thePlayer.vVel > kMaxVVelocity)
		thePlayer.vVel = kMaxVVelocity;
	else if (thePlayer.vVel < -kMaxVVelocity)
		thePlayer.vVel = -kMaxVVelocity;
	
	thePlayer.h += thePlayer.hVel;
	thePlayer.v += thePlayer.vVel;
	
	if (thePlayer.facingRight)
	{
		thePlayer.srcNum = 1;
		if (thePlayer.vVel < -kDontFlapVel)
		{
			if (thePlayer.flapping)
				thePlayer.srcNum = 0;
			else
				thePlayer.srcNum = 1;
		}
		else if (thePlayer.vVel > kDontFlapVel)
		{
			if (thePlayer.flapping)
				thePlayer.srcNum = 1;
			else
				thePlayer.srcNum = 0;
		}
		else if (thePlayer.flapping)
			thePlayer.srcNum = 0;
	}
	else
	{
		thePlayer.srcNum = 2;
		if (thePlayer.vVel < -kDontFlapVel)
		{
			if (thePlayer.flapping)
				thePlayer.srcNum = 3;
			else
				thePlayer.srcNum = 2;
		}
		else if (thePlayer.vVel > kDontFlapVel)
		{
			if (thePlayer.flapping)
				thePlayer.srcNum = 2;
			else
				thePlayer.srcNum = 3;
		}
		else if (thePlayer.flapping)
			thePlayer.srcNum = 3;
	}
	
	SetAndCheckPlayerDest();
	
	CheckLavaRoofCollision();
	CheckPlayerEnemyCollision();
	CheckPlatformCollision();
	CheckTouchDownCollision();
}

//--------------------------------------------------------------  HandlePlayerWalking

void HandlePlayerWalking (void)
{
	short		desiredHVel;
	
	if (thePlayer.walking)
	{
		if (evenFrame)
		{
			if (thePlayer.facingRight)
			{
				if (thePlayer.srcNum == 4)
					desiredHVel = 208;
				else
					desiredHVel = 128;
			}
			else
			{
				if (thePlayer.srcNum == 7)
					desiredHVel = -208;
				else
					desiredHVel = -128;
			}
			
			if (thePlayer.hVel < desiredHVel)
			{
				thePlayer.hVel += 80;
				if (thePlayer.hVel > desiredHVel)
				{
					thePlayer.hVel = desiredHVel;
					PlayExternalSound(kWalkSound, kWalkPriority);
				}
				else
					PlayExternalSound(kScreechSound, kScreechPriority);
			}
			else
			{
				thePlayer.hVel -= 80;
				if (thePlayer.hVel < desiredHVel)
				{
					thePlayer.hVel = desiredHVel;
					PlayExternalSound(kWalkSound, kWalkPriority);
				}
				else
					PlayExternalSound(kScreechSound, kScreechPriority);
			}
		}
	}
	else
	{
		thePlayer.hVel -= thePlayer.hVel / 4;
		if ((thePlayer.hVel < 4) && (thePlayer.hVel > -4))
			thePlayer.hVel = 0;
		else
			PlayExternalSound(kScreechSound, kScreechPriority);
	}
	
	if (thePlayer.vVel > kMaxVVelocity)
		thePlayer.vVel = kMaxVVelocity;
	else if (thePlayer.vVel < -kMaxVVelocity)
		thePlayer.vVel = -kMaxVVelocity;
	
	thePlayer.h += thePlayer.hVel;
	thePlayer.v += thePlayer.vVel;
	
	if (thePlayer.walking) {
		if (evenFrame) {
			if (thePlayer.facingRight)
				thePlayer.srcNum = 9 - thePlayer.srcNum;
			else
				thePlayer.srcNum = 13 - thePlayer.srcNum;
		}
	} else {
		if (thePlayer.facingRight)
			thePlayer.srcNum = 5;
		else
			thePlayer.srcNum = 6;
	}
	
	SetAndCheckPlayerDest();
	
	CheckTouchDownCollision();
	KeepPlayerOnPlatform();
	CheckPlayerEnemyCollision();
}

//--------------------------------------------------------------  HandlePlayerSinking

void HandlePlayerSinking()
{
	thePlayer.hVel = 0;
	thePlayer.vVel = 16;
	if (thePlayer.dest.top > kLavaHeight)
		OffAMortal();
	
	thePlayer.v += thePlayer.vVel;
	
	SetAndCheckPlayerDest();
}

//--------------------------------------------------------------  HandlePlayerFalling

void HandlePlayerFalling()
{
	if (thePlayer.hVel > 0) {
		thePlayer.hVel -= kAirResistance;
		if (thePlayer.hVel < 0)
			thePlayer.hVel = 0;
	} else if (thePlayer.hVel < 0) {
		thePlayer.hVel += kAirResistance;
		if (thePlayer.hVel > 0)
			thePlayer.hVel = 0;
	}
	
	thePlayer.vVel += kGravity;
	
	if (thePlayer.vVel > kMaxVVelocity)
		thePlayer.vVel = kMaxVVelocity;
	else if (thePlayer.vVel < -kMaxVVelocity)
		thePlayer.vVel = -kMaxVVelocity;
	
	thePlayer.h += thePlayer.hVel;
	thePlayer.v += thePlayer.vVel;
	
	SetAndCheckPlayerDest();
	
	CheckLavaRoofCollision();
	CheckPlatformCollision();
}

//--------------------------------------------------------------  HandlePlayerBones

void HandlePlayerBones()
{
	if (evenFrame) {
		thePlayer.frame--;
		if (thePlayer.frame == 0)
			OffAMortal();
		else
			thePlayer.dest.top = thePlayer.dest.bottom - thePlayer.frame;
	}
}

//--------------------------------------------------------------  MovePlayer

void MovePlayer()
{
	switch (thePlayer.mode) {
		case kIdle:
			HandlePlayerIdle();
			break;
			
		case kFlying:
			HandlePlayerFlying();
			break;
			
		case kWalking:
			HandlePlayerWalking();
			break;
			
		case kSinking:
			HandlePlayerSinking();
			break;
			
		case kFalling:
			HandlePlayerFalling();
			break;
			
		case kBones:
			HandlePlayerBones();
			break;
	}
}

//--------------------------------------------------------------  CheckTouchDownCollision

void CheckTouchDownCollision()
{
	Rect	testRect, whoCares;
	short	i, offset;
	Boolean	sected;
	
	sected = FALSE;
	for (i = 0; i < numLedges; i++)
	{
		testRect = touchDownRects[i];
		if (thePlayer.mode == kWalking)
			OffsetRect(&testRect, 0, 11);
		
		if (SectRect(&thePlayer.dest, &testRect, &whoCares))
		{
			if (thePlayer.mode == kFlying)
			{
				thePlayer.mode = kWalking;
				if (thePlayer.facingRight)
					thePlayer.srcNum = 5;
				else
					thePlayer.srcNum = 6;
				if (thePlayer.vVel > 0)
					thePlayer.vVel = 0;
				thePlayer.dest.bottom += 11;
				thePlayer.wasDest.bottom += 11;
				offset = thePlayer.dest.bottom - testRect.bottom - 1;
				thePlayer.dest.bottom -= offset;
				thePlayer.dest.top -= offset;
				thePlayer.v = thePlayer.dest.top << 4;
				PlayExternalSound(kGrateSound, kGratePriority);
			}
			sected = TRUE;
		}
	}
	
	if (!sected) {
		if (thePlayer.mode == kWalking) {
			thePlayer.mode = kFlying;
			thePlayer.dest.bottom -= 11;
			thePlayer.wasDest.bottom -= 11;
		}
	}
}

//--------------------------------------------------------------  CheckPlatformCollision

void CheckPlatformCollision()
{
	Rect	hRect, vRect, whoCares;
	short	i, offset;
	
	for (i = 0; i < numLedges; i++) {
		if (SectRect(&thePlayer.dest, &platformRects[i], &whoCares))
		{
			hRect.left = thePlayer.dest.left;
			hRect.right = thePlayer.dest.right;
			hRect.top = thePlayer.wasDest.top;
			hRect.bottom = thePlayer.wasDest.bottom;
			
			if (SectRect(&hRect, &platformRects[i], &whoCares))
			{
				if (thePlayer.h > thePlayer.wasH)			// heading right
				{
					offset = thePlayer.dest.right - platformRects[i].left;
					thePlayer.dest.left -= offset;
					thePlayer.dest.right -= offset;
					thePlayer.h = thePlayer.dest.left << 4;
					if (thePlayer.hVel > 0)
						thePlayer.hVel = -(thePlayer.hVel >> 1);
					else
						thePlayer.hVel = thePlayer.hVel >> 1;
				}
				else if (thePlayer.h < thePlayer.wasH)		// heading left
				{
					offset = platformRects[i].right - thePlayer.dest.left;
					thePlayer.dest.left += offset;
					thePlayer.dest.right += offset;
					thePlayer.h = thePlayer.dest.left << 4;
					if (thePlayer.hVel < 0)
						thePlayer.hVel = -(thePlayer.hVel >> 1);
					else
						thePlayer.hVel = thePlayer.hVel >> 1;
				}
				PlayExternalSound(kGrateSound, kGratePriority);
			} else {
				vRect.left = thePlayer.wasDest.left;
				vRect.right = thePlayer.wasDest.right;
				vRect.top = thePlayer.dest.top;
				vRect.bottom = thePlayer.dest.bottom;
				
				if (SectRect(&vRect, &platformRects[i], &whoCares)) {
					if (thePlayer.wasV < thePlayer.v)		// heading down
					{
						offset = thePlayer.dest.bottom - platformRects[i].top;
						thePlayer.dest.top -= offset;
						thePlayer.dest.bottom -= offset;
						thePlayer.v = thePlayer.dest.top << 4;
						if (thePlayer.vVel > kDontFlapVel)
							PlayExternalSound(kGrateSound, kGratePriority);
						if (thePlayer.mode == kFalling) {
							if ((thePlayer.dest.right - 16) > platformRects[i].right) {
								thePlayer.hVel = 16;
								if (thePlayer.vVel > 0)
									thePlayer.vVel = -(thePlayer.vVel >> 1);
								else
									thePlayer.vVel = thePlayer.vVel >> 1;
							} else if ((thePlayer.dest.left + 16) < platformRects[i].left) {
								thePlayer.hVel = -16;
								if (thePlayer.vVel > 0)
									thePlayer.vVel = -(thePlayer.vVel >> 1);
								else
									thePlayer.vVel = thePlayer.vVel >> 1;
							} else {
								PlayExternalSound(kBoom1Sound, kBoom1Priority);
								thePlayer.vVel = 0;
								thePlayer.mode = kBones;
								thePlayer.frame = 22;
								thePlayer.dest.top = thePlayer.dest.bottom - 22;
								thePlayer.v = thePlayer.dest.top << 4;
								thePlayer.srcNum = 10;
							}
						} else {
							if (thePlayer.vVel > 0)
								thePlayer.vVel = -(thePlayer.vVel >> 1);
							else
								thePlayer.vVel = thePlayer.vVel >> 1;
						}
					}
					else if (thePlayer.wasV > thePlayer.v)	// heading up
					{
						offset = platformRects[i].bottom - thePlayer.dest.top;
						thePlayer.dest.top += offset;
						thePlayer.dest.bottom += offset;
						thePlayer.v = thePlayer.dest.top << 4;
						PlayExternalSound(kGrateSound, kGratePriority);
						if (thePlayer.vVel < 0)
							thePlayer.vVel = -(thePlayer.vVel >> 1);
						else
							thePlayer.vVel = thePlayer.vVel >> 1;
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------  KeepPlayerOnPlatform

void KeepPlayerOnPlatform (void)
{
	Rect	whoCares;
	short	i, offset;
	
	for (i = 0; i < numLedges; i++)
	{
		if ((SectRect(&thePlayer.dest, &platformRects[i], &whoCares)) && (thePlayer.vVel > 0))
		{
			offset = thePlayer.dest.bottom - platformRects[i].top - 1;
			thePlayer.dest.top -= offset;
			thePlayer.dest.bottom -= offset;
			thePlayer.v = thePlayer.dest.top * 16;
		}
	}
	
	if (thePlayer.vVel > 0)
		thePlayer.vVel = 0;
}

//--------------------------------------------------------------  CheckLavaRoofCollision

void CheckLavaRoofCollision (void)
{
	short		offset;
	
	if (thePlayer.dest.bottom > kLavaHeight) {
		if (thePlayer.mode == kFalling)
			PlayExternalSound(kSplashSound, kSplashPriority);
		else
			PlayExternalSound(kBirdSound, kBirdPriority);
		thePlayer.mode = kSinking;
	} else if (thePlayer.dest.top < kRoofHeight) {
		offset = kRoofHeight - thePlayer.dest.top;
		thePlayer.dest.top += offset;
		thePlayer.dest.bottom += offset;
		thePlayer.v = thePlayer.dest.top * 16;
		PlayExternalSound(kGrateSound, kGratePriority);
		thePlayer.vVel = thePlayer.vVel / -2;
	}
}

//--------------------------------------------------------------  SetAndCheckPlayerDest

void SetAndCheckPlayerDest()
{
	short wasTall, wasWide;
	
	wasTall = thePlayer.dest.bottom - thePlayer.dest.top;
	wasWide = thePlayer.dest.right - thePlayer.dest.left;
	
	thePlayer.dest.left = thePlayer.h >> 4;
	thePlayer.dest.right = thePlayer.dest.left + wasWide;
	thePlayer.dest.top = thePlayer.v >> 4;
	thePlayer.dest.bottom = thePlayer.dest.top + wasTall;
	
	if (thePlayer.dest.left > 640) {
		OffsetRect(&thePlayer.dest, -640, 0);
		thePlayer.h = thePlayer.dest.left << 4;
		OffsetRect(&thePlayer.wasDest, -640, 0);
	} else if (thePlayer.dest.right < 0) {
		OffsetRect(&thePlayer.dest, 640, 0);
		thePlayer.h = thePlayer.dest.left << 4;
		OffsetRect(&thePlayer.wasDest, 640, 0);
	}
}

//--------------------------------------------------------------  HandleLightning

void HandleLightning()
{
	if (lightningCount > 0) {
		if (lightningCount == kNumLightningStrikes)
			FlashObelisks(TRUE);
		GenerateLightning(lightH, lightV);
		StrikeLightning();
	}
}

//--------------------------------------------------------------  FinishLightning

void FinishLightning()
{
	if (lightningCount > 0) {
		StrikeLightning();
		lightningCount--;
		if (lightningCount == 0)
			FlashObelisks(FALSE);
		PlayExternalSound(kLightningSound, kLightningPriority);
	}
}

//--------------------------------------------------------------  HandleCountDownTimer

void HandleCountDownTimer()
{
	if (countDownTimer == 0)
		return;
	else {
		countDownTimer--;
		if (countDownTimer == 0) {
			countDownTimer = 0;
			levelOn++;
			UpdateLevelNumbers();
			SetUpLevel();
			GenerateEnemies();
		}
	}
}

//--------------------------------------------------------------  PlayGame

void PlayGame()
{
#define		kTicksPerFrame		2
	long		waitUntil;
	EventRecord theEvent;
	short		thePart;
	BitMap		screenbits;
	WindowPtr	whichWindow;
	long		menuChoice;
	
	
	GetQDGlobalsScreenBits(&screenbits);
#ifdef OBSCURE_CURSOR_DURING_PLAY
	ObscureCursor();
#endif
	waitUntil = TickCount() + kTicksPerFrame;
	
	do {
		MovePlayer();
		MoveEnemies();
		HandleHand();
		HandleEye();
		DrawFrame();
		HandleLightning();
		do {
			// See if we switched out
			if (WaitNextEvent(everyEvent, &theEvent, 0, 0L)) {
				switch (theEvent.what) {
					case updateEvt:
						BeginUpdate(mainWindow);
						DrawFrame();
						EndUpdate(mainWindow);
						break;
					case mouseDown:
						thePart = FindWindow(theEvent.where, &whichWindow);
						switch (thePart)
					{
						case inMenuBar:
							pausing = TRUE;
							MenusReflectMode();
							DumpMainToWorkMap();
							FlushEvents(everyEvent, 0);
							thePart = FindWindow(theEvent.where, &whichWindow);
							menuChoice = MenuSelect(theEvent.where);
							if (canPlay)
								DoMenuChoice(menuChoice);
							break;
						case inDrag:
							GetQDGlobalsScreenBits(&screenbits);
							DragWindow(whichWindow, theEvent.where, &screenbits.bounds);
							break;
					}
						break;
					case kHighLevelEvent:
						AEProcessAppleEvent(&theEvent);
						break;
					case osEvt:
						switch((theEvent.message >> 24) & 0x000000FF)
					{
						case suspendResumeMessage:
							if ((theEvent.message & resumeFlag) == 0) {
								pausing = TRUE;
								ShowCursor();
								MenusReflectMode();
								DumpMainToWorkMap();
								FlushEvents(everyEvent, 0);
							}
							HiliteMenu(0);
							break;
					}
						break;
				}
			}
		} while (TickCount() < waitUntil);
		waitUntil = TickCount() + kTicksPerFrame;
		evenFrame = !evenFrame;
		
		GetPlayerInput();
		HandleCountDownTimer();
		FinishLightning();
	}
	while ((playing) && (!pausing));
	
	if ((!playing) && (!quitting)) {
		PlayExternalSound(kMusicSound, kMusicPriority);
		CheckHighScore();
	}
	
	ShowCursor();
	MenusReflectMode();
	FlushEvents(everyEvent, 0);
}

//--------------------------------------------------------------  CheckHighScore

void CheckHighScore (void)
{
#define		kHighNameDialogID	130
	Str255		placeStr, tempStr;
	DialogPtr	theDial;
	short		i, item;
	Boolean		leaving;
	
	ShowCursor();
	if (theScore > thePrefs.highScores[9]) {
		openTheScores = TRUE;
		PlayExternalSound(kBonusSound, kMusicPriority - 1);
		i = 8;
		while ((theScore > thePrefs.highScores[i]) && (i >= 0))
		{
			thePrefs.highScores[i + 1] = thePrefs.highScores[i];
			thePrefs.highLevel[i + 1] = thePrefs.highLevel[i];
			PasStringCopy(thePrefs.highNames[i], thePrefs.highNames[i + 1]);
			i--;
		}
		
		i++;
		thePrefs.highScores[i] = theScore;
		thePrefs.highLevel[i] = levelOn + 1;
		
		LogNextTick(20);
		WaitForNextTick();
		theDial = GetNewDialog(kHighNameDialogID, 0L, kPutInFront);
		NumToString(i + 1, placeStr);
		ParamText(placeStr, "\p", "\p", "\p");
		SetDialogDefaultItem(theDial, 1);
		FlushEvents(everyEvent, 0);
		SetDialogString(theDial, 2, thePrefs.highName);
		leaving = FALSE;
		
		while (!leaving) {
			ModalDialog(NULL, &item);
			
			if (item == 1) {
				GetDialogString(theDial, 2, tempStr);
				PasStringCopyNum(tempStr, thePrefs.highNames[i], 15);
				PasStringCopy(thePrefs.highNames[i], thePrefs.highName);
				leaving = TRUE;
			}
		}
		
		DisposeDialog(theDial);
	} else
		openTheScores = FALSE;
}

