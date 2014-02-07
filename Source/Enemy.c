
//============================================================================
//----------------------------------------------------------------------------
//									Enemy.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"


#define kEnemyImpulse			8

#define kOwlMaxHVel				96
#define kOwlMaxVVel				320
#define kOwlHeightSmell			96
#define kOwlFlapImpulse			32

#define kWolfMaxHVel			128
#define kWolfMaxVVel			400
#define kWolfHeightSmell		160
#define kWolfFlapImpulse		48

#define kJackalMaxHVel			192
#define kJackalMaxVVel			512
#define kJackalHeightSmell		240
#define kJackalFlapImpulse		72


Boolean SetEnemyInitialLocation(Rect *);
void SetEnemyAttributes(short);
short AssignNewAltitude();
void InitEnemy(short, Boolean);
void CheckEnemyPlatformHit(short);
void CheckEnemyRoofCollision(short);
void HandleIdleEnemies(short);
void HandleFlyingEnemies(short);
void HandleWalkingEnemy(short);
void HandleSpawningEnemy(short);
void HandleFallingEnemy(short);
void HandleEggEnemy(short);
void ResolveEnemyPlayerHit(short);


		handInfo	theHand;
		eyeInfo		theEye;
		Rect		grabZone;
		short		deadEnemies, spawnedEnemies, numEnemiesThisLevel, numOwls;

extern	playerType	thePlayer;
extern	enemyType	theEnemies[kMaxEnemies];
extern	Rect		platformRects[6], enemyInitRects[5];
extern	long		theScore;
extern	short		numLedges, lightningCount, numEnemies, countDownTimer;
extern	short		levelOn, lightH, lightV;
extern	Boolean		evenFrame, doEnemyFlapSound, doEnemyScrapeSound;


//==============================================================  Functions
//--------------------------------------------------------------  SetEnemyInitialLocation

Boolean SetEnemyInitialLocation(Rect *theRect)
{
	short	where, possibilities;
	Boolean	facing;
	
	possibilities = numLedges - 1;
	where = RandomInt(possibilities);
	*theRect = enemyInitRects[where];
	
	switch (where) {
		case 0:
		case 2:
			facing = TRUE;
			break;
			
		case 3:
			if (RandomInt(2) == 0)
				facing = TRUE;
			else
				facing = FALSE;
			break;
			
		default:
			facing = FALSE;
			break;
	}
	
	if ((levelOn % 5) == 4) { // Egg Wave
		theRect->left += 12 + RandomInt(48) - 24;
		theRect->right = theRect->left + 24;
		theRect->top = theRect->bottom - 24;
	}
	
	return (facing);
}

//--------------------------------------------------------------  SetEnemyAttributes

void SetEnemyAttributes(short i)
{
	short		h;
	
	h = (theEnemies[i].dest.left + theEnemies[i].dest.right) >> 1;
	if (h < 320)
		theEnemies[i].facingRight = TRUE;
	else
		theEnemies[i].facingRight = FALSE;
	
	switch (theEnemies[i].kind) {
		case kOwl:
		if (theEnemies[i].facingRight)
			theEnemies[i].srcNum = 0;
		else
			theEnemies[i].srcNum = 2;
		theEnemies[i].maxHVel = kOwlMaxHVel;
		theEnemies[i].maxVVel = kOwlMaxVVel;
		theEnemies[i].heightSmell = kOwlHeightSmell;
		theEnemies[i].flapImpulse = kOwlFlapImpulse;
		break;
		
		case kWolf:
		if (theEnemies[i].facingRight)
			theEnemies[i].srcNum = 4;
		else
			theEnemies[i].srcNum = 6;
		theEnemies[i].maxHVel = kWolfMaxHVel;
		theEnemies[i].maxVVel = kWolfMaxVVel;
		theEnemies[i].heightSmell = kWolfHeightSmell;
		theEnemies[i].flapImpulse = kWolfFlapImpulse;
		break;
		
		case kJackal:
		if (theEnemies[i].facingRight)
			theEnemies[i].srcNum = 8;
		else
			theEnemies[i].srcNum = 10;
		theEnemies[i].maxHVel = kJackalMaxHVel;
		theEnemies[i].maxVVel = kJackalMaxVVel;
		theEnemies[i].heightSmell = kJackalHeightSmell;
		theEnemies[i].flapImpulse = kJackalFlapImpulse;
		break;
	}
}

//--------------------------------------------------------------  AssignNewAltitude

short AssignNewAltitude (void)
{
	short		which, altitude = 0;
	
	which = RandomInt(4);
	switch (which)
	{
		case 0:
		altitude = 65 << 4;
		break;
		
		case 1:
		altitude = 150 << 4;
		break;
		
		case 2:
		altitude = 245 << 4;
		break;
		
		case 3:
		altitude = 384 << 4;
		break;
	}
	
	return (altitude);
}

//--------------------------------------------------------------  InitEnemy

void InitEnemy (short i, Boolean reincarnated)
{
	Boolean		facing;
	
	if (spawnedEnemies < numEnemiesThisLevel)
	{
		facing = SetEnemyInitialLocation(&theEnemies[i].dest);
		theEnemies[i].wasDest = theEnemies[i].dest;
		theEnemies[i].h = theEnemies[i].dest.left << 4;
		theEnemies[i].v = theEnemies[i].dest.top << 4;
		theEnemies[i].wasH = theEnemies[i].h;
		theEnemies[i].wasV = theEnemies[i].v;
		theEnemies[i].targetAlt = theEnemies[i].v - (40 << 4);
		theEnemies[i].hVel = 0;
		theEnemies[i].vVel = 0;
		theEnemies[i].pass = 0;
		if ((levelOn % 5) == 4)			// Egg Wave
			theEnemies[i].mode = kEggTimer;
		else
			theEnemies[i].mode = kIdle;
		if (i < numOwls)
			theEnemies[i].kind = kOwl;
		else if (i > (numOwls + 6))
			theEnemies[i].kind = kJackal;
		else
			theEnemies[i].kind = kWolf;
		theEnemies[i].facingRight = facing;
		SetEnemyAttributes(i);
		
		if (reincarnated)
			theEnemies[i].frame = RandomInt(48) + 8 + (numOwls * 32);
		else
			theEnemies[i].frame = RandomInt(48) + 32 + (64 * i) + (numOwls * 32);
		
		if ((levelOn % 5) == 4)			// Egg Wave
			theEnemies[i].kind--;
		
		spawnedEnemies++;
	}
}

//--------------------------------------------------------------  GenerateEnemies

void GenerateEnemies (void)
{
	short		i;
	
	if ((levelOn % 5) == 4)			// Egg Wave
	{
		numEnemies = kMaxEnemies;
		numEnemiesThisLevel = numEnemies;
	}
	else
	{
		numEnemies = ((levelOn / 5) + 2) * 2;
		if (numEnemies > kMaxEnemies)
			numEnemies = kMaxEnemies;
		numEnemiesThisLevel = numEnemies * 2;
	}
	
	deadEnemies = 0;
	
	numOwls = 4 - ((levelOn + 2) / 5);
	if (numOwls < 0)
		numOwls = 0;
	
	spawnedEnemies = 0;
	
	for (i = 0; i < numEnemies; i++)
		InitEnemy(i, FALSE);
}

//--------------------------------------------------------------  CheckEnemyPlatformHit

void CheckEnemyPlatformHit (short h)
{
	Rect		hRect, vRect, whoCares;
	short		i, offset;
	
	for (i = 0; i < numLedges; i++)
	{
		if (SectRect(&theEnemies[h].dest, &platformRects[i], &whoCares))
		{
			hRect.left = theEnemies[h].dest.left;
			hRect.right = theEnemies[h].dest.right;
			hRect.top = theEnemies[h].wasDest.top;
			hRect.bottom = theEnemies[h].wasDest.bottom;
			
			if (SectRect(&hRect, &platformRects[i], &whoCares))
			{
				if (theEnemies[h].h > theEnemies[h].wasH)	// moving to right
				{
					offset = theEnemies[h].dest.right - platformRects[i].left;
					theEnemies[h].dest.left -= offset;
					theEnemies[h].dest.right -= offset;
					theEnemies[h].h = theEnemies[h].dest.left << 4;
					theEnemies[h].wasH = theEnemies[h].h;
					if (theEnemies[h].hVel > 0)
						theEnemies[h].hVel = -(theEnemies[h].hVel >> 1);
					else
						theEnemies[h].hVel = theEnemies[h].hVel >> 1;
				}
				if (theEnemies[h].h < theEnemies[h].wasH)	// moving to left
				{
					offset = platformRects[i].right - theEnemies[h].dest.left;
					theEnemies[h].dest.left += offset;
					theEnemies[h].dest.right += offset;
					theEnemies[h].h = theEnemies[h].dest.left << 4;
					theEnemies[h].wasH = theEnemies[h].h;
					if (theEnemies[h].hVel < 0)
						theEnemies[h].hVel = -(theEnemies[h].hVel >> 1);
					else
						theEnemies[h].hVel = theEnemies[h].hVel >> 1;
				}
				doEnemyScrapeSound = TRUE;
				theEnemies[h].facingRight = !theEnemies[h].facingRight;
			}
			else
			{
				vRect.left = theEnemies[h].wasDest.left;
				vRect.right = theEnemies[h].wasDest.right;
				vRect.top = theEnemies[h].dest.top;
				vRect.bottom = theEnemies[h].dest.bottom;
				
				if (SectRect(&vRect, &platformRects[i], &whoCares))
				{
					if (theEnemies[h].mode == kFalling)
					{
						theEnemies[i].hVel -= (theEnemies[i].hVel >> 3);
						if ((theEnemies[i].hVel < 8) && (theEnemies[i].hVel > -8))
						{
							if (theEnemies[i].hVel > 0)
								theEnemies[i].hVel--;
							else if (theEnemies[i].hVel < 0)
								theEnemies[i].hVel++;
						}
					}
					
					if (theEnemies[h].v > theEnemies[h].wasV)		// heading down
					{
						offset = theEnemies[h].dest.bottom - platformRects[i].top;
						theEnemies[h].dest.top -= offset;
						theEnemies[h].dest.bottom -= offset;
						theEnemies[h].v = theEnemies[h].dest.top << 4;
						theEnemies[h].wasV = theEnemies[h].v;
						if (theEnemies[h].vVel > kDontFlapVel)
							doEnemyScrapeSound = TRUE;
						if (theEnemies[h].vVel > 0)
							theEnemies[h].vVel = -(theEnemies[h].vVel >> 1);
						else
							theEnemies[h].vVel = theEnemies[h].vVel >> 1;
						if ((theEnemies[h].vVel < 8) && (theEnemies[h].vVel > -8) && 
								(theEnemies[h].hVel == 0) && (theEnemies[h].mode == kFalling))
						{
							if (((theEnemies[h].dest.right - 8) > platformRects[i].right) && 
									(theEnemies[h].hVel == 0))
							{				// if enemy has come to rest half off the edge…
								theEnemies[h].hVel = 32;
							}
							else if (((theEnemies[h].dest.left + 8) < platformRects[i].left) && 
									(theEnemies[h].hVel == 0))
							{
								theEnemies[h].hVel = -32;
							}
							else
							{
								theEnemies[h].mode = kEggTimer;
								theEnemies[h].frame = (numOwls * 96) + 128;
								theEnemies[h].vVel = 0;
							}
						}
					}
					if (theEnemies[h].v < theEnemies[h].wasV)		// heading up
					{
						offset = theEnemies[h].dest.top - platformRects[i].bottom;
						theEnemies[h].dest.top -= offset;
						theEnemies[h].dest.bottom -= offset;
						theEnemies[h].v = theEnemies[h].dest.top << 4;
						theEnemies[h].wasV = theEnemies[h].v;
						doEnemyScrapeSound = TRUE;
						if (theEnemies[h].vVel < 0)
							theEnemies[h].vVel = -(theEnemies[h].vVel >> 2);
						else
							theEnemies[h].vVel = theEnemies[h].vVel >> 2;
						if ((theEnemies[h].vVel < 8) && (theEnemies[h].vVel > -8) && 
								(theEnemies[h].hVel == 0) && (theEnemies[h].mode == kFalling))
						{
							theEnemies[h].mode = kEggTimer;
							theEnemies[h].frame = (numOwls * 96) + 128;
							theEnemies[h].vVel = 0;
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------  CheckEnemyRoofCollision

void CheckEnemyRoofCollision (short i)
{
	short		offset;
	
	if (theEnemies[i].dest.top < (kRoofHeight - 2))
	{
		offset = kRoofHeight - theEnemies[i].dest.top;
		theEnemies[i].dest.top += offset;
		theEnemies[i].dest.bottom += offset;
		theEnemies[i].v = theEnemies[i].dest.top << 4;
		doEnemyScrapeSound = TRUE;
		theEnemies[i].vVel = -(theEnemies[i].vVel >> 2);
	}
	else if (theEnemies[i].dest.top > kLavaHeight)
	{
		theEnemies[i].mode = kDeadAndGone;
		deadEnemies++;
		
		PlayExternalSound(kSplashSound, kSplashPriority);
		InitEnemy(i, TRUE);
	}
}

//--------------------------------------------------------------  HandleIdleEnemies

void HandleIdleEnemies (short i)
{
	theEnemies[i].frame--;
	if (theEnemies[i].frame <= 0)
	{
		theEnemies[i].mode = kSpawning;
		theEnemies[i].wasH = theEnemies[i].h;
		theEnemies[i].wasV = theEnemies[i].v;
		theEnemies[i].hVel = 0;
		theEnemies[i].vVel = 0;
		theEnemies[i].frame = 0;
		SetEnemyAttributes(i);
		PlayExternalSound(kSpawnSound, kSpawnPriority);
	}
}

//--------------------------------------------------------------  HandleFlyingEnemies

void HandleFlyingEnemies (short i)
{
	short		dist;
	Boolean		shouldFlap;
	
	theEnemies[i].vVel += kGravity;
	
	dist = thePlayer.dest.top - theEnemies[i].dest.top;
	if (dist < 0)
		dist = -dist;
	
	if ((dist < theEnemies[i].heightSmell) && 
			((thePlayer.mode == kFlying) || (thePlayer.mode == kWalking)))
	{							// enemy will actively seek the player
		if (thePlayer.dest.left < theEnemies[i].dest.left)
		{
			dist = theEnemies[i].dest.left - thePlayer.dest.left;
			if (dist < 320)		// closest route is to the left
				theEnemies[i].facingRight = FALSE;
			else				// closest route is to the right
				theEnemies[i].facingRight = TRUE;
		}
		else if (thePlayer.dest.left > theEnemies[i].dest.left)
		{
			dist = thePlayer.dest.left - theEnemies[i].dest.left;
			if (dist < 320)		// closest route is to the right
				theEnemies[i].facingRight = TRUE;
			else				// closest route is to the left
				theEnemies[i].facingRight = FALSE;
		}
								// seek point 16 pixels above player
		if (((theEnemies[i].v + 16) > thePlayer.v) && (evenFrame))
			shouldFlap = TRUE;
		else
			shouldFlap = FALSE;
	}
	else
	{
		if ((theEnemies[i].v > theEnemies[i].targetAlt) && (evenFrame))
			shouldFlap = TRUE;
		else
			shouldFlap = FALSE;
	}
	
	if (shouldFlap)
	{
		theEnemies[i].vVel -= theEnemies[i].flapImpulse;
		doEnemyFlapSound = TRUE;
	}
	
	if (theEnemies[i].facingRight)
	{
		theEnemies[i].hVel += kEnemyImpulse;
		if (theEnemies[i].hVel > theEnemies[i].maxHVel)
			theEnemies[i].hVel = theEnemies[i].maxHVel;
		
		switch (theEnemies[i].kind)
		{
			case kOwl:
			if (shouldFlap)
				theEnemies[i].srcNum = 12;
			else
				theEnemies[i].srcNum = 13;
			break;
			
			case kWolf:
			if (shouldFlap)
				theEnemies[i].srcNum = 16;
			else
				theEnemies[i].srcNum = 17;
			break;
			
			case kJackal:
			if (shouldFlap)
				theEnemies[i].srcNum = 20;
			else
				theEnemies[i].srcNum = 21;
			break;
		}
		
	}
	else
	{
		theEnemies[i].hVel -= kEnemyImpulse;
		if (theEnemies[i].hVel < -theEnemies[i].maxHVel)
			theEnemies[i].hVel = -theEnemies[i].maxHVel;
		
		switch (theEnemies[i].kind)
		{
			case kOwl:
			if (shouldFlap)
				theEnemies[i].srcNum = 14;
			else
				theEnemies[i].srcNum = 15;
			break;
			
			case kWolf:
			if (shouldFlap)
				theEnemies[i].srcNum = 18;
			else
				theEnemies[i].srcNum = 19;
			break;
			
			case kJackal:
			if (shouldFlap)
				theEnemies[i].srcNum = 22;
			else
				theEnemies[i].srcNum = 23;
			break;
		}
	}
	
	theEnemies[i].h += theEnemies[i].hVel;
	theEnemies[i].dest.left = theEnemies[i].h >> 4;
	theEnemies[i].dest.right = theEnemies[i].dest.left + 64;
	
	theEnemies[i].v += theEnemies[i].vVel;
	theEnemies[i].dest.top = theEnemies[i].v >> 4;
	theEnemies[i].dest.bottom = theEnemies[i].dest.top + 40;
	
	if (theEnemies[i].dest.left > 640)
	{
		OffsetRect(&theEnemies[i].dest, -640, 0);
		theEnemies[i].h = theEnemies[i].dest.left << 4;
		OffsetRect(&theEnemies[i].wasDest, -640, 0);
		theEnemies[i].pass++;
		if (theEnemies[i].pass > 2)		// after two screen passes…
		{								// enemy patrols a new altitude
			theEnemies[i].targetAlt = AssignNewAltitude();
			theEnemies[i].pass = 0;
		}
	}
	else if (theEnemies[i].dest.right < 0)
	{
		OffsetRect(&theEnemies[i].dest, 640, 0);
		theEnemies[i].h = theEnemies[i].dest.left << 4;
		OffsetRect(&theEnemies[i].wasDest, 640, 0);
		theEnemies[i].pass++;
		if (theEnemies[i].pass > 2)
		{
			theEnemies[i].targetAlt = AssignNewAltitude();
			theEnemies[i].pass = 0;
		}
	}
	
	theEnemies[i].vVel -= theEnemies[i].vVel >> 4;	// friction
	
	if (theEnemies[i].vVel > theEnemies[i].maxVVel)
		theEnemies[i].vVel = theEnemies[i].maxVVel;
	else if (theEnemies[i].vVel < -theEnemies[i].maxVVel)
		theEnemies[i].vVel = -theEnemies[i].maxVVel;
	
	CheckEnemyRoofCollision(i);
	CheckEnemyPlatformHit(i);
}

//--------------------------------------------------------------  HandleWalkingEnemy

void HandleWalkingEnemy (short i)
{
	if (theEnemies[i].facingRight)
	{
		theEnemies[i].dest.left += 6;
		theEnemies[i].dest.right += 6;
		switch (theEnemies[i].kind)
		{
			case kOwl:
			theEnemies[i].srcNum = 1 - theEnemies[i].srcNum;
			break;
			
			case kWolf:
			theEnemies[i].srcNum = 9 - theEnemies[i].srcNum;
			break;
			
			case kJackal:
			theEnemies[i].srcNum = 17 - theEnemies[i].srcNum;
			break;
		}
		theEnemies[i].hVel = 6 << 4;
	}
	else
	{
		theEnemies[i].dest.left -= 6;
		theEnemies[i].dest.right -= 6;
		switch (theEnemies[i].kind)
		{
			case kOwl:
			theEnemies[i].srcNum = 5 - theEnemies[i].srcNum;
			break;
			
			case kWolf:
			theEnemies[i].srcNum = 13 - theEnemies[i].srcNum;
			break;
			
			case kJackal:
			theEnemies[i].srcNum = 21 - theEnemies[i].srcNum;
			break;
		}
		theEnemies[i].hVel = -6 << 4;
	}
	theEnemies[i].frame++;
	if (theEnemies[i].frame >= 8)
	{
		theEnemies[i].mode = kFlying;
		theEnemies[i].frame = 0;
		switch (theEnemies[i].kind)
		{
			case kOwl:
			if (theEnemies[i].facingRight)
				theEnemies[i].srcNum = 12;
			else
				theEnemies[i].srcNum = 14;
			break;
			
			case kWolf:
			if (theEnemies[i].facingRight)
				theEnemies[i].srcNum = 16;
			else
				theEnemies[i].srcNum = 18;
			break;
			
			case kJackal:
			if (theEnemies[i].facingRight)
				theEnemies[i].srcNum = 20;
			else
				theEnemies[i].srcNum = 22;
			break;
		}
		
		theEnemies[i].dest.left -= 8;
		theEnemies[i].dest.right += 8;
		theEnemies[i].dest.bottom = theEnemies[i].dest.top + 40;
		theEnemies[i].h = theEnemies[i].dest.left * 16;
		theEnemies[i].v = theEnemies[i].dest.top * 16;
	}
}

//--------------------------------------------------------------  HandleSpawningEnemy

void HandleSpawningEnemy (short i)
{
	theEnemies[i].frame++;
	if (theEnemies[i].frame >= 48)
	{
		theEnemies[i].mode = kWalking;
		theEnemies[i].frame = 0;
		
		switch (theEnemies[i].kind)
		{
			case kOwl:
			if (theEnemies[i].facingRight)
				theEnemies[i].srcNum = 0;
			else
				theEnemies[i].srcNum = 2;
			break;
			
			case kWolf:
			if (theEnemies[i].facingRight)
				theEnemies[i].srcNum = 4;
			else
				theEnemies[i].srcNum = 6;
			break;
			
			case kJackal:
			if (theEnemies[i].facingRight)
				theEnemies[i].srcNum = 8;
			else
				theEnemies[i].srcNum = 10;
			break;
		}
	}
	else
		theEnemies[i].dest.top = theEnemies[i].dest.bottom - theEnemies[i].frame;
}

//--------------------------------------------------------------  HandleFallingEnemy

void HandleFallingEnemy (short i)
{
	theEnemies[i].vVel += kGravity;
	
	if (theEnemies[i].vVel > theEnemies[i].maxVVel)
		theEnemies[i].vVel = theEnemies[i].maxVVel;
	else if (theEnemies[i].vVel < -theEnemies[i].maxVVel)
		theEnemies[i].vVel = -theEnemies[i].maxVVel;
	
	if (evenFrame)
	{
		theEnemies[i].hVel -= (theEnemies[i].hVel >> 5);
		if ((theEnemies[i].hVel < 32) && (theEnemies[i].hVel > -32))
		{
			if (theEnemies[i].hVel > 0)
				theEnemies[i].hVel--;
			else if (theEnemies[i].hVel < 0)
				theEnemies[i].hVel++;
		}
	}
	
	theEnemies[i].h += theEnemies[i].hVel;
	theEnemies[i].dest.left = theEnemies[i].h >> 4;
	theEnemies[i].dest.right = theEnemies[i].dest.left + 24;
	
	theEnemies[i].v += theEnemies[i].vVel;
	theEnemies[i].dest.top = theEnemies[i].v >> 4;
	theEnemies[i].dest.bottom = theEnemies[i].dest.top + 24;
	
	if (theEnemies[i].dest.left > 640)
	{
		OffsetRect(&theEnemies[i].dest, -640, 0);
		theEnemies[i].h = theEnemies[i].dest.left << 4;
		OffsetRect(&theEnemies[i].wasDest, -640, 0);
	}
	else if (theEnemies[i].dest.right < 0)
	{
		OffsetRect(&theEnemies[i].dest, 640, 0);
		theEnemies[i].h = theEnemies[i].dest.left << 4;
		OffsetRect(&theEnemies[i].wasDest, 640, 0);
	}
	
	CheckEnemyRoofCollision(i);
	CheckEnemyPlatformHit(i);
}

//--------------------------------------------------------------  HandleEggEnemy

void HandleEggEnemy (short i)
{
	short		center;
	
	theEnemies[i].frame--;
	if (theEnemies[i].frame < 24)
	{
		theEnemies[i].dest.top = theEnemies[i].dest.bottom - theEnemies[i].frame;
		if (theEnemies[i].frame == 0)		// a sphinx is born!
		{
			theEnemies[i].frame = 0;
			PlayExternalSound(kSpawnSound, kSpawnPriority);
			center = (theEnemies[i].dest.left + theEnemies[i].dest.right) >> 1;
			theEnemies[i].dest.left = center - 24;
			theEnemies[i].dest.right = center + 24;
			theEnemies[i].wasDest = theEnemies[i].dest;
			theEnemies[i].h = theEnemies[i].dest.left << 4;
			theEnemies[i].v = theEnemies[i].dest.top << 4;
			theEnemies[i].wasH = theEnemies[i].h;
			theEnemies[i].wasV = theEnemies[i].v;
			theEnemies[i].hVel = 0;
			theEnemies[i].vVel = 0;
			theEnemies[i].mode = kSpawning;
			theEnemies[i].kind++;
			if (theEnemies[i].kind > kJackal)
				theEnemies[i].kind = kJackal;
			SetEnemyAttributes(i);
		}
	}
}

//--------------------------------------------------------------  MoveEnemies

void MoveEnemies (void)
{
	short		i;
	
	doEnemyFlapSound = FALSE;
	doEnemyScrapeSound = FALSE;
	
	for (i = 0; i < numEnemies; i++)
	{
		switch (theEnemies[i].mode)
		{
			case kIdle:
			HandleIdleEnemies(i);
			break;
			
			case kFlying:
			HandleFlyingEnemies(i);
			break;
			
			case kWalking:
			HandleWalkingEnemy(i);
			break;
			
			case kSpawning:
			HandleSpawningEnemy(i);
			break;
			
			case kFalling:
			HandleFallingEnemy(i);
			break;
			
			case kEggTimer:
			HandleEggEnemy(i);
			break;
			
			case kDeadAndGone:
			break;
		}
	}
	
	if (doEnemyFlapSound)
		PlayExternalSound(kFlap2Sound, kFlap2Priority);
	if (doEnemyScrapeSound)
		PlayExternalSound(kScrape2Sound, kScrape2Priority);
	if ((deadEnemies >= numEnemiesThisLevel) && (countDownTimer == 0))
		countDownTimer = 30;
}

//--------------------------------------------------------------  InitHandLocation

void InitHandLocation (void)
{
	SetRect(&theHand.dest, 0, 0, 56, 57);
	OffsetRect(&theHand.dest, 48, 460);
}

//--------------------------------------------------------------  HandleHand

void HandleHand (void)
{
	Rect		whoCares;
	short		hDiff, vDiff, pull, speed;
	
	switch (theHand.mode)
	{
		case kLurking:
		if ((thePlayer.mode == kFlying) && (SectRect(&thePlayer.dest, &grabZone, &whoCares)))
		{
			theHand.mode = kOutGrabeth;
			InitHandLocation();
		}
		break;
		
		case kOutGrabeth:
		case kClutching:
		if (SectRect(&thePlayer.dest, &grabZone, &whoCares))
		{
			hDiff = theHand.dest.left - thePlayer.dest.left;
			vDiff = theHand.dest.top - thePlayer.dest.top;
			
			if (thePlayer.facingRight)
				hDiff -= 3;
			else
				hDiff -= 21;
			vDiff -= 29;
			
			speed = (levelOn >> 3) + 1;
			if (hDiff < 0)
			{
				theHand.dest.left += speed;
				theHand.dest.right += speed;
			}
			else if (hDiff > 0)
			{
				theHand.dest.left -= speed;
				theHand.dest.right -= speed;
			}
			if (vDiff < 0)
			{
				theHand.dest.top += speed;
				theHand.dest.bottom += speed;
			}
			else if (vDiff > 0)
			{
				theHand.dest.top -= speed;
				theHand.dest.bottom -= speed;
			}
			
			if (hDiff < 0)
				hDiff = -hDiff;
			if (vDiff < 0)
				vDiff = -vDiff;
			if ((hDiff < 8) && (vDiff < 8))
			{
				theHand.mode = kClutching;
				thePlayer.clutched = TRUE;
				thePlayer.hVel = thePlayer.hVel >> 3;
				thePlayer.vVel = thePlayer.vVel >> 3;
				pull = levelOn << 2;
				if (pull > 48)
					pull = 48;
				thePlayer.vVel += pull;
				theHand.dest.top = thePlayer.dest.top + 29;
				theHand.dest.bottom = theHand.dest.top + 57;
				if (thePlayer.facingRight)
					theHand.dest.left = thePlayer.dest.left + 3;
				else
					theHand.dest.left = thePlayer.dest.left + 21;
				theHand.dest.right = theHand.dest.left + 58;
			}
			else
			{
				thePlayer.clutched = FALSE;
				theHand.mode = kOutGrabeth;
			}
		}
		else
		{
			theHand.dest.top++;
			theHand.dest.bottom++;
			if (theHand.dest.top > 460)
				theHand.mode = kLurking;
			else
				theHand.mode = kOutGrabeth;
			thePlayer.clutched = FALSE;
		}
		break;
	}
}

//--------------------------------------------------------------  InitEye

void InitEye (void)
{
	SetRect(&theEye.dest, 0, 0, 48, 31);
	OffsetRect(&theEye.dest, 296, 97);
	theEye.mode = kWaiting;
	theEye.frame = (numOwls + 2) * 720;
	theEye.srcNum = 0;
	theEye.opening = 1;
	theEye.killed = FALSE;
	theEye.entering = FALSE;
}

//--------------------------------------------------------------  KillOffEye

void KillOffEye (void)
{
	if (theEye.mode == kStalking)
	{
		theEye.killed = TRUE;
		theEye.opening = 1;
		theEye.entering = FALSE;
		if (theEye.srcNum == 0)
			theEye.srcNum = 1;
	}
	else
		InitEye();
}

//--------------------------------------------------------------  HandleEye

void HandleEye (void)
{
	short		diffH = 0, diffV = 0, speed;
	
	if (theEye.mode == kStalking)		// eye is about
	{
		speed = (levelOn >> 4) + 1;
		if (speed > 3)
			speed = 3;
		
		if ((theEye.killed) || (theEye.entering))
		{
			speed = 0;
		}
		else if ((thePlayer.mode != kFlying) && (thePlayer.mode != kWalking))
		{
			diffH = theEye.dest.left - 296;
			diffV = theEye.dest.bottom - 128;
		}
		else
		{
			diffH = theEye.dest.left - thePlayer.dest.left;
			diffV = theEye.dest.bottom - thePlayer.dest.bottom;
		}
		
		if (diffH > 0)
		{
			if (diffH < speed)
				theEye.dest.left -= diffH;
			else
				theEye.dest.left -= speed;
			theEye.dest.right = theEye.dest.left + 48;
		}
		else if (diffH < 0)
		{
			if (-diffH < speed)
				theEye.dest.left -= diffH;
			else
				theEye.dest.left += speed;
			theEye.dest.right = theEye.dest.left + 48;
		}
		if (diffV > 0)
		{
			if (diffV < speed)
				theEye.dest.bottom -= diffV;
			else
				theEye.dest.bottom -= speed;
			theEye.dest.top = theEye.dest.bottom - 31;
		}
		else if (diffV < 0)
		{
			if (-diffV < speed)
				theEye.dest.bottom -= diffV;
			else
				theEye.dest.bottom += speed;
			theEye.dest.top = theEye.dest.bottom - 31;
		}
		
		theEye.frame++;
		
		if (theEye.srcNum != 0)
		{
			if (theEye.frame > 3)		// eye-closing frame holds for 3 frames
			{
				theEye.frame = 0;
				theEye.srcNum += theEye.opening;
				if (theEye.srcNum > 3)
				{
					theEye.srcNum = 3;
					theEye.opening = -1;
					if (theEye.killed)
						InitEye();
				}
				else if (theEye.srcNum <= 0)
				{
					theEye.srcNum = 0;
					theEye.opening = 1;
					theEye.frame = 0;
					theEye.entering = FALSE;
				}
			}
		}
		else if (theEye.frame > 256)
		{
			theEye.srcNum = 1;
			theEye.opening = 1;
			theEye.frame = 0;
		}
		
		diffH = theEye.dest.left - thePlayer.dest.left;
		diffV = theEye.dest.bottom - thePlayer.dest.bottom;
		if (diffH < 0)
			diffH = -diffH;
		if (diffV < 0)
			diffV = -diffV;
		
		if ((diffH < 16) && (diffV < 16) && (!theEye.entering) && 
				(!theEye.killed))			// close enough to call it a kill
		{
			if (theEye.srcNum == 0)			// if eye open, player is killed
			{
				if (lightningCount == 0)
				{
					lightH = thePlayer.dest.left + 24;
					lightV = thePlayer.dest.bottom - 24;
					lightningCount = 6;
				}
				thePlayer.mode = kFalling;
				if (thePlayer.facingRight)
					thePlayer.srcNum = 8;
				else
					thePlayer.srcNum = 9;
				thePlayer.dest.bottom = thePlayer.dest.top + 37;
				PlayExternalSound(kBoom2Sound, kBoom2Priority);
			}
			else							// wow, player killed the eye
			{
				if (lightningCount == 0)
				{
					lightH = theEye.dest.left + 24;
					lightV = theEye.dest.top + 16;
					lightningCount = 15;
				}
				theScore += 2000L;
				UpdateScoreNumbers();
				PlayExternalSound(kBonusSound, kBonusPriority);
				
				KillOffEye();
			}
		}
	}
	else if (theEye.frame > 0)
	{
		theEye.frame--;
		if (theEye.frame == 0)			// eye appears
		{
			theEye.mode = kStalking;
			if (lightningCount == 0)
			{
				lightH = theEye.dest.left + 24;
				lightV = theEye.dest.top + 16;
				lightningCount = 6;
			}
			theEye.srcNum = 3;
			theEye.opening = 1;
			theEye.entering = TRUE;
		}
	}	
}

//--------------------------------------------------------------  ResolveEnemyPlayerHit

void ResolveEnemyPlayerHit (short i)
{
	short		wasVel, diff, h, v;
	
	if ((theEnemies[i].mode == kFalling) || (theEnemies[i].mode == kEggTimer))
	{
		deadEnemies++;
		
		theEnemies[i].mode = kDeadAndGone;
		theScore += 500L;
		UpdateScoreNumbers();
		PlayExternalSound(kBonusSound, kBonusPriority);
		InitEnemy(i, TRUE);
	}
	else
	{
		diff = (theEnemies[i].dest.top + 25) - (thePlayer.dest.top + 19);
		
		if (diff < -2)		// player is bested
		{
			if (lightningCount == 0)
			{
				lightH = thePlayer.dest.left + 24;
				lightV = thePlayer.dest.bottom - 24;
				lightningCount = 6;
			}
			
			thePlayer.mode = kFalling;
			if (thePlayer.facingRight)
				thePlayer.srcNum = 8;
			else
				thePlayer.srcNum = 9;
			thePlayer.dest.bottom = thePlayer.dest.top + 37;
			PlayExternalSound(kBoom2Sound, kBoom2Priority);
		}
		else if (diff > 2)	// enemy killed
		{
			if ((theEnemies[i].mode == kSpawning) && (theEnemies[i].frame < 16))
				return;
			
			h = (theEnemies[i].dest.left + theEnemies[i].dest.right) >> 1;
			if (theEnemies[i].mode == kSpawning)
				v = theEnemies[i].dest.bottom - 2;
			else
				v = (theEnemies[i].dest.top + theEnemies[i].dest.bottom) >> 1;
			theEnemies[i].dest.left = h - 12;
			theEnemies[i].dest.right = h + 12;
			if (theEnemies[i].mode == kSpawning)
				theEnemies[i].dest.top = v - 24;
			else
				theEnemies[i].dest.top = v - 12;
			theEnemies[i].dest.bottom = theEnemies[i].dest.top + 24;
			theEnemies[i].h = theEnemies[i].dest.left << 4;
			theEnemies[i].v = theEnemies[i].dest.top << 4;
			theEnemies[i].mode = kFalling;
			theEnemies[i].wasDest = theEnemies[i].dest;
			theEnemies[i].wasH = theEnemies[i].h;
			theEnemies[i].wasV = theEnemies[i].v;
			
			switch (theEnemies[i].kind)
			{
				case kOwl:
				theScore += 500L;
				break;
				
				case kWolf:
				theScore += 1000L;
				break;
				
				case kJackal:
				theScore += 1500L;
				break;
			}
			UpdateScoreNumbers();
			PlayExternalSound(kBoom2Sound, kBoom2Priority);
		}
		else		// neither player nor enemy killed
		{
			if (theEnemies[i].hVel > 0)
				theEnemies[i].facingRight = TRUE;
			else
				theEnemies[i].facingRight = FALSE;
			PlayExternalSound(kScreechSound, kScreechPriority);
		}
		
		wasVel = thePlayer.hVel;
		thePlayer.hVel = theEnemies[i].hVel;
		theEnemies[i].hVel = wasVel;
		wasVel = thePlayer.vVel;
		thePlayer.vVel = theEnemies[i].vVel;
		theEnemies[i].vVel = wasVel;
	}
}

//--------------------------------------------------------------  CheckPlayerEnemyCollision

void CheckPlayerEnemyCollision (void)
{
	Rect		whoCares, playTest, wrapTest;
	short		i;
	
	playTest = thePlayer.dest;
	InsetRect(&playTest, 8, 8);
	if (thePlayer.wrapping)
		wrapTest = thePlayer.wrap;
	InsetRect(&wrapTest, 8, 8);
	
	for (i = 0; i < numEnemies; i++)
	{
		if ((theEnemies[i].mode != kIdle) && (theEnemies[i].mode != kDeadAndGone))
		{
			if (SectRect(&playTest, &theEnemies[i].dest, &whoCares))
			{
				ResolveEnemyPlayerHit(i);
			}
			else if (thePlayer.wrapping)
			{
				if (SectRect(&wrapTest, &theEnemies[i].dest, &whoCares))
					ResolveEnemyPlayerHit(i);
			}
		}
	}
}

