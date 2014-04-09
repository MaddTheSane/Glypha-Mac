/*
	File:		G4Play.c

	Contains:	xxx put contents here xxx

	Version:	xxx put version here xxx

	Copyright:	© 1998 by Apple Computer, Inc., all rights reserved.

	File Ownership:

		DRI:				xxx put dri here xxx

		Other Contact:		xxx put other contact here xxx

		Technology:			xxx put technology here xxx

	Writers:

		(BWS)	Brent Schorsch
		(sjb)	Steve Bollinger

	Change History (most recent first):

		 <5>	  7/1/98	BWS		fix comment
		 <4>	  7/1/98	sjb		Update to CWPro 2
		 <3>	 6/18/98	sjb		InputSprocket.h comes from <> place
*/


//============================================================================
//----------------------------------------------------------------------------
//									Play.c
//----------------------------------------------------------------------------
//============================================================================

// This (rather large) file handles all player routines while a game is in…
// progress. It gets the player's input, moves the player, tests for collisions…
// and generally handles the "main game loop".  Enemies and actually drawing…
// the graphics to the screen are handled in other files.

#include "G4Externs.h"

#if GENERATINGPOWERPC
	#include <InputSprocket.h>
#endif

#include <ToolUtils.h>
#include <TextUtils.h>
#include <Timer.h>

#define kFlapImpulse			48
#define kGlideImpulse			12
#define kAirResistance			2
#define kMaxHVelocity			192
#define kMaxVVelocity			512
#define kNumLightningStrikes	5


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

void HandleLava(void);
void DrawLava(void);

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
extern	short		numOwls;
extern	Boolean		quitting, openTheScores;

extern CGrafPtr workSrcMap;


short oldFrameRate;
short currentFrameRate;
UnsignedWide frameTime;

short numScoreFloater = 0;
scoreFloaterType theScoreFloater[kMaxScoreFloater];



//

OSErr EasyJoystick(Boolean *left, Boolean *right, Boolean *button);
OSErr EasyJoystick(Boolean *left, Boolean *right, Boolean *button)
{
	// remember a little across calls
	static ISpElementReference lastXAxis = 0;
	static Boolean lastLeft = false;
	static Boolean lastRight = false;
	static Boolean lastButton = false;

	int maxEvents = 10;		// process only ten events per round
	
	ISpElementEvent theEvent;			// our event
	Boolean wasEvent = true;
	OSErr err = noErr;
	ISpElementListReference globalList;
	ISpElementInfo info;
	
	*left = lastLeft;
	*right = lastRight;
	*button = lastButton;


	if (ISpGetGlobalElementList == nil)
	{
		return -1;
	}

	err = ISpGetGlobalElementList(&globalList);

	while(maxEvents > 0)
	{
		err = ISpElementList_GetNextEvent(globalList, sizeof(theEvent), &theEvent, &wasEvent);
		
		if ((wasEvent == false) || (err))
		{
			break;
		}
		
		ISpElement_GetInfo(theEvent.element, &info);

		// get information about this element since we are not doing any configuration
		// we need to know the about labels and kinds

		switch(info.theKind)
		{
			case kISpElementKind_Button:
			{
				Boolean newValue;
				
				// figure out the new value for the boolean
				if (theEvent.data)
				{
					newValue = true;
				}
				else
				{
					newValue = false;
				}
	
				*button = newValue;
			}
			break;
			
			case kISpElementKind_DPad:
			{
				// we'll support using any DPad to move
				if (1) // (info.theLabel == kISpElementLabel_Pad_Move || info.theLabel == kISpElementLabel_Pad_Move_Horiz)
				{
					// a movement DPad so they might be using
					// a console style direction pad so turn
					// that into axis style data
					
					lastXAxis = nil;
					
					switch(theEvent.data)
					{
						case kISpPadIdle:
						case kISpPadUp:
						case kISpPadDown:			
							*left = false;
							*right = false;
							break;
						case kISpPadLeft:
						case kISpPadUpLeft:
						case kISpPadDownLeft:
							*left = true;
							*right = false;
							break;
						case kISpPadUpRight:
						case kISpPadRight:
						case kISpPadDownRight:
							*left = false;
							*right = true;
							break;
					}
				}
			}
			break;
			
			case kISpElementKind_Axis:
			{
				// if it is an axis find out if it is the
				// x or y axis style data and use that.
				
				if (info.theLabel == kISpElementLabel_Axis_XAxis || info.theLabel == kISpElementLabel_Axis_Rudder)
				{
					lastXAxis = theEvent.element;
				}
				
				ISpElement_Flush(theEvent.element);
			}
			break;
		}
		
		maxEvents--;
	}
	
	// poll the last meaningful axis we had on the way out
	// just to make sure we had good data
	if (lastXAxis != nil)
	{
		UInt32 xAxis;
		double calc;
		
		*left = false;
		*right = false;
	
		ISpElement_GetSimpleState(lastXAxis, &xAxis);

		calc = xAxis;
		calc -= kISpAxisMiddle;
		calc /= kISpAxisMaximum;
		
		if (calc > 0.2)
		{
			*right = true;
		}
		else if (calc < -0.2)
		{
			*left = true;
		}
	}
	
	return err;
}

//==============================================================  Functions
//--------------------------------------------------------------  InitNewGame

// This funciton sets up variables and readies for a new game.  It is called…
// only when a the user selects "New Game" - during the course of a game, it…
// is  not called again.

void InitNewGame (void)
{								// Initialize a number of game variables.
	countDownTimer = 0;			// Zero count down timer.
	numLedges = 3;				// Initial number of ledges (platforms).
	beginOnLevel = 1;			// Ledge (platform) the player is on (center ledge).
	levelOn = 0;				// Game level on (first level).
	livesLeft = kInitNumLives;	// Number of player lives remaining.
	theScore = 0L;				// Player's score (a long - can go to 2 billion).
	playing = TRUE;				// Flag playing.
	pausing = FALSE;			// Not paused.
	evenFrame = TRUE;			// Set an initial state for evenFrame.
	wasTensOfThousands = 0L;	// Used for noting when player gets an extra life.
	numOwls = 4;				// Number of "owl" enemies for this level.
	
	numPixelShatter = 0;		// no pixels exploding currently
	numScoreFloater = 0;		// no score floaters
	
	InitHandLocation();			// Get the mummy hand down in the lava.
	theHand.mode = kLurking;	// Flag the hand in "lurking" mode.
	
	ResetPlayer(TRUE);			// Initialize all player variables and put on ledge.
	UpdateLivesNumbers();		// Display number of lives remaining on screen.
	UpdateScoreNumbers();		// Display the player's score (zero at this point).
	UpdateLevelNumbers();		// Display the level (wave) the player is on.

	DumpBackToWorkMap();		// Copy background offscreen to "work" offscreen.

	SetUpLevel();				// Set up platforms for first level (wave).
	
	GenerateEnemies();			// Prepare all enemies for this level.
}

//--------------------------------------------------------------  SetUpLevel

// Primarily, this function is called to set up the ledges for the…
// current level (wave) the player is on.  It determines how many…
// are required and then draws these offscreen.  It also flashes…
// the obelisks and strikes the lightning.

void SetUpLevel (void)
{
	short		wasLedges, waveMultiple;
	Boolean validLightning[6];
	int invalidLightning;
	
	for(invalidLightning = 0; invalidLightning < 6; invalidLightning++)
	{
		validLightning[invalidLightning] = false;
	}
	
	KillOffEye();					// Return eye to the aether.
	
	wasLedges = numLedges;			// Remember number of ledges.
	waveMultiple = levelOn % 5;		// Waves repeat every 5th wave (but harder!).
	
	switch (waveMultiple)			// See which of the 5 we're on.
	{
		case 0:						// Waves 0, 5, 10, …
		numLedges = 5;				// have 5 ledges (platforms) on screen.
		validLightning[5] = true;
		break;
		
		case 1:						// Waves 1, 6, 11, …
		numLedges = 6;				// are up to 6 ledges (platforms) on screen.
		validLightning[5] = true;
		break;
		
		case 2:						// Waves 2, 7, 12, …
		numLedges = 5;				// return to 5 ledges (platforms) on screen.
		validLightning[5] = true;
		break;
		
		case 3:						// Waves 3, 8, 13, …
		numLedges = 3;				// drop to 3 ledges (platforms) on screen.
		validLightning[3] = true;
		validLightning[4] = true;
		break;
		
		case 4:						// Waves 4, 9, 14, …
		numLedges = 6;				// and return to 6 ledges (platforms) on screen.
		validLightning[3] = true;
		validLightning[4] = true;
		validLightning[5] = true;
		break;
	}
	
	if (wasLedges != numLedges)		// No need to redraw if platforms are unchanged.
		DrawPlatforms(numLedges);

#if 0
							// These are the platforms.  See diagram for numbering.
	SetRect(&platformRects[0], 206, 424, 433, 438);		//_______________
	SetRect(&platformRects[1], -256, 284, 149, 298);	//
	SetRect(&platformRects[2], 490, 284, 896, 298);		//--3--     --4--
	SetRect(&platformRects[3], -256, 105, 149, 119);	//     --5--
	SetRect(&platformRects[4], 490, 105, 896, 119);		//--1--     --2--
	SetRect(&platformRects[5], 233, 190, 407, 204);		//_____--0--_____

#endif



	{
		int toast;
		
		if (levelOn == 0)
		{
			for(toast = 0; toast < 6; toast++)
			{
				validLightning[toast] = true;
			}
		}
		
		for(toast = 0; toast < 6; toast++)
		{
			Rect r = platformRects[toast];
			int diff = (r.right - r.left) / 4;
			int subtoast;
			if (!validLightning[toast])
			{
				continue;
			}
			
			r.right = r.left + diff;
			
			for(subtoast = 0; subtoast < 4; subtoast++)
			{
				unsigned long tickWait = TickCount() + 3L;
				
				
				PlayExternalSound(kLightningSound, kLightningPriority);
	
				DSpContext_GetBackBuffer( gTheContext, kDSpBufferKind_Normal, &workSrcMap );
				GetPlayerInput();						// Get the player's input (keystrokes).
				MovePlayer();							// Move the player's bird.
				
				HandleLava();
				DumpBackToWorkMap();					// clear the screen
				DrawBanner();
				DrawLava();
				DrawPlayer();

				// lightning
				lightningCount = 1;						
				lightH = (r.left + r.right) / 2;
				lightV = (r.top + r.bottom) / 2;
				HandleLightning();

				// pixel explosions
				StartPixelShatterRect(&r, 0, 0, kShatterPlatformLightning);
				HandlePixelShatter();
				DrawPixelShatter();
				
				DSpContext_SwapBuffers( gTheContext, NULL, NULL );

				r.left += diff;
				r.right += diff;

				evenFrame = !evenFrame;
				
				do				// Wait for 2 Ticks to pass to keep fast Macs at bay.
				{
				}
				while (TickCount() < tickWait);
			}
		}
	}
	
	
	UpdateLevelNumbers();			// Display the current level on screen.
}

//--------------------------------------------------------------  ResetPlayer

// This function prepares the player - it places the player and his/her mount…
// in their proper starting location (depending on which platform they are to…
// begin on), and it sets all the player's variables to their initial state.

void ResetPlayer (Boolean initialPlace)
{
	short		location;
	
	thePlayer.srcNum = 5;			// Set which graphic (frame) the player is to use.
	thePlayer.frame = 320;			// This variable will be used as a coutndown timer.
	
	if (initialPlace)				// If "initialPlace" is TRUE, …
		location = 0;				// the player is to begin on the lowest platform.
	else							// Otherwise, a random location is chosen.
		location = RandomInt(numLedges);
	
	switch (location)				// Move player horizontally and vertically to their…
	{								// proper location (based on ledge # they're on).
		case 0:
		thePlayer.h = 296 << 4;		// Bottom center ledge.
		thePlayer.v = 377 << 4;		// We're scaling by 16.
		break;
		
		case 1:
		thePlayer.h = 102 << 4;		// Lower left ledge.
		thePlayer.v = 237 << 4;
		break;
		
		case 2:
		thePlayer.h = 489 << 4;		// Lower right ledge.
		thePlayer.v = 237 << 4;
		break;
		
		case 3:
		thePlayer.h = 102 << 4;		// Top left ledge.
		thePlayer.v = 58 << 4;
		break;
		
		case 4:
		thePlayer.h = 489 << 4;		// Top right ledge.
		thePlayer.v = 58 << 4;
		break;
		
		case 5:
		thePlayer.h = 296 << 4;		// Top central ledge.
		thePlayer.v = 143 << 4;
		break;
	}
									// Assign destination rectangle.
	thePlayer.dest = playerRects[thePlayer.srcNum];
	ZeroRectCorner(&thePlayer.dest);
	OffsetRect(&thePlayer.dest, thePlayer.h >> 4, thePlayer.v >> 4);
	thePlayer.wasDest = thePlayer.dest;
	
	thePlayer.hVel = 0;				// Player initially has no velocity.
	thePlayer.vVel = 0;
	thePlayer.facingRight = TRUE;	// We're facing to the right.
	thePlayer.flapping = FALSE;		// We're not flapping our wings initially.
	thePlayer.wrapping = FALSE;		// We can't be wrapping around the edge of the screen.
	thePlayer.clutched = FALSE;		// The hand ain't got us.
	thePlayer.mode = kIdle;			// Our mode is "idle" - waiting to be "born".
	if (lightningCount == 0)		// Prepare for a lightning display to "birth" us.
	{
		lightH = thePlayer.dest.left + 24;
		lightV = thePlayer.dest.bottom - 24;
		lightningCount = kNumLightningStrikes;
	}
}

//--------------------------------------------------------------  OffAMortal

// Alas, 'tis here that a player is brought who loses a life.

void OffAMortal (void)
{
	livesLeft--;				// Decrememnt number of player lives left.
	
	if (livesLeft > 0)			// Indeed, are there lives remaining?
	{
		ResetPlayer(FALSE);		// Good, start a new one off.
		UpdateLivesNumbers();	// Make note of the number of lives remaining.
	}
	else						// Otherwise, we are at the dreaded "Game Over".
		playing = FALSE;		// Set flag to drop us out of game loop.
}

//--------------------------------------------------------------  DoCommandKey

// This function handles the case when the user has held down the command…
// key.  Note, this only applies to input when a game is in session - otherwise…
// a standard event loop handles command keys and everything else.

void DoCommandKey (void)
{
	if (BitTst(&theKeys, kEKeyMap))			// Test for "command - E"…
	{
		playing = FALSE;					// which would indicate "End Game".
	}
	else if (BitTst(&theKeys, kPKeyMap))	// Otherwise, see if it's "command - P".
	{
		pausing = TRUE;						// This means the player is pausing the game.
		MenusReflectMode();					// Gray-out menus etc.
	}
	else if (BitTst(&theKeys, kQKeyMap))	// Or perhaps the player hit "command - Q".
	{
		playing = FALSE;					// Set flag to drop out of game loop.
		quitting = TRUE;					// Set flag to drop out of Glypha.
	}
}

//--------------------------------------------------------------  GetPlayerInput

// This function looks for keystrokes when a game is underway.  We don't use…
// the more conventional event routines (like GetNextEvent()), because they're…
// notoriously slow, allow background tasks, introduce possible INIT problems,…
// and we don't have to.  Instead, we'll rely on GetKeys() (which has its own…
// set of problems - but we deal with them).

void GetPlayerInput (void)
{
	thePlayer.flapping = FALSE;				// Assume we're not flapping.
	thePlayer.walking = FALSE;				// Assume too we're not walking.

	GetKeys(theKeys);						// Get the current keyboard keymap.
	
	// InputSprocket!
	{
		Boolean left = false;
		Boolean right = false;
		Boolean button = false;
		
		EasyJoystick(&left, &right, &button);
		
		if (button)	{ BitSet(&theKeys, kDownArrowKeyMap); }
		if (left) { BitSet(&theKeys, kLeftArrowKeyMap); }
		if (right) { BitSet(&theKeys, kRightArrowKeyMap); }
	}
	
	if (BitTst(&theKeys, kCommandKeyMap))	// See first if command key down…
		DoCommandKey();						// and handle those seperately.
	else									// If not command key, continue.
	{										// Look for one of the two "flap" keys.
		if ((BitTst(&theKeys, kSpaceBarMap)) || (BitTst(&theKeys, kDownArrowKeyMap)))
		{
			if (thePlayer.mode == kIdle)	// Handle special case when player is idle.
			{
				thePlayer.mode = kWalking;	// Set the player's mode now to walking.
				thePlayer.frame = 0;		// Used to note "state" of walking.
			}								// Otherwise, if player is flying or walking…
			else if ((thePlayer.mode == kFlying) || (thePlayer.mode == kWalking))
			{
				if (!flapKeyDown)			// If flap key was not down last frame…
				{							// (this is to prevent "automatic fire").
											// Give player lift.
					thePlayer.vVel -= kFlapImpulse;
					flapKeyDown = TRUE;		// Note that the flap key is down.
											// Play the "flap" sound.
					PlayExternalSound(kFlapSound, kFlapPriority);
											// Set player flag to indicate flapping.
					thePlayer.flapping = TRUE;
				}
			}
		}
		else
			flapKeyDown = FALSE;			// If flap key not down, remember this.
		
											// Test now for one of three "right" keys.
		if ((BitTst(&theKeys, kRightArrowKeyMap) || 
				BitTst(&theKeys, kSKeyMap) || 
				BitTst(&theKeys, kQuoteMap)) && 
				(thePlayer.hVel < kMaxHVelocity))
		{
			if (thePlayer.mode == kIdle)	// Handle special case when player idle.
			{								// They are to begin walking (no longer idle).
				thePlayer.mode = kWalking;
				thePlayer.frame = 0;
			}
			else if ((thePlayer.mode == kFlying) || (thePlayer.mode == kWalking))
			{								// If flying or walking, player moves right.
				if (!thePlayer.facingRight)	// If facing left, player does an about face.
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
				}							// Otherwise, if facing right already…
				else
				{							// If flying, add to their horizontal velocity.
					if (thePlayer.mode == kFlying)
						thePlayer.hVel += kGlideImpulse;
					else					// If walking, set flag to indicate a step.
						thePlayer.walking = TRUE;
				}
			}
		}									// Test now for one of three "left" keys.
		else if ((BitTst(&theKeys, kLeftArrowKeyMap) || 
				BitTst(&theKeys, kAKeyMap) || 
				BitTst(&theKeys, kColonMap)) && 
				(thePlayer.hVel > -kMaxHVelocity))
		{
			if (thePlayer.mode == kIdle)	// Handle special case when player idle.
			{
				thePlayer.mode = kWalking;
				thePlayer.frame = 0;
			}
			else if ((thePlayer.mode == kFlying) || (thePlayer.mode == kWalking))
			{								// If flying or walking, player moves left.
				if (thePlayer.facingRight)	// If facing right, player does an about face.
				{							// Flag player facing left.
					thePlayer.facingRight = FALSE;
					if (thePlayer.clutched)	// Handle case where player gripped by hand.
					{						// An about face handled a bit differently.
						thePlayer.dest.left -= 18;
						thePlayer.dest.right -= 18;
						thePlayer.h = thePlayer.dest.left << 4;
						thePlayer.wasH = thePlayer.h;
						thePlayer.wasDest = thePlayer.dest;
					}
				}
				else						// Otherwise, player already facing left.
				{							// So player will move left.
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

// Following are a number of functions handling the player's different "modes".
// This first function handles the player when in "idle" mode.  When idle, the…
// player is standing on a platform - having just been "born".  This is when the…
// player is in a "safe" mode - meaning no enemy can kill them.  The player remains…
// in idle mode until they hit a key to flap or move or until a timer (thePlayer.frame)…
// counts down to zero.

void HandlePlayerIdle (void)
{
	thePlayer.frame--;				// Count down the timer.
	if (thePlayer.frame == 0)		// See if timer has reached zero yet.
		thePlayer.mode = kWalking;	// If so, player is no longer idle.
	
	SetAndCheckPlayerDest();		// Keep player on platform.
}

//--------------------------------------------------------------  HandlePlayerFlying

// This function handles a player in "flying" mode.  In flying mode, the player…
// is alive and not standing/walking on any platform.  A plyaer remains in flying…
// mode until the player dies (collides unfavorably with an enemy), is caught by…
// the hand, or comes near the top of a platform (in which case they land and…
// switch to walking mode).  While in flying mode, gravity pulls the player down…
// while friction acts to slow the player down.

void HandlePlayerFlying (void)
{	
	if (thePlayer.hVel > 0)					// If player has a positive hori. velocity…
	{										// subtract frictional constant from velocity.
		thePlayer.hVel -= kAirResistance;
		if (thePlayer.hVel < 0)				// Don't let it go negative (otherwise, you…
			thePlayer.hVel = 0;				// can get a "yo-yo" effect set up).
	}
	else if (thePlayer.hVel < 0)			// Otherwise, if horizontal velocity negative…
	{										// add firctional constant to hori. velocity.
		thePlayer.hVel += kAirResistance;
		if (thePlayer.hVel > 0)
			thePlayer.hVel = 0;
	}
	
	thePlayer.vVel += kGravity;				// Add gravity to player's vertical velocity.
	
	if (thePlayer.vVel > kMaxVVelocity)		// Don't allow player to fall too fast.
		thePlayer.vVel = kMaxVVelocity;
	else if (thePlayer.vVel < -kMaxVVelocity)
		thePlayer.vVel = -kMaxVVelocity;	// And don't allow player to climb too fast.
	
	thePlayer.h += thePlayer.hVel;			// Add velocities to players position.
	thePlayer.v += thePlayer.vVel;
											// Now we determine which graphic to use.
	if (thePlayer.facingRight)				// There are the set of right-facing graphics.
	{
		thePlayer.srcNum = 1;				// Assume standard right-facing graphic.
		if (thePlayer.vVel < -kDontFlapVel)	// Now we jump through a series of hoops…
		{									// simply to determine whether we'll use…
			if (thePlayer.flapping)			// the graphic of the player with the wings…
				thePlayer.srcNum = 0;		// up (srcNum = 0) or with the wings down…
			else							// (srcNum = 1).
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
	else									// If the player is facing left…
	{										// We jump through a similar set of hoops…
		thePlayer.srcNum = 2;				// this time choosing between srcNum = 2 …
		if (thePlayer.vVel < -kDontFlapVel)	// and srcNum = 3.
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
	
	SetAndCheckPlayerDest();				// Check for wrap-around, etc.
	
	CheckLavaRoofCollision();				// See if player hit top or bottom of screen.
	CheckPlayerEnemyCollision();			// See if player hit an enemy.
	CheckPlatformCollision();				// See if player collided with platform.
	CheckTouchDownCollision();				// See if player has landed on platform.
}

//--------------------------------------------------------------  HandlePlayerWalking

// This function handles a player in "walking" mode.  They remain in this mode…
// until they walk off a platform's edge, flap to lift off the platform, or…
// collide unfavorably with an enemy (die).  While in walking mode, we need only…
// determine which frame of animation to display (if the player is taking steps)…
// and check for the usual set of collisions.

void HandlePlayerWalking (void)
{
	short		desiredHVel;
	
	if (thePlayer.walking)					// This means user is actively holding down…
	{										// the left or right key.
		if (evenFrame)						// Now we jump through a number of hoops…
		{									// in order to get a semi-realistic…
			if (thePlayer.facingRight)		// "stepping" animation going.  We take steps…
			{								// only on "even frames".
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
				thePlayer.hVel += 80;		// Move player right.
				if (thePlayer.hVel > desiredHVel)
				{							// This is the case where player is walking.
					thePlayer.hVel = desiredHVel;
					PlayExternalSound(kWalkSound, kWalkPriority);
				}
				else
				{						// In this case, player is skidding.
					PlayExternalSound(kScreechSound, kScreechPriority);
					StartPixelShatter((thePlayer.dest.left + thePlayer.dest.right) / 2,
										thePlayer.dest.bottom, thePlayer.hVel, thePlayer.vVel, kShatterPlatformScrape);
			
				}
			}
			else
			{
				thePlayer.hVel -= 80;		// Move player to the left.
				if (thePlayer.hVel < desiredHVel)
				{							// Player is stepping to left.
					thePlayer.hVel = desiredHVel;
					PlayExternalSound(kWalkSound, kWalkPriority);
				}
				else
				{						// Player is skidding to a stop.
					PlayExternalSound(kScreechSound, kScreechPriority);
					StartPixelShatter((thePlayer.dest.left + thePlayer.dest.right) / 2,
									thePlayer.dest.bottom, thePlayer.hVel, thePlayer.vVel, kShatterPlatformScrape);

				}
			}
		}
	}
	else									// If user is not actively holding down the…
	{										// left or right key, bring player to a stop.
		thePlayer.hVel -= thePlayer.hVel / 4;
		if ((thePlayer.hVel < 4) && (thePlayer.hVel > -4))
			thePlayer.hVel = 0;				// If close to zero (within 4), stop player.
		else								// Othewrwise, play the skidding sound.
			PlayExternalSound(kScreechSound, kScreechPriority);
	}
	
	if (thePlayer.vVel > kMaxVVelocity)		// Keep player from moving too quickly…
		thePlayer.vVel = kMaxVVelocity;		// left or right.
	else if (thePlayer.vVel < -kMaxVVelocity)
		thePlayer.vVel = -kMaxVVelocity;
	
	thePlayer.h += thePlayer.hVel;			// Move player horizontally and vertically…
	thePlayer.v += thePlayer.vVel;			// by the corresponding velocity.
	
	if (thePlayer.walking)					// "If player holding down left or right keys…".
	{
		if (evenFrame)						// Here's where we toggle between the two…
		{									// frames of "stepping" animation.
			if (thePlayer.facingRight)
				thePlayer.srcNum = 9 - thePlayer.srcNum;
			else
				thePlayer.srcNum = 13 - thePlayer.srcNum;
		}
	}
	else									// If the player not holding down keys…
	{										// draw the player just standing there.
		if (thePlayer.facingRight)
			thePlayer.srcNum = 5;
		else
			thePlayer.srcNum = 6;
	}
	
	SetAndCheckPlayerDest();				// Check for wrap-around and all that.
	
	CheckTouchDownCollision();				// See if player still on platform.
	KeepPlayerOnPlatform();					// Don't let player "sink through" ledge.
	CheckPlayerEnemyCollision();			// See if player hit an enemy.
}

//--------------------------------------------------------------  HandlePlayerSinking

// When the player is in "sinking" mode, they are on a one-way ticket to death.
// The player is sinking into the lava.  We put the player into this mode (rather…
// than kill them outright) so that we can have a number of frames of them slowly…
// slipping beneath the surface of the lava.  When the get below the surface of…
// the lava, they will be officially "killed" and a new player will be "born",

void HandlePlayerSinking (void)
{
	thePlayer.hVel = 0;						// Don't allow horizontal motion.
	thePlayer.vVel = 16;					// They will sink at this constant rate.
	if (thePlayer.dest.top > kLavaHeight)	// See if they slipped below the surface.
		OffAMortal();						// If they did, kill 'em.
	
	thePlayer.v += thePlayer.vVel;			// Otherwise, move them down a notch.
	
	SetAndCheckPlayerDest();				// Check for wrap-around, etc.
}

//--------------------------------------------------------------  HandlePlayerFalling

// "Falling" refers to a player who is dead already but is still careening…
// down the screen as a skeleton.  If (when) the player lands on a ledge they…
// will turn into a pile of bones for a short duration.  If instead they fall…
// into the lava, they'll sink.  In any event, it is then that they are…
// officially pronounced dead and a new player is born.

void HandlePlayerFalling (void)
{
	if (thePlayer.hVel > 0)				// Handle horizontal air resistance.
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
	
	thePlayer.vVel += kGravity;			// Add in effect of gravity.
	
	if (thePlayer.vVel > kMaxVVelocity)	// Keep player from falling too fast.
		thePlayer.vVel = kMaxVVelocity;
	else if (thePlayer.vVel < -kMaxVVelocity)
		thePlayer.vVel = -kMaxVVelocity;
	
	thePlayer.h += thePlayer.hVel;		// Move player's x and y (h and v)…
	thePlayer.v += thePlayer.vVel;		// by amount of velocity in each direction.
	
	SetAndCheckPlayerDest();			// Check for wrap-around, etc.
	
	CheckLavaRoofCollision();			// See if they hit roof or lava.
	CheckPlatformCollision();			// See if they crashed to a ledge.
}

//--------------------------------------------------------------  HandlePlayerBones

// This is when the player is just a static pile of bones on a platform.  They…
// have been killed by an enemy and now are waiting to slip away so that a new…
// player can be born.

void HandlePlayerBones (void)
{
	if (evenFrame)					// To slow it down a bit, action only occurs…
	{								// on the even frames.
		thePlayer.frame--;			// Drop the counter down by one.
		if (thePlayer.frame == 0)	// When counter reaches zero, player officially dies.
			OffAMortal();
		else						// Otherwise, player's bones are sinking.
			thePlayer.dest.top = thePlayer.dest.bottom - thePlayer.frame;
	}
}

//--------------------------------------------------------------  MovePlayer

// This function is the sort of "master movement" function.  It looks…
// at what mode a player is in and calls the appropriate function from…
// above.  Arcade games (at least this one) tend to be very "modal" in…
// this way.  It's the actions of the user and the enemies in the game…
// that cause the player's mode to move from one state to another.

void MovePlayer (void)
{
	switch (thePlayer.mode)		// Check the "mode" the player is in.
	{
		case kIdle:				// Invulnerable - standing there - just born.
		HandlePlayerIdle();
		break;
		
		case kFlying:			// Flapping, floating, airborne.
		HandlePlayerFlying();
		break;
		
		case kWalking:			// On terra firma.  Standing or walking on ledge.
		HandlePlayerWalking();
		break;
		
		case kSinking:			// Trapped in the lava - going down.
		HandlePlayerSinking();
		break;
		
		case kFalling:			// Dead - a skeleton falling to earth.
		HandlePlayerFalling();
		break;
		
		case kBones:			// Dead - a static pile of bones on a ledge.
		HandlePlayerBones();
		break;
	}
}

//--------------------------------------------------------------  CheckTouchDownCollision

// This function determines whether or not the player is landed on a ledge.
// It does this by doing a rectangle collision between the player's bounding…
// rectangle and an imaginary rectangle enclosing an area above the ledges.
// I call these imaginary rectangles "touchDownRects[]".  The trick was that…
// you don't want the player to have to "hit" the top of a ledge in order to…
// land on it - there is an arbitrary distance above a ledge where, if the player…
// is within this area, the legs ought to come out and the player flagged as…
// walking.  As well, this same function is used for a walking player to see…
// if they are still on the ledge (they may walk off the edge).

void CheckTouchDownCollision (void)
{
	Rect		testRect, whoCares;
	short		i, offset;
	Boolean		sected;
	
	sected = FALSE;								// Assume not on ledge.
	for (i = 0; i < numLedges; i++)				// Go through all ledges.
	{
		testRect = touchDownRects[i];			// Here's the imaginary rect.
		if (thePlayer.mode == kWalking)			// We need an offset if player walking…
			OffsetRect(&testRect, 0, 11);		// since the player graphic is taller.
		
		if (SectRect(&thePlayer.dest, &testRect, &whoCares))
		{										// Does the player's rect intersect?
			if (thePlayer.mode == kFlying)		// Okay, it does, is the player airborne?
			{
				StartPixelShatter((thePlayer.dest.right+thePlayer.dest.left)/2,
									thePlayer.dest.bottom,
									thePlayer.hVel,
									thePlayer.vVel, kShatterPlatformScrape);

				thePlayer.mode = kWalking;		// Put player into walking mode.
				if (thePlayer.facingRight)		// Assign correct graphic for player.
					thePlayer.srcNum = 5;
				else
					thePlayer.srcNum = 6;
				if (thePlayer.vVel > 0)			// Stop player from falling further.
					thePlayer.vVel = 0;
				thePlayer.dest.bottom += 11;	// "Grow" player's bounding rect.
				thePlayer.wasDest.bottom += 11;
												// Move player so standing on top of ledge.
				offset = thePlayer.dest.bottom - testRect.bottom - 1;
				thePlayer.dest.bottom -= offset;
				thePlayer.dest.top -= offset;
				thePlayer.v = thePlayer.dest.top << 4;
												// Play brief collision sound.
				PlayExternalSound(kGrateSound, kGratePriority);
			}
			
			sected = TRUE;						// Make note that we've landed.
		}
	}
	
	if (!sected)								// Now, if we didn't collide…
	{											// were we walking?
		if (thePlayer.mode == kWalking)			// Did we walk off the ledge?
		{
			thePlayer.mode = kFlying;			// Set player to flying mode.
			thePlayer.dest.bottom -= 11;		// Resize player's bounding rect.
			thePlayer.wasDest.bottom -= 11;
		}
	}
}

//--------------------------------------------------------------  CheckPlatformCollision

// Unlike the above function, this one tests the player's bounding rect against…
// the bounding rect of each ledge (not an imaginary rect above the ledge).  This…
// function is primarily for (then) collisions off the bottom and sides of the…
// ledges.  In this way, the ledges are "solid" - not able to be passed through.

void CheckPlatformCollision (void)
{
	Rect		hRect, vRect, whoCares;
	short		i, offset;
	
	for (i = 0; i < numLedges; i++)					// Walk through all ledges.
	{												// Test rectangle overlap.
		if (SectRect(&thePlayer.dest, &platformRects[i], &whoCares))
		{											// If player intersecting ledge…
			hRect.left = thePlayer.dest.left;		// Create our special test rect.
			hRect.right = thePlayer.dest.right;
			hRect.top = thePlayer.wasDest.top;
			hRect.bottom = thePlayer.wasDest.bottom;
													// Determine if the player hit the…
													// top/bottom of the ledge or the…
													// sides of the ledge.
			if (SectRect(&hRect, &platformRects[i], &whoCares))
			{										// We're fairly sure the player hit…
													// the left or right edge of ledge.
													
				PlayExternalSound(kGrateSound, kGratePriority);
				
				StartPixelShatter(	(thePlayer.dest.right+thePlayer.dest.left)/2,
									thePlayer.dest.bottom,
									thePlayer.hVel,
									thePlayer.vVel, kShatterPlatformHit);
									
				if (thePlayer.h > thePlayer.wasH)	// If player was heading right…
				{									// player will bounce to left.
					offset = thePlayer.dest.right - platformRects[i].left;
					thePlayer.dest.left -= offset;
					thePlayer.dest.right -= offset;
					thePlayer.h = thePlayer.dest.left << 4;
					if (thePlayer.hVel > 0)			// We bounce back with 1/2 our vel.
						thePlayer.hVel = -(thePlayer.hVel >> 1);
					else
						thePlayer.hVel = thePlayer.hVel >> 1;
				}									// Else if player was heading left…
				else if (thePlayer.h < thePlayer.wasH)
				{									// player will bounce right.
					offset = platformRects[i].right - thePlayer.dest.left;
					thePlayer.dest.left += offset;
					thePlayer.dest.right += offset;
					thePlayer.h = thePlayer.dest.left << 4;
					if (thePlayer.hVel < 0)			// We bounce back with 1/2 our vel.
						thePlayer.hVel = -(thePlayer.hVel >> 1);
					else
						thePlayer.hVel = thePlayer.hVel >> 1;
				}									// Play impact sound.

			}
			else									// It doesn't look like we hit the…
			{										// the left or right edge of ledge.
				vRect.left = thePlayer.wasDest.left;
				vRect.right = thePlayer.wasDest.right;
				vRect.top = thePlayer.dest.top;
				vRect.bottom = thePlayer.dest.bottom;
													// So we'll test top/bottom collision.
				if (SectRect(&vRect, &platformRects[i], &whoCares))
				{									// We've decided we've hit top/bottom.
					if (thePlayer.wasV < thePlayer.v)
					{								// If we were heading down (hit top)…
													// keep player on top of ledge.
						offset = thePlayer.dest.bottom - platformRects[i].top;
						thePlayer.dest.top -= offset;
						thePlayer.dest.bottom -= offset;
						thePlayer.v = thePlayer.dest.top << 4;
													// Play collision sound.
						if (thePlayer.vVel > kDontFlapVel)
						{				
							StartPixelShatter(	(thePlayer.dest.right+thePlayer.dest.left)/2,
									thePlayer.dest.bottom,
									thePlayer.hVel,
									thePlayer.vVel, kShatterPlatformScrape);

							PlayExternalSound(kGrateSound, kGratePriority);
						}
													// If we were falling bones (dead)…
						if (thePlayer.mode == kFalling)
						{							// we'll bounce.
							if ((thePlayer.dest.right - 16) > platformRects[i].right)							{
								thePlayer.hVel = 16;
								if (thePlayer.vVel > 0)
									thePlayer.vVel = -(thePlayer.vVel >> 1);
								else
									thePlayer.vVel = thePlayer.vVel >> 1;
							}
							else if ((thePlayer.dest.left + 16) < platformRects[i].left)
							{
								thePlayer.hVel = -16;
								if (thePlayer.vVel > 0)
									thePlayer.vVel = -(thePlayer.vVel >> 1);
								else
									thePlayer.vVel = thePlayer.vVel >> 1;
							}
							else					// If we were nearly stopped…
							{						// turn into pile of bones.
								PlayExternalSound(kBoom1Sound, kBoom1Priority);
								thePlayer.vVel = 0;
								thePlayer.mode = kBones;
								thePlayer.frame = 22;
								thePlayer.dest.top = thePlayer.dest.bottom - 22;
								thePlayer.v = thePlayer.dest.top << 4;
								thePlayer.srcNum = 10;
							}
						}
						else						// Okay, if we weren't falling bones…
						{							// bounce the player (-1/2 vel.).
							if (thePlayer.vVel > 0)
								thePlayer.vVel = -(thePlayer.vVel >> 1);
							else
								thePlayer.vVel = thePlayer.vVel >> 1;
						}
					}								// If the player was instead moving up…
					else if (thePlayer.wasV > thePlayer.v)
					{								// the player likely hit the bottom of…
													// the ledge.  Keep player below ledge.
													
						StartPixelShatter(	(thePlayer.dest.right+thePlayer.dest.left)/2,
											thePlayer.dest.bottom,
											thePlayer.hVel,
											thePlayer.vVel, kShatterPlatformHit);
						
						offset = platformRects[i].bottom - thePlayer.dest.top;
						thePlayer.dest.top += offset;
						thePlayer.dest.bottom += offset;
						thePlayer.v = thePlayer.dest.top << 4;
													// Play collision sound.
						PlayExternalSound(kGrateSound, kGratePriority);
						if (thePlayer.vVel < 0)		// Bounce player down (-1/2 vel.).
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

// This is an alignment function.  It is called only if the player is standing or…
// walking on a ledge.  It is designed to keep the player's mount's (bird's)…
// feet firmly planted on the ledge.  Consider that, with the addition of gravity…
// to a player's downward velocity, there is a problem where the player can appear…
// to slowly sink down through the ledge.  There may be any number of methods you…
// might want to try to prevent this from becoming a problem in the first place, …
// but my experience has been that all the methods I've tried have flaws - correcting…
// for those flaws points out other flaws and you start getting a messy sort of…
// patchwork.  Should you ever get it to work, the mess that is your function has come…
// to resemble the Knot of ????.

void KeepPlayerOnPlatform (void)
{
	Rect		whoCares;
	short		i, offset;
	
	for (i = 0; i < numLedges; i++)		// For each ledge for this wave…
	{									// test for a collision.
		if ((SectRect(&thePlayer.dest, &platformRects[i], &whoCares)) && (thePlayer.vVel > 0))
		{								// If collided (player sinking), force…
										// player to top of ledge.
			offset = thePlayer.dest.bottom - platformRects[i].top - 1;
			thePlayer.dest.top -= offset;
			thePlayer.dest.bottom -= offset;
			thePlayer.v = thePlayer.dest.top * 16;
		}
	}
	
	if (thePlayer.vVel > 0)				// Set player's vertical velocity to zero.
		thePlayer.vVel = 0;
}

//--------------------------------------------------------------  CheckLavaRoofCollision

// This is a simple high/low test to see if the player has either bounced off…
// the roof of the "arena" or dipped down into the lava below.

void CheckLavaRoofCollision (void)
{
	short		offset;
	
	if (thePlayer.dest.bottom > kLavaHeight)	// See if player in lava.
	{
		if (thePlayer.mode == kFalling)			// If falling (dead), "Splash!"
			PlayExternalSound(kSplashSound, kSplashPriority);
		else									// If flying (alive), "Yeow!"
			PlayExternalSound(kBirdSound, kBirdPriority);
	
		{
			short left = thePlayer.dest.left;
			short right = thePlayer.dest.right;
			short bottom = thePlayer.dest.bottom;
			short delta = (right - left);
			short temp = left * 32;
			short splashItr;
			
			for(splashItr = 0; splashItr < 32; splashItr++)
			{
				StartPixelShatter(temp / 32, bottom, (thePlayer.hVel * 0.75), -(thePlayer.vVel * 0.75), kShatterLavaSplash);
				temp += delta;
			}
		}

		thePlayer.mode = kSinking;				// Irregardless, player is now sinking.
	}
	else if (thePlayer.dest.top < kRoofHeight)	// See if player hit roof.
	{											// Move player to below roof.
		offset = kRoofHeight - thePlayer.dest.top;
		thePlayer.dest.top += offset;
		thePlayer.dest.bottom += offset;
		thePlayer.v = thePlayer.dest.top * 16;
												// Play collision sound.
		PlayExternalSound(kGrateSound, kGratePriority);
		thePlayer.vVel = thePlayer.vVel / -2;	// Rebound player (-1/2 vel.).
	}
}

//--------------------------------------------------------------  SetAndCheckPlayerDest

// This function keeps our player's screen coordinates and "scaled" coordinates…
// in agreement.  As well, it checks for wrap-around and handles it.

void SetAndCheckPlayerDest (void)
{
	short		wasTall, wasWide;
										// Remember width and height of player.
	wasTall = thePlayer.dest.bottom - thePlayer.dest.top;
	wasWide = thePlayer.dest.right - thePlayer.dest.left;
										// Convert scaled coords to screen coords.
	thePlayer.dest.left = thePlayer.h >> 4;
	thePlayer.dest.right = thePlayer.dest.left + wasWide;
	thePlayer.dest.top = thePlayer.v >> 4;
	thePlayer.dest.bottom = thePlayer.dest.top + wasTall;
	
	if (thePlayer.dest.left > 640)		// Has player left right side of arena?
	{									// Wrap player back to left side of screen.
		OffsetRect(&thePlayer.dest, -640, 0);
		thePlayer.h = thePlayer.dest.left << 4;
		OffsetRect(&thePlayer.wasDest, -640, 0);
	}
	else if (thePlayer.dest.right < 0)	// Else, has player left left side of screen?
	{									// Wrap player around to right side of screen.
		OffsetRect(&thePlayer.dest, 640, 0);
		thePlayer.h = thePlayer.dest.left << 4;
		OffsetRect(&thePlayer.wasDest, 640, 0);
	}
}

//--------------------------------------------------------------  HandleLightning

// Lightning is handled here.  Obelisks are flashed, lightning is generated, …
// lighting strikes, and the lightning counter decremented.  This is pretty…
// nice - we can just set "lightningCount" to a non-zero number and this…
// function will strike lightning every fram until the counter returns to zero.

void HandleLightning (void)
{
	Boolean doLightning = false;
	short hLoc;
	short vLoc;
	
	if (thePlayer.electrical > 0)
	{
		lightningCount = 0;
		
		hLoc = (thePlayer.dest.left + thePlayer.dest.right) / 2;
		vLoc = (thePlayer.dest.top + thePlayer.dest.bottom) / 2;
		thePlayer.electrical--;
		
		doLightning = true;
	}
	else if (lightningCount > 0)						// Is lightning to strik this frame?
	{	
		lightningCount--;
		hLoc = lightH;
		vLoc = lightV;
		
		doLightning = true;
	}
	
	if (doLightning)
	{
		GenerateLightning(hLoc, vLoc);
		StrikeLightningWork();
	}
}

//--------------------------------------------------------------  HandleCountDownTimer

// This is a pretty boring function.  It is here so that when one level ends,…
// the next one does begin immediately.  It gives the player a few seconds of…
// breathing time.  Essentially, to engage it, we need merely set "countDownTimer"…
// to a positive number.  Each frame the counter gets decremented.  When it…
// reaches zero, the level is advanced to the next wave.

void HandleCountDownTimer (void)
{
	if (countDownTimer == 0)		// If already zero, do nothing.
		return;
	else							// Otherwise, if greater than zero…
	{
		countDownTimer--;			// decrememnt counter.
		if (countDownTimer == 0)	// Did it just hit zero?
		{
			countDownTimer = 0;		// Well, just to be sure (dumb line of code).
			levelOn++;				// Increment the level (wave) we're on.
			UpdateLevelNumbers();	// Display new level on screen.
			SetUpLevel();			// Set up the platforms.
			GenerateEnemies();		// Ready nemesis.
		}
	}
}

//--------------------------------------------------------------  PlayGame

// Here is the "core" of the "game loop".  When a player has elected to…
// begin a game, Glypha falls into this function and remains in a loop…
// herein until the player either quits, or loses their last "bird".
// Each pass through the main loop below constitutes one "frame" of the game.

void PlayGame (void)
{
	Point		offsetPt;
	UnsignedWide thisTime;
	
	offsetPt.h = 0;								// Set up ShieldCursor() point.
	offsetPt.v = 20;


	oldFrameRate = 0;
	Microseconds(&frameTime);
	
	frameTime.lo += 1000000;
		
	do											// Main game loop!!!!
	{
		Microseconds(&thisTime);
		
		if ((thisTime.hi > frameTime.hi) || 
			((thisTime.hi == frameTime.hi) && (thisTime.lo > frameTime.lo)))
		{
			UpdateLevelNumbers();					// update the level, using for frame rate
			oldFrameRate = currentFrameRate;
			currentFrameRate = 0;
			Microseconds(&frameTime);
			frameTime.lo += 1000000;
		}
		else
		{
			currentFrameRate++;
		}
		
		MovePlayer();							// Move the player's bird.
		MoveEnemies();							// Move all sphinx enemies.
		HandleHand();							// Handle the mummy hand (may do nothing).
		HandleEye();							// Handle eye (probably will do nothing).
		HandlePixelShatter();
		HandleScoreFloaters();
		
		HandleLava();
		
		DSpContext_GetBackBuffer( gTheContext, kDSpBufferKind_Normal, &workSrcMap );

		
		DumpBackToWorkMap();					// clear the screen
		DrawFrame();							// Draw the whole scene for this frame.
		HandleLightning();

		DSpContext_SwapBuffers( gTheContext, NULL, NULL );

		evenFrame = !evenFrame;					// Toggle "evenFrame" variable.
		
		GetPlayerInput();						// Get the player's input (keystrokes).
		HandleCountDownTimer();					// Handle countdown (may do nothing).
	}
	while ((playing) && (!pausing));			// Stay in loop until dead, paused or quit.
	
	if ((!playing) && (!quitting))				// If the player died!
	{											// Then play some sweet music.
		PlayExternalSound(kMusicSound, kMusicPriority);
		CheckHighScore();						// And see if they're on the high scores.
	}
	
	MenusReflectMode();							// Set the menus grayed-out state correctly.
	FlushEvents(everyEvent, 0);					// Flush any events in the queue.
}

//--------------------------------------------------------------  CheckHighScore

// This function handles testing to see if the player's score is in the …
// high scores.  If that is the case, the function prompts the user for…
// a name to enter, and sorts and stores off the new score list.

void CheckHighScore (void)
{
	#define		kHighNameDialogID	130
	Str255		placeStr, tempStr;
	DialogPtr	theDial;
	short		i, item;
	Boolean		leaving;
	
	if (theScore > thePrefs.highScores[9])		// To see if on high scores, we need…
	{											// merely see if the last guy is beat out.
		openTheScores = TRUE;					// Will automatically bring up high scores.
												// Play some congratulatory music.
		PlayExternalSound(kBonusSound, kMusicPriority - 1);
		i = 8;									// Find where new score fits in list.
		while ((theScore > thePrefs.highScores[i]) && (i >= 0))
		{										// We'll bump everyone down as we look.
			thePrefs.highScores[i + 1] = thePrefs.highScores[i];
			thePrefs.highLevel[i + 1] = thePrefs.highLevel[i];
			PasStringCopy(thePrefs.highNames[i], thePrefs.highNames[i + 1]);
			i--;
		}
		
		i++;									// i is our place in list (zero based).
		thePrefs.highScores[i] = theScore;		// Pop the new score in place.
		thePrefs.highLevel[i] = levelOn + 1;	// Drop in the new highest level.
		
		NumToString((long)i + 1L, placeStr);	// Convert place to a string to display…
		ParamText(placeStr, "\p", "\p", "\p");	// in the dialog (via ParamText()).
		
		InitCursor();							// Show cursor.
		CenterDialog(kHighNameDialogID);		// Center the dialog and then bring it up.
		theDial = GetNewDialog(kHighNameDialogID, 0L, kPutInFront);
		SetPort((GrafPtr)theDial);
		ShowWindow((GrafPtr)theDial);			// Make dialog visible.
		DrawDefaultButton(theDial);				// Draw outline around "Okay" button.
		FlushEvents(everyEvent, 0);				// Flush any events queued up.
												// Put a default name in text edit box.
		SetDialogString(theDial, 2, thePrefs.highName);
		SelectDialogItemText(theDial, 2, 0, 1024);		// Select the whole text edit string.
		leaving = FALSE;						// Flag for noting when player hit "Okay".
		
		while (!leaving)						// Simple modal dialog loop.
		{
			ModalDialog(0L, &item);				// Use standard filtering.
			
			if (item == 1)						// If player hit the "Okay" button…
			{									// Get the name entered in text edit box.
				GetDialogString(theDial, 2, tempStr);
												// Copy the name into high score list.
				PasStringCopyNum(tempStr, thePrefs.highNames[i], 15);
				PasStringCopy(thePrefs.highNames[i], thePrefs.highName);
				leaving = TRUE;					// We're gone!
			}
		}
		
		DisposeDialog(theDial);					// Clean up.
	}
	else										// But if player didn't get on high scores…
		openTheScores = FALSE;					// no need to rub their face in it.
}


void HandlePixelShatter(void)
{
	int i = 0;
	
	while(i < numPixelShatter)
	{
		if (thePixelShatter[i].duration == 0)
		{
			thePixelShatter[i] = thePixelShatter[numPixelShatter];
			numPixelShatter--;
			continue;
		}
		
		thePixelShatter[i].duration--;
		thePixelShatter[i].h += thePixelShatter[i].hVel;
		thePixelShatter[i].v += thePixelShatter[i].vVel;	
		thePixelShatter[i].hVel += RandomInt(5) - 2;
		thePixelShatter[i].vVel += kGravity + RandomInt(5) - 2;
		
		if (	(thePixelShatter[i].type == kShatterLavaBubble) ||
				(thePixelShatter[i].type == kShatterLavaSplash) ||
				(thePixelShatter[i].type == kShatterLavaBurst))
		{
			thePixelShatter[i].color += RandomInt(3) - 1;
			if (thePixelShatter[i].color > 220)
			{
				thePixelShatter[i].color = 220; 
			}
			else if (thePixelShatter[i].color < 216)
			{
				thePixelShatter[i].color = 216;
			}
		}
		else if ((thePixelShatter[i].type == kShatterPlatformLightning))
		{
			if (thePixelShatter[i].duration > 18)
			{
				thePixelShatter[i].color = 5;
			}
			else if (thePixelShatter[i].duration > 12)
			{
				thePixelShatter[i].color = 47;
			}
			else if (thePixelShatter[i].duration > 6)
			{
				thePixelShatter[i].color = 89;
			}
			else
			{
				thePixelShatter[i].color = 255;
			}
		}
		else if (	(thePixelShatter[i].type == kShatterObeliskTip) ||
				 	(thePixelShatter[i].type == kShatterLightningDust) ||
				 	(thePixelShatter[i].type == kShatterPlayerDeath) ||
				 	(thePixelShatter[i].type == kShatterEnemyDeath) ||
				 	(thePixelShatter[i].type == kShatterPlayerEnemyScrape))
		{
			// white 0
			// yellow 1,2,3,4,5
			// orange 11, 17, 23, 29
			// red 35, ... 216, 218, 220, 222, 223
			// black 255
			
			unsigned char blackBodyTable[17] = {	0,					// white
												1,2,3,4,5,			// yellow (5)
												11,17,23,29,		// orange (4)
												35, 216, 218, 220, 222, 223, // red (6)
												255					// black
											};
											
			if (thePixelShatter[i].duration >= 17)
			{
				thePixelShatter[i].color = 0;
			}
			else
			{
				thePixelShatter[i].color = blackBodyTable[thePixelShatter[i].duration];
			}
		}
		else
		{
			if (thePixelShatter[i].duration > 23)
			{
				thePixelShatter[i].color = 0;
			}
			else
			{
				thePixelShatter[i].color = 255 - (thePixelShatter[i].duration / 2);
			}
		}
		
		thePixelShatter[i].h %= (640 * 16) ;
		
		if ((thePixelShatter[i].h < 0) || (thePixelShatter[i].h >= (640 * 16)) ||
			(thePixelShatter[i].v < 0) || (thePixelShatter[i].v >= (480 * 16)))
		{
			thePixelShatter[i] = thePixelShatter[numPixelShatter];
			numPixelShatter--;
			continue;
		}
		
		i++;
	}
}

void StartPixelShatterRect(Rect *r, short dH, short dV, short type)
{
	StartPixelShatter(r->left, r->top, dH, dV, type);
	StartPixelShatter(r->left, r->bottom, dH, dV, type);
	StartPixelShatter(r->right, r->top, dH, dV, type);
	StartPixelShatter(r->right, r->bottom, dH, dV, type);
	StartPixelShatter((r->left + r->right) / 2, (r->top + r->bottom) / 2, dH, dV, type);	
}

void StartPixelShatter(short h, short v, short hVel, short vVel, short type)
{ 
	signed short hOffset[9] = {0,16,16,0,-16,-16,-16,0,16};
	signed short vOffset[9] = {0,0,-16,-16,-16,0,16,16,16};	
	int itr;
	unsigned char color;
	
	h *= 16;
	v *= 16;

	if ((numPixelShatter + 10) >= kMaxPixelShatter)
	{
		return;
	}
	
	
	if (	(type == kShatterLavaBubble) ||
			(type == kShatterLavaSplash) ||
			(type == kShatterLavaBurst) ||
			(type == kShatterPlayerDeath) ||
			(type == kShatterEnemyDeath))
	{
		color = 215;
	}
	else if (type == kShatterPlatformLightning)
	{
		color = 5;
	}
	else
	{
		color = 0;
	}


	for(itr = 0; itr < 9; itr++)
	{
		thePixelShatter[numPixelShatter].h = h + hOffset[itr];
		thePixelShatter[numPixelShatter].v = v + vOffset[itr];
		thePixelShatter[numPixelShatter].h = h + hOffset[itr] + RandomInt(20) - 10;
		thePixelShatter[numPixelShatter].v = v + vOffset[itr] + RandomInt(20) - 10;
		thePixelShatter[numPixelShatter].hVel = hVel + hOffset[itr];
		thePixelShatter[numPixelShatter].vVel = vVel + vOffset[itr];
		thePixelShatter[numPixelShatter].color = color;
		
		if ((type == kShatterLavaSplash) || (type == kShatterPlatformLightning))
		{
			thePixelShatter[numPixelShatter].duration = 400;
		}
		else if ((type == kShatterPlatformScrape) || (type == kShatterPlatformHit))
		{
			thePixelShatter[numPixelShatter].duration = RandomInt(15) + 10;
		}
		else
		{
			thePixelShatter[numPixelShatter].duration = RandomInt(21) + 20;
		}
		thePixelShatter[numPixelShatter].type = type;
		
		numPixelShatter++;
	}
}

void HandleScoreFloaters(void)
{
	int i = 0;
	
	while(i < numScoreFloater)
	{
		if (theScoreFloater[i].duration == 0)
		{
			theScoreFloater[i] = theScoreFloater[numScoreFloater];
			numScoreFloater--;
			continue;
		}
		
		theScoreFloater[i].location.v -= 2;
		theScoreFloater[i].duration--;
		i++;
	}
}

void StartScoreFloater(unsigned long score, Point where)
{
	if (where.h > 419) { where.h = 419; }
	if (where.v > 459) { where.v = 549; }
	if (where.h < 0) {  where.h = 0;}
	if (where.v < 0) { where.v = 0; }
	
	theScoreFloater[numScoreFloater].score = score;
	theScoreFloater[numScoreFloater].location = where;
	theScoreFloater[numScoreFloater].duration = 20;
	numScoreFloater++;
}