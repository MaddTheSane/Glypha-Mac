/*
	File:		G4Graphics.c

	Contains:	xxx put contents here xxx

	Version:	xxx put version here xxx

	Copyright:	© 1998 by Apple Computer, Inc., all rights reserved.

	File Ownership:

		DRI:				xxx put dri here xxx

		Other Contact:		xxx put other contact here xxx

		Technology:			xxx put technology here xxx

	Writers:

		(sjb)	Steve Bollinger

	Change History (most recent first):

		 <3>	  7/1/98	sjb		Update to CWPro 2
*/


//============================================================================
//----------------------------------------------------------------------------
//									Graphics.c
//----------------------------------------------------------------------------
//============================================================================

// I like to isolate all the graphic routines - put them in their own file.
// This way all the thousands of Rect variables and Pixmaps have a place to go.
// Anyway, this file contains all the drawing routines.

#include "G4Externs.h"
#include <Palettes.h>
#include <QuickdrawText.h>
#include <Fonts.h>
#include <TextUtils.h>
#include <Events.h>

#define kUpperEyeHeight			100
#define kLowerEyeHeight			200
#define kNumLightningPts		8


void QuickUnionRect (Rect *, Rect *, Rect *);
void CheckPlayerWrapAround (void);
void DrawHand (void);
void DrawEye (void);
void DrawPlayer (void);
void CheckEnemyWrapAround (short);
void DrawEnemies (void);
void DrawBanner (void);
void DrawLava(void);

void HandleLava(void);
void HandlePixelShatter(void);

Rect		backSrcRect, workSrcRect, obSrcRect, playerSrcRect;
Rect		numberSrcRect, idleSrcRect, enemyWalkSrcRect, enemyFlySrcRect;
Rect		obeliskRects[4], playerRects[11], numbersSrc[11], numbersDest[11];
Rect		flameSrcRect, flameDestRects[2], flameRects[4], eggSrcRect;
Rect		platformSrcRect, platformCopyRects[9], helpSrcRect, eyeSrcRect, bannerSrcRect;
Rect		helpSrc, helpDest, handSrcRect, handRects[2], eyeRects[4];
Point		leftLightningPts[kNumLightningPts], rightLightningPts[kNumLightningPts];
CGrafPtr	origBackSrcMap, backSrcMap, workSrcMap, obeliskSrcMap, playerSrcMap, eyeSrcMap;
CGrafPtr	numberSrcMap, idleSrcMap, enemyWalkSrcMap, enemyFlySrcMap;
CGrafPtr	flameSrcMap, eggSrcMap, bannerSrcMap, platformSrcMap, helpSrcMap, handSrcMap;
GrafPtr		playerMaskMap, enemyWalkMaskMap, enemyFlyMaskMap, eggMaskMap;
GrafPtr		handMaskMap, eyeMaskMap, obeliskMaskMap;
Boolean		whichList, helpOpen, scoresOpen;

short numPixelShatter;
pixelShatter thePixelShatter[kMaxPixelShatter];

extern	handInfo	theHand;
extern	eyeInfo		theEye;
extern	prefsInfo	thePrefs;
extern	playerType	thePlayer;
extern	enemyType	theEnemies[];
extern	Rect		enemyRects[24];
extern	long		theScore, wasTensOfThousands;
extern	short		livesLeft, levelOn, numEnemies;
extern	Boolean		evenFrame;

extern short lightningCount;
void HandleLightning();


extern short oldFrameRate;

// in DR3 of DrawSprocket the underlay buffer will give the game the
// ability to not worry about keeping track fo the update rects.  As it
// is right now, if you use update rects you have to save the update
// rectangle for each frame, and the number of frames will vary depending
// on the state of triple buffering and page flipping.
// 
// Once the Underlay buffers are operational, enabiling this #define
// will allow DS to optimize the screen updates instead of doing
// a full screen copy (as happens in Glypha)
//#define USE_INVALID_RECTS 1

//==============================================================  Functions
//--------------------------------------------------------------  DrawPlatforms

// This function draws all the platforms on the background pixmap and the…
// work pixmap.  It needs to know merely how many of them to draw.

void DrawPlatforms (short howMany)
{
	if (howMany > 3)			// If there are more than 3 platforms…
	{							// Draw a platform to background pixmap.
		CopyBits(&((GrafPtr)platformSrcMap)->portBits, 
				&((GrafPtr)backSrcMap)->portBits, 
				&platformCopyRects[2], &platformCopyRects[7], srcCopy, 0L);
		
								// Draw a platform to work pixmap.
		CopyBits(&((GrafPtr)backSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&platformCopyRects[7], &platformCopyRects[7], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &platformCopyRects[7] );
		#endif

								// Add rectangle to update list to be drawn to screen.
								// Ditto for a second platform.
		CopyBits(&((GrafPtr)platformSrcMap)->portBits, 
				&((GrafPtr)backSrcMap)->portBits, 
				&platformCopyRects[4], &platformCopyRects[8], srcCopy, 0L);
		CopyBits(&((GrafPtr)backSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&platformCopyRects[8], &platformCopyRects[8], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &platformCopyRects[8] );
		#endif

	}
	else						// If there are 3 or less platforms…
	{
		CopyBits(&((GrafPtr)platformSrcMap)->portBits, 
				&((GrafPtr)backSrcMap)->portBits, 
				&platformCopyRects[3], &platformCopyRects[7], srcCopy, 0L);
		CopyBits(&((GrafPtr)backSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&platformCopyRects[7], &platformCopyRects[7], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &platformCopyRects[7] );
		#endif

		
		CopyBits(&((GrafPtr)platformSrcMap)->portBits, 
				&((GrafPtr)backSrcMap)->portBits, 
				&platformCopyRects[5], &platformCopyRects[8], srcCopy, 0L);
		CopyBits(&((GrafPtr)backSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&platformCopyRects[8], &platformCopyRects[8], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &platformCopyRects[8] );
		#endif

	}
	
	if (howMany > 5)		// If there are more than 5 platforms…
	{
		CopyBits(&((GrafPtr)platformSrcMap)->portBits, 
				&((GrafPtr)backSrcMap)->portBits, 
				&platformCopyRects[0], &platformCopyRects[6], srcCopy, 0L);
		CopyBits(&((GrafPtr)backSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&platformCopyRects[6], &platformCopyRects[6], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &platformCopyRects[6] );
		#endif

	}
	else					// If there are 5 or less platforms…
	{
		CopyBits(&((GrafPtr)platformSrcMap)->portBits, 
				&((GrafPtr)backSrcMap)->portBits, 
				&platformCopyRects[1], &platformCopyRects[6], srcCopy, 0L);
		CopyBits(&((GrafPtr)backSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&platformCopyRects[6], &platformCopyRects[6], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &platformCopyRects[6] );
		#endif

	}
}

//--------------------------------------------------------------  ScrollHelp

// This function scrolls the help screen.  You pass it a number of pixels…
// to scroll up or down (positive or negative number).

void ScrollHelp (short scrollDown)
{
	// don't do any drawing, drawing is done by the idle animation automatically
	
	OffsetRect(&helpSrc, 0, scrollDown);		// Move the source rectangle.
	
	if (helpSrc.bottom > 398)					// Check to see we don't go too far.
	{
		helpSrc.bottom = 398;
		helpSrc.top = helpSrc.bottom - 199;
	}
	else if (helpSrc.top < 0)
	{
		helpSrc.top = 0;
		helpSrc.bottom = helpSrc.top + 199;
	}
}

//--------------------------------------------------------------  OpenHelp

// Bring up the help screen.  This is a kind of "wipe" or "barn door" effect.

void OpenHelp (void)
{
	Rect		wallSrc, wallDest;
	short		i;
	
	SetRect(&helpSrc, 0, 0, 231, 0);	// Initialize source and destination rects.
	helpDest = helpSrc;
	OffsetRect(&helpDest, 204, 171);
	
	SetRect(&wallSrc, 0, 0, 231, 199);
	OffsetRect(&wallSrc, 204, 171);
	wallDest = wallSrc;
	
	for (i = 0; i < 199; i ++)			// Loop through 1 pixel at a time.
	{
		DSpContext_GetBackBuffer( gTheContext, kDSpBufferKind_Normal, &workSrcMap );

		DumpBackToWorkMap();					// clear the screen
		HandleLava();
		HandlePixelShatter();
		DrawTorches();
		DrawBanner();
		DrawLava();
		DrawPixelShatter();

		LogNextTick(1L);				// Speed governor.
		helpSrc.bottom++;				// Grow help source rect.
		helpDest.bottom++;				// Grow help dest as well.
		wallSrc.bottom--;				// Shrink wall source.
		wallDest.top++;					// Shrink wall dest.
		
										// So, as the help graphic grows, the wall graphic…
										// shrinks.  Thus it is as though the wall is…
										// lifting up to expose the help screen beneath.
		
		CopyBits(	&((GrafPtr)helpSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&helpSrc, &helpDest, srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &helpDest );
		#endif


		CopyBits(	&((GrafPtr)backSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&wallSrc, &wallDest, srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &wallDest );
		#endif

										// Copy slightly larger help screen.

		DSpContext_SwapBuffers( gTheContext, NULL, NULL );
	}
	helpOpen = TRUE;					// When done, set flag to indicate help is open.
}

//--------------------------------------------------------------  CloseWall

// Close the wall over whatever screen is up (help screen or high scores).
// Since the wall just comes down over the opening - covering whatever was beneath,…
// it's simpler than the above function.

void CloseWall (void)
{
	DSpContext_GetBackBuffer( gTheContext, kDSpBufferKind_Normal, &workSrcMap );
	DumpBackToWorkMap();					// clear the screen
	DSpContext_SwapBuffers( gTheContext, NULL, NULL );
}

//--------------------------------------------------------------  OpenHighScores

// This function is practically identical to the OpenHelp().  The only real…
// difference is that we must first draw all the high scores offscreen before…
// lifting the wall to reveal them.

void OpenHighScores (void)
{
	RGBColor	theRGBColor, wasColor;
	Rect		wallSrc, wallDest;
	Rect		scoreSrc, scoreDest;
	Str255		scoreStr;
	short		i, scoreWide;
	
	SetRect(&scoreSrc, 0, 0, 231, 0);				// Initialize source and dest rects.
	OffsetRect(&scoreSrc, 204, 171);
	scoreDest = scoreSrc;
	
	SetRect(&wallSrc, 0, 0, 231, 199);
	OffsetRect(&wallSrc, 204, 171);
	wallDest = wallSrc;
	
	SetPort((GrafPtr)workSrcMap);					// We'll draw scores to the work pixmap.
	PaintRect(&wallSrc);							// Paint it black.
	
	GetForeColor(&wasColor);						// Save the foreground color.
	
	TextFont(kFontIDGeneva);						// Use Geneva 12 point Bold font.
	TextSize(12);
	TextFace(bold);
	
	Index2Color(132, &theRGBColor);					// Get the 132nd color in RGB form.
	RGBForeColor(&theRGBColor);						// Make this color the pen color.
	MoveTo(scoreSrc.left + 36, scoreSrc.top + 20);	// Get pen in right position to draw.
	DrawString("\pGlypha IV High Scores");			// Draw the title.
	
	TextFont(kFontIDGeneva);						// Use Geneva 9 point Bold font.
	TextSize(9);
	TextFace(bold);
	
	for (i = 0; i < 10; i++)						// Walk through all 10 high scores.
	{
		Index2Color(133, &theRGBColor);				// Use color 133 (in palette).
		RGBForeColor(&theRGBColor);
		NumToString((long)i + 1L, scoreStr);		// Draw "place" (1, 2, 3, …).
		MoveTo(scoreSrc.left + 8, scoreSrc.top + 40 + (i * 16));
		DrawString(scoreStr);
		
		Index2Color(128, &theRGBColor);				// Use color 128 (from palette).
		RGBForeColor(&theRGBColor);
		MoveTo(scoreSrc.left + 32, scoreSrc.top + 40 + (i * 16));
		DrawString(thePrefs.highNames[i]);			// Draw the high score name (Sue, …).
		
		Index2Color(164, &theRGBColor);				// Use color 164 (from palette).
		RGBForeColor(&theRGBColor);
		NumToString(thePrefs.highScores[i], scoreStr);
		scoreWide = StringWidth(scoreStr);			// Right justify.
		MoveTo(scoreSrc.left + 191 - scoreWide, scoreSrc.top + 40 + (i * 16));
		DrawString(scoreStr);						// Draw the high score (12,000, …).
		
		Index2Color(134, &theRGBColor);				// Use color 134 (from palette).
		RGBForeColor(&theRGBColor);
		NumToString(thePrefs.highLevel[i], scoreStr);
		scoreWide = StringWidth(scoreStr);			// Right justify.
		MoveTo(scoreSrc.left + 223 - scoreWide, scoreSrc.top + 40 + (i * 16));
		DrawString(scoreStr);						// Draw highest level (12, 10, …).
	}
	
	RGBForeColor(&wasColor);						// Restore foreground color.

	for (i = 0; i < 199; i ++)						// Now the standard scroll functions.
	{
		WaitForNextTick();
		
		DSpContext_GetBackBuffer( gTheContext, kDSpBufferKind_Normal, &workSrcMap );
		DumpBackToWorkMap();					// clear the screen
	
		LogNextTick(1L);				// Speed governor.
		scoreSrc.bottom++;
		scoreDest.bottom++;
		wallSrc.bottom--;
		wallDest.top++;
		
										// So, as the help graphic grows, the wall graphic…
										// shrinks.  Thus it is as though the wall is…
										// lifting up to expose the help screen beneath.
		
		CopyBits(	&((GrafPtr)workSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&scoreSrc, &scoreDest, srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &scoreDest );
		#endif


		CopyBits(	&((GrafPtr)backSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&wallSrc, &wallDest, srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &wallDest );
		#endif

										// Copy slightly larger help screen.

		DSpContext_SwapBuffers( gTheContext, NULL, NULL );
	}
	
	scoresOpen = TRUE;								// Flag that the scores are up.
}

//--------------------------------------------------------------  UpdateLivesNumbers

// During a game, this function is called to reflect the current number of lives.
// This is "lives remaining", so 1 is subtracted before displaying it to the screen.
// The lives is "wrapped around" after 99.  So 112 lives will display as 12.  It's…
// a lot easier to handle numbers this way (it beats a recursive function that might…
// potentially draw across the entire screen.

void UpdateLivesNumbers (void)
{
	short		digit;
	
	digit = (livesLeft - 1) / 10;		// Get the "10's" digit.
	digit = digit % 10L;				// Keep it less than 10 (0 -> 9).
	if ((digit == 0) && ((livesLeft - 1) < 10))
		digit = 10;						// Use a "blank" space if zero and less than 10.
										// Draw digit.
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[0], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[0] );
	#endif


	digit = (livesLeft - 1) % 10;		// Get 1's digit.
										// Draw digit.
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[1], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[1] );
	#endif

}

//--------------------------------------------------------------  UpdateScoreNumbers

// This function works just like the above function.  However, we allow the…
// score to go to 6 digits (999,999) before rolling over.  Note however, that…
// in both the case of the score, number of lives, etc., the game does in fact…
// keep track of the "actual" number.  It is just that only so many digits are…
// being displayed.

void UpdateScoreNumbers (void)
{
	long		digit;

	digit = theScore / 100000L;		// Get "hundreds of thousands" digit
	digit = digit % 10L;			// Clip off anything greater than 9.
	if ((digit == 0) && (theScore < 1000000L))
		digit = 10;					// Use blank space if zero.
									// Draw digit.
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[2], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[2] );
	#endif


	digit = theScore / 10000L;		// Get "tens of thousands" digit.
	if (digit > wasTensOfThousands)	// Check for "extra life" here.
	{
		livesLeft++;				// Increment number of lives.
		UpdateLivesNumbers();		// Reflect new lives on screen.
		wasTensOfThousands = digit;	// Note that life was given.
	}
	digit = digit % 10L;			// Clip off anything greater than 9.
	if ((digit == 0) && (theScore < 100000L))
		digit = 10;					// Use blank space if zero.
									// Draw digit.
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[3], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[3] );
	#endif

	
	digit = theScore / 1000L;		// Handle "thousands" digit.
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 10000L))
		digit = 10;
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[4], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[4] );
	#endif


	digit = theScore / 100L;		// Handle 100's digit.
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 1000L))
		digit = 10;
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[5], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[5] );
	#endif


	digit = theScore / 10L;			// Handle 10's digit.
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 100L))
		digit = 10;
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[6], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[6] );
	#endif

	
	digit = theScore % 10L;			// Handle 1's digit.
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[7], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[7] );
	#endif
}

//--------------------------------------------------------------  UpdateLevelNumbers

// Blah, blah, blah.  Just like the above functions but handles displaying the…
// level the player is on.  We allow 3 digits here (up to 999) before wrapping.

void UpdateLevelNumbers (void)
{
	short		digit;
	short		workNumber;
	
//	workNumber = levelOn;
	workNumber = oldFrameRate;
		
	digit = (workNumber + 1) / 100;		// Do 100's digit.
	digit = digit % 10L;
	if ((digit == 0) && ((workNumber + 1) < 1000))
		digit = 10;
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[8], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[3] );
	#endif
	
	digit = (workNumber + 1) / 10;			// Do 10's digit.
	digit = digit % 10L;
	if ((digit == 0) && ((workNumber + 1) < 100))
		digit = 10;
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[9], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[9] );
	#endif
	
	digit = (workNumber + 1) % 10;			// Do 1's digit.
	CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
			&((GrafPtr)backSrcMap)->portBits, 
			&numbersSrc[digit], &numbersDest[10], srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &numbersDest[10] );
	#endif
}

//--------------------------------------------------------------  GenerateLightning

// This function takes a point (h and v) and then generates two lightning bolts…
// (one from the tip of each obelisk) to the point.  It does this by generating…
// a list of segments (as the lightning is broken up into segements).  The drawing…
// counterpart to this function will draw a line connecting these segements (a sort…
// of dot-to-dot).

void GenerateLightning (short h, short v)
{
	#define kLeftObeliskH		172
	#define kLeftObeliskV		250
	#define kRightObeliskH		468
	#define kRightObeliskV		250
	#define kWander				16
	
	short		i, leftDeltaH, rightDeltaH, leftDeltaV, rightDeltaV, range;
	
	leftDeltaH = h - kLeftObeliskH;				// Determine the h and v distances between…
	rightDeltaH = h - kRightObeliskH;			// obelisks and the target point.
	leftDeltaV = v - kLeftObeliskV;
	rightDeltaV = v - kRightObeliskV;
	
	for (i = 0; i < kNumLightningPts; i++)		// Calculate an even spread of points between…
	{											// obelisk tips and the target point.
		leftLightningPts[i].h = (leftDeltaH * i) / (kNumLightningPts - 1) + kLeftObeliskH;
		leftLightningPts[i].v = (leftDeltaV * i) / (kNumLightningPts - 1) + kLeftObeliskV;
		rightLightningPts[i].h = (rightDeltaH * i) / (kNumLightningPts - 1) + kRightObeliskH;
		rightLightningPts[i].v = (rightDeltaV * i) / (kNumLightningPts - 1) + kRightObeliskV;
	}
	
	range = kWander * 2 + 1;					// Randomly scatter the points vertically…
	for (i = 1; i < kNumLightningPts - 1; i++)	// but NOT the 1st or last points.
	{
		leftLightningPts[i].v += RandomInt(range) - kWander;
		rightLightningPts[i].v += RandomInt(range) - kWander;
	}
}

void DrawObelisks (void)
{	
	if ((lightningCount > 0) && evenFrame)		// Draw them "inverted"
	{
		CopyMask(	&((GrafPtr)obeliskSrcMap)->portBits,		// src
					&((GrafPtr)obeliskMaskMap)->portBits,		// mask
					&((GrafPtr) workSrcMap)->portBits, 			// dst
					&obeliskRects[0], 							// src rect
					&obeliskRects[0], 							// mask rect
					&obeliskRects[2]);							// dst rect
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &obeliskRects[2] );
		#endif

		CopyMask(	&((GrafPtr)obeliskSrcMap)->portBits,		// src
					&((GrafPtr)obeliskMaskMap)->portBits,		// mask
					&((GrafPtr) workSrcMap)->portBits, 			// dst
					&obeliskRects[1], 							// src rect
					&obeliskRects[1], 							// mask rect
					&obeliskRects[3]);							// dst rect
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &obeliskRects[3] );
		#endif
	}
	else
	{
		CopyMask(	&((GrafPtr)backSrcMap)->portBits,		// src
					&((GrafPtr)obeliskMaskMap)->portBits,		// mask
					&((GrafPtr) workSrcMap)->portBits, 			// dst
					&obeliskRects[2], 							// src rect
					&obeliskRects[0], 							// mask rect
					&obeliskRects[2]); 							// dst rect
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &obeliskRects[2] );
		#endif

		CopyMask(	&((GrafPtr)backSrcMap)->portBits,		// src
					&((GrafPtr)obeliskMaskMap)->portBits,		// mask
					&((GrafPtr) workSrcMap)->portBits, 			// dst
					&obeliskRects[3], 							// src rect
					&obeliskRects[1], 							// mask rect
					&obeliskRects[3]);							// dst rect
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &obeliskRects[3] );
		#endif
	}
}


void StrikeLightningWork (void)
{
	short		i;
	GrafPtr		oldPort;
	RGBColor 	yellow = {65535, 65535, 0};
	Rect		theRect;
	
	GetPort(&oldPort);
	SetPort((GrafPtr)workSrcMap);
	PenSize(1, 2);								// Use a tall pen.
	RGBForeColor(&yellow);
												// Draw lightning bolts with inverted pen.
	MoveTo(leftLightningPts[0].h, leftLightningPts[0].v);
	for (i = 0; i < kNumLightningPts - 1; i++)	// Draw left lightning bolt.
	{
		MoveTo(leftLightningPts[i].h, leftLightningPts[i].v);
		LineTo(leftLightningPts[i + 1].h - 1, leftLightningPts[i + 1].v);
	}
	SetRect( &theRect, leftLightningPts[0].h, leftLightningPts[0].v, 
		leftLightningPts[kNumLightningPts].h - 1, leftLightningPts[kNumLightningPts].v );
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &theRect );
	#endif
	
	MoveTo(rightLightningPts[0].h, rightLightningPts[0].v);
	for (i = 0; i < kNumLightningPts - 1; i++)	// Draw right lightning bolt.
	{
		MoveTo(rightLightningPts[i].h, rightLightningPts[i].v);
		LineTo(rightLightningPts[i + 1].h - 1, rightLightningPts[i + 1].v);
	}
	SetRect( &theRect, rightLightningPts[0].h, rightLightningPts[0].v, 
		rightLightningPts[kNumLightningPts].h - 1, rightLightningPts[kNumLightningPts].v );
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &theRect );
	#endif
	
	PenNormal();								// Return pen to normal.
	
	SetPort(oldPort);
}

//--------------------------------------------------------------  DumpBackToWorkMap

// Simple handy function that copies the entire background pixmap to the…
// work pixmap.
//--

//--------------------------------------------------------------  DumpBackToWorkMap

// Simple handy function that copies the entire background pixmap to the…
// work pixmap.


static inline void ClassicDumpBackToWork(void)
{
	// this is doing a full clean of the work map from the source map, don't inval
	// this rect.
	//
	// • this will go away once DrawSprocket Underlays are used (in DR3 release)
	CopyBits(&((GrafPtr)backSrcMap)->portBits, 
			&((GrafPtr)workSrcMap)->portBits, 
			&backSrcRect, &backSrcRect, srcCopy, 0L);
}

// faster on some machines, on others it is slow because of dcbz
static inline void FastDumpBackToWorkMap (void)
{
	CGrafPtr srcGrafPtr = backSrcMap;
	PixMap *srcMap = *(srcGrafPtr->portPixMap);
	char *src = srcMap->baseAddr;
	UInt32 srcBump = srcMap->rowBytes & 0x3fff;
	
	CGrafPtr dstGrafPtr = workSrcMap;
	PixMap *dstMap = *(dstGrafPtr->portPixMap);
	char *dst = dstMap->baseAddr;
	UInt32 dstBump = dstMap->rowBytes & 0x3fff;
	
	int count;
	
	for(count = 0; count < 480; count++)
	{
		BlockMoveData(src,dst,640);
		
		src += srcBump;
		dst += dstBump;
	}
}

void DumpBackToWorkMap(void)
{	
	ClassicDumpBackToWork();
}

//--------------------------------------------------------------  QuickUnionRect

// The Mac Toolbox gives you a UnionRect() function, but, like any Toolbox…
// routine, if we can do it faster, we ought to.  Well, the function below…
// is quick because (among other reasons), it assumes that the two rects…
// being compared are the same size.
//
// Notes from Cary Farrier: Sorry John I don't agree with that advice.  You
// may be able to do something faster, but the speed gain you achieve may not
// be worth the effort required to develop, test, and debug the code.  John's
// advice is good for some scenarios, but it is the exception and not the
// rule.
void QuickUnionRect (Rect *rect1, Rect *rect2, Rect *whole)
{
	if (rect1->left < rect2->left)		// See if we're to use rect1's left.
	{
		whole->left = rect1->left;
		whole->right = rect2->right;
	}
	else								// Use rect2's left.
	{
		whole->left = rect2->left;
		whole->right = rect1->right;
	}
	
	if (rect1->top < rect2->top)		// See if we're to use rect1's top.
	{
		whole->top = rect1->top;
		whole->bottom = rect2->bottom;
	}
	else								// Use rect2's top.
	{
		whole->top = rect2->top;
		whole->bottom = rect1->bottom;
	}
}

//--------------------------------------------------------------  CheckPlayerWrapAround

// This handles drawing wrap-around.  It is such that, when a player walks partly…
// off the right edge of the screen, you see the player peeking through on the left…
// side of the screen.  Since we can't (shouldn't) assume that the physical screen…
// memory wraps around, we'll draw the right player clipped against the right edge…
// of the screen and draw a SECOND PLAYER on the left edge (clipped to the left).

void CheckPlayerWrapAround (void)
{
	Rect		wrapRect, wasWrapRect, src;
	
	if (thePlayer.dest.right > 640)		// Player off right edge of screen.
	{
		thePlayer.wrapping = TRUE;		// Set "wrapping" flag.
		wrapRect = thePlayer.dest;		// Start out with copy of player bounds.
		wrapRect.left -= 640;			// Offset it a screenwidth to left.
		wrapRect.right -= 640;
										// Ditto with old location.
		wasWrapRect = thePlayer.wasDest;
		wasWrapRect.left -= 640;
		wasWrapRect.right -= 640;
		
		if (thePlayer.mode == kBones)	// Draw second bones.
		{
			src = playerRects[thePlayer.srcNum];
			src.bottom = src.top + thePlayer.frame;
			CopyMask(&((GrafPtr)playerSrcMap)->portBits, 
					&((GrafPtr)playerMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&src, &src, &wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
		else							// Draw second player (not bones).
		{
			CopyMask(&((GrafPtr)playerSrcMap)->portBits, 
					&((GrafPtr)playerMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&playerRects[thePlayer.srcNum], 
					&playerRects[thePlayer.srcNum], 
					&wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
		thePlayer.wrap = wrapRect;
	}
	else if (thePlayer.dest.left < 0)	// Else if off the left edge…
	{
		thePlayer.wrapping = TRUE;		// Set "wrapping" flag.
		wrapRect = thePlayer.dest;		// Start out with copy of player bounds.
		wrapRect.left += 640;			// Offset it a screenwidth to right.
		wrapRect.right += 640;
										// Ditto with old location.
		wasWrapRect = thePlayer.wasDest;
		wasWrapRect.left += 640;
		wasWrapRect.right += 640;
		
		if (thePlayer.mode == kBones)	// Draw second bones.
		{
			src = playerRects[thePlayer.srcNum];
			src.bottom = src.top + thePlayer.frame;
			CopyMask(&((GrafPtr)playerSrcMap)->portBits, 
					&((GrafPtr)playerMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&src, &src, &wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
		else							// Draw second player (not bones).
		{
			CopyMask(&((GrafPtr)playerSrcMap)->portBits, 
					&((GrafPtr)playerMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&playerRects[thePlayer.srcNum], 
					&playerRects[thePlayer.srcNum], 
					&wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
		thePlayer.wrap = wrapRect;
	}
	else
		thePlayer.wrapping = FALSE;		// Otherwise, we're not wrapping.
}

//--------------------------------------------------------------  DrawTorches

// This handles drawing the two torch's flames.  It chooses randomly from…
// 4 torch graphics and draws right over the old torches.

void DrawTorches (void)
{
	short		who;
	
	who = RandomInt(4);
	if (evenFrame)		// Only draw 1 torch - left on even frames…
	{
		CopyBits(&((GrafPtr)flameSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&flameRects[who], &flameDestRects[0], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &flameDestRects[0] );
		#endif
	}
	else				// and draw the right torch on odd frames.
	{					// We do this even/odd thing for speed.  Why draw both?
		CopyBits(&((GrafPtr)flameSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&flameRects[who], &flameDestRects[1], srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &flameDestRects[1] );
		#endif
	}
}

//--------------------------------------------------------------  DrawHand

// This function takes care of drawing the hand offscreen.  There are only…
// two (well really three) choices - hand open, hand clutching (or no hand…
// in which case both options are skipped).

void DrawHand (void)
{
	theHand.dest.right = theHand.dest.left + 56;
	theHand.dest.bottom = theHand.dest.top + 57;

	if (theHand.mode == kOutGrabeth)		// Fingers open.
	{
		CopyMask(&((GrafPtr)handSrcMap)->portBits, 
				&((GrafPtr)handMaskMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&handRects[0], 
				&handRects[0], 
				&theHand.dest);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &theHand.dest );
		#endif
	}
	else if (theHand.mode == kClutching)	// Fingers clenched.
	{
		CopyMask(&((GrafPtr)handSrcMap)->portBits, 
				&((GrafPtr)handMaskMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&handRects[1], 
				&handRects[1], 
				&theHand.dest);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &theHand.dest );
		#endif
	}
}

//--------------------------------------------------------------  DrawEye

// This function draws the eye (if it's floating about - stalking).

void DrawEye (void)
{
	if (theEye.mode == kStalking)
	{
		CopyMask(&((GrafPtr)eyeSrcMap)->portBits, 
				&((GrafPtr)eyeMaskMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&eyeRects[theEye.srcNum], 
				&eyeRects[theEye.srcNum], 
				&theEye.dest);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &theEye.dest );
		#endif
	}
}

//--------------------------------------------------------------  DrawPlayer

// Although called "DrawPlayer()", this function actually does its drawing…
// offscreen.  It is the above routine that will finally copy our offscreen…
// work to the main screen.  Anyway, the below function draws the player…
// offscreen in the correct position and state.

void DrawPlayer (void)
{
	Rect		src;
	
	if ((evenFrame) && (thePlayer.mode == kIdle))
	{			// On even frames, we'll draw the "flashed" graphic of the player.
				// If you've played Glypha, you notice that the player begins a…
				// game flashing alternately between bones and a normal player.
		CopyMask(&((GrafPtr)idleSrcMap)->portBits, 
				&((GrafPtr)playerMaskMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&idleSrcRect, 
				&playerRects[thePlayer.srcNum], 
				&thePlayer.dest);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &thePlayer.dest );
		#endif
	}
	else if (thePlayer.mode == kBones)
	{			// If the player is dead and a pile of bones…
		src = playerRects[thePlayer.srcNum];
		src.bottom = src.top + thePlayer.frame;
		CopyMask(&((GrafPtr)playerSrcMap)->portBits, 
				&((GrafPtr)playerMaskMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&src, &src, &thePlayer.dest);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &thePlayer.dest );
		#endif
	}
	else		// Else, if the player is neither idle nor dead…
	{
		CopyMask(&((GrafPtr)playerSrcMap)->portBits, 
				&((GrafPtr)playerMaskMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&playerRects[thePlayer.srcNum], 
				&playerRects[thePlayer.srcNum], 
				&thePlayer.dest);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &thePlayer.dest );
		#endif
	}
				// Now we add the player to the update rect list.
				// Record old locations.
	thePlayer.wasH = thePlayer.h;
	thePlayer.wasV = thePlayer.v;
				// Record old bounds rect.
	thePlayer.wasDest = thePlayer.dest;
}

//--------------------------------------------------------------  CheckEnemyWrapAround

// This function both determines whether or not an enemy (sphinx) is wrapping around.
// If it is, the "second" wrapped-around enemy is drawn.

void CheckEnemyWrapAround (short who)
{
	Rect		wrapRect, wasWrapRect, src;
	
	if (theEnemies[who].dest.right > 640)	// Is enemy off the right edge of screen?
	{
		wrapRect = theEnemies[who].dest;	// Copy bounds.
		wrapRect.left -= 640;				// Offset bounds copy to left (one screen width).
		wrapRect.right -= 640;
											// Ditto with old bounds.
		wasWrapRect = theEnemies[who].wasDest;
		wasWrapRect.left -= 640;
		wasWrapRect.right -= 640;
											// Handle "egg" enemies.
		if ((theEnemies[who].mode == kFalling) || (theEnemies[who].mode == kEggTimer))
		{									// Handle "egg" enemy sinking into platform.
			if ((theEnemies[who].mode == kEggTimer) && (theEnemies[who].frame < 24))
			{
				src = eggSrcRect;
				src.bottom = src.top + theEnemies[who].frame;
			}
			else
				src = eggSrcRect;
			CopyMask(&((GrafPtr)eggSrcMap)->portBits, 
					&((GrafPtr)eggMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&src, &src, &wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
		else								// Otherwise, if enemy not an egg…
		{
			CopyMask(&((GrafPtr)enemyFlySrcMap)->portBits, 
					&((GrafPtr)enemyFlyMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&enemyRects[theEnemies[who].srcNum], 
					&enemyRects[theEnemies[who].srcNum], 
					&wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
	}
	else if (theEnemies[who].dest.left < 0)	// Check to see if enemy off left edge instead.
	{
		wrapRect = theEnemies[who].dest;	// Make a copy of enemy bounds.
		wrapRect.left += 640;				// Offset it right one screens width.
		wrapRect.right += 640;
											// Ditto with old bounds.
		wasWrapRect = theEnemies[who].wasDest;
		wasWrapRect.left += 640;
		wasWrapRect.right += 640;
		if ((theEnemies[who].mode == kFalling) || (theEnemies[who].mode == kEggTimer))
		{									// Blah, blah, blah.  This is just like the above.
			if ((theEnemies[who].mode == kEggTimer) && (theEnemies[who].frame < 24))
			{
				src = eggSrcRect;
				src.bottom = src.top + theEnemies[who].frame;
			}
			else
				src = eggSrcRect;
			CopyMask(&((GrafPtr)eggSrcMap)->portBits, 
					&((GrafPtr)eggMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&src, &src, &wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
		else
		{
			CopyMask(&((GrafPtr)enemyFlySrcMap)->portBits, 
					&((GrafPtr)enemyFlyMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&enemyRects[theEnemies[who].srcNum], 
					&enemyRects[theEnemies[who].srcNum], 
					&wrapRect);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &wrapRect );
			#endif
		}
	}
}

//--------------------------------------------------------------  DrawEnemies

// This function draws all the sphinx enemies (or eggs if they're in that state).
// It doesn't handle wrap-around (the above function does) but it does call it.

void DrawEnemies (void)
{
	Rect		src;
	short		i;
	
	for (i = 0; i < numEnemies; i++)	// Go through all enemies.
	{
		switch (theEnemies[i].mode)		// Handle the different modes as seperate cases.
		{
			case kSpawning:				// Spawning enemies are "growing" out of the platform.
			src = enemyRects[theEnemies[i].srcNum];
			src.bottom = src.top + theEnemies[i].frame;
			CopyMask(&((GrafPtr)enemyWalkSrcMap)->portBits, 
					&((GrafPtr)enemyWalkMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&src, &src, &theEnemies[i].dest);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &theEnemies[i].dest );
			#endif
					
										// Don't need to check wrap-around, when enemies…
										// spawn, they're never on the edge of screen.
			theEnemies[i].wasDest = theEnemies[i].dest;
			theEnemies[i].wasH = theEnemies[i].h;
			theEnemies[i].wasV = theEnemies[i].v;
			break;
			
			case kFlying:				// Flying enemies are air borne (gee).
			CopyMask(&((GrafPtr)enemyFlySrcMap)->portBits, 
					&((GrafPtr)enemyFlyMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&enemyRects[theEnemies[i].srcNum], &enemyRects[theEnemies[i].srcNum], 
					&theEnemies[i].dest);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &theEnemies[i].dest );
			#endif
					
			CheckEnemyWrapAround(i);	// I like the word "air bourne".
			theEnemies[i].wasDest = theEnemies[i].dest;
			theEnemies[i].wasH = theEnemies[i].h;
			theEnemies[i].wasV = theEnemies[i].v;
			break;
			
			case kWalking:				// Walking enemies are walking.  Enemies.
			CopyMask(&((GrafPtr)enemyWalkSrcMap)->portBits, 
					&((GrafPtr)enemyWalkMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&enemyRects[theEnemies[i].srcNum], &enemyRects[theEnemies[i].srcNum], 
					&theEnemies[i].dest);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &theEnemies[i].dest );
			#endif
										// Don't need to check wrap-around, enemies walk…
										// only briefly, and never off edge of screen.
			theEnemies[i].wasDest = theEnemies[i].dest;
			theEnemies[i].wasH = theEnemies[i].h;
			theEnemies[i].wasV = theEnemies[i].v;
			break;
			
			case kFalling:				// Falling enemies are in fact eggs!
			CopyMask(&((GrafPtr)eggSrcMap)->portBits, 
					&((GrafPtr)eggMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&eggSrcRect, &eggSrcRect, &theEnemies[i].dest);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &theEnemies[i].dest );
			#endif
					
			CheckEnemyWrapAround(i);	// Check for wrap around.
			theEnemies[i].wasDest = theEnemies[i].dest;
			theEnemies[i].wasH = theEnemies[i].h;
			theEnemies[i].wasV = theEnemies[i].v;
			break;
			
			case kEggTimer:				// These are idle, perhaps hatching, eggs.
			if (theEnemies[i].frame < 24)
			{							// Below countdown = 24, the egss are sinking…
				src = eggSrcRect;		// into the platform (hatch time!).
				src.bottom = src.top + theEnemies[i].frame;
			}
			else
				src = eggSrcRect;
			CopyMask(&((GrafPtr)eggSrcMap)->portBits, 
					&((GrafPtr)eggMaskMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&src, &src, &theEnemies[i].dest);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &theEnemies[i].dest );
			#endif
					
			CheckEnemyWrapAround(i);	// Check for wrap around.
			theEnemies[i].wasDest = theEnemies[i].dest;
			theEnemies[i].wasH = theEnemies[i].h;
			theEnemies[i].wasV = theEnemies[i].v;
			break;
		}
	}
}

//------------- DrawBanner
void DrawBanner (void)
{
	Rect destRect;
	Rect srcRect;
	long start = (TickCount() / 2) % 1280;
	long seg1Len;
	long seg2Len;
	
	if (start <= 640) 
	{
		seg1Len = 640;
		seg2Len = 0;
	}
	else
	{
		seg1Len = 1280 - start;
		seg2Len = 640 - seg1Len;
	}
	
	SetRect(&destRect,
			0,		// left
			460,	// top
			seg1Len,	// right
			480);	// bottom
	
			
	SetRect(&srcRect,
			start,
			0,
			start + seg1Len,
			20);
			
			
	CopyBits(	&((GrafPtr)bannerSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&srcRect, &destRect, srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &destRect );
	#endif
				
	if (seg2Len > 0)
	{
		destRect.left = seg1Len;
		destRect.right = 640;
		srcRect.left = 0;
		srcRect.right = seg2Len;
		
		CopyBits(	&((GrafPtr)bannerSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&srcRect, &destRect, srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &destRect );
	#endif
	}

}

//--------------------------------------------------------------  DrawPixelShatter

void DrawPixelShatter (void)
{
	int i;
	CGrafPtr theGrafPtr = workSrcMap;
	PixMap *theMap = *(theGrafPtr->portPixMap);
	short h,v;
	short rowBytes = theMap->rowBytes & 0x3fff;
	short minH = 1000, minV = 1000, maxH = 0, maxV = 0;
	Rect theRect;
	
	for(i = 0; i < numPixelShatter; i++)
	{
		char *addr = theMap->baseAddr;
		int offset = 0;
				
		h = thePixelShatter[i].h;
		v = thePixelShatter[i].v;
		if( h < minH )
			minH = h;
		if( h > maxH )
			maxH = h;
		if( v < minV )
			minV = v;
		if( v > maxV )
			maxV = v;
			
		h >>= 4;
		v >>= 4;
		
		offset = rowBytes * v;
		offset += h;
		
		addr += offset;
		
		*addr = thePixelShatter[i].color;
	}
	
	SetRect( &theRect, minH, minV, maxH, maxV );
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &theRect );
	#endif
	
}

//--------------------------------------------------------------  DrawLava
void DrawLava(void);
#if 0
void DrawLava(void)
{
	short where = RandomInt(640);
	short velocity = -(RandomInt(15) + 15);

	StartPixelShatter(	where, 460, 0, velocity, kShatterLavaBubble);
}
#endif

void DrawScoreFloaters(void);
void DrawScoreFloaters(void)
{
#if 0
	int i,j,count;
	Rect r;
	CGrafPtr theGrafPtr = workSrcMap;
	PixMap *theMap = *(theGrafPtr->portPixMap);
	short rowBytes = theMap->rowBytes & 0x3fff;
	char *addr = theMap->baseAddr;
	
	for(count = 0; count < numScoreFloater; count++)
	{
		r.top = theScoreFloater[count].location.v;
		r.left = theScoreFloater[count].location.h;
		r.bottom = theScoreFloater[count].location.v + 20;
		r.right = theScoreFloater[count].location.h + 60;
	
		addr = theMap->baseAddr + (r.top * rowBytes);
		
		for(j = r.top; j < r.bottom; j++)
		{
			for(i = r.left; i < r.right; i++)
			{
				addr[i] = 0;
			}
			
			addr += rowBytes;
		}
	}
#else
	int count;
	Rect baseRect = {0,0,8,11};
		
	for(count = 0; count < numScoreFloater; count++)
	{
		short		digit;
		short		workNumber;
		Rect r = baseRect;
				
		r.top += theScoreFloater[count].location.v;
		r.bottom += theScoreFloater[count].location.v;
		r.left += theScoreFloater[count].location.h;
		r.right += theScoreFloater[count].location.h;
		workNumber = theScoreFloater[count].score;
			
		digit = (workNumber + 1) / 1000;		// Do 1000's digit.
		digit = digit % 10L;
		if (digit != 0)
		{
			CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&numbersSrc[digit], &r, srcCopy, 0L);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &r );
			#endif
		}
		
		r.left += 11;
		r.right += 11;

		digit = (workNumber + 1) / 100;		// Do 100's digit.
		digit = digit % 10L;
		if ((digit != 0) || (workNumber >= 1000))
		{
			CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&numbersSrc[digit], &r, srcCopy, 0L);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &r );
			#endif
		}

		r.left += 11;
		r.right += 11;
				
		digit = (workNumber + 1) / 10;			// Do 10's digit.
		digit = digit % 10L;
		if ((digit != 0) || (workNumber >= 100))
		{
			CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
					&((GrafPtr)workSrcMap)->portBits, 
					&numbersSrc[digit], &r, srcCopy, 0L);
			#ifdef USE_INVALID_RECTS
			DSpContext_InvalBackBufferRect( gTheContext, &r );
			#endif
		}
		
		r.left += 11;
		r.right += 11;

		digit = (workNumber) % 10;			// Do 1's digit.
		CopyBits(&((GrafPtr)numberSrcMap)->portBits, 
				&((GrafPtr)workSrcMap)->portBits, 
				&numbersSrc[digit], &r, srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &r );
		#endif
	}
#endif
}

//--------------------------------------------------------------  DrawFrame

// This function is the "master" drawing function that calls all the above…
// routines.  It is called once per frame.

void DrawFrame (void)
{
	Boolean bannerInFront = false;
	
	{
		KeyMap theKeys;
		GetKeys(theKeys);
	
		if ((theKeys[1] & 0x004))
		{
			bannerInFront = true;
		}
	}

	if (!bannerInFront)
	{
		DrawBanner();
	}

	DrawTorches();				// Gee, draws the torches?
	
	DrawScoreFloaters();
	
	DrawHand();					// Draws the hand?
	DrawEye();					// A clue to easing your documentation demands…
	DrawPlayer();				// is to use "smart" names for your functions.
	CheckPlayerWrapAround();	// Check for player wrap-around.
	DrawEnemies();				// Handle all sphinx-type enemy drawing.

	DrawObelisks();			// draw the obelisks
	DrawPixelShatter();			// draw exploding pixels
	
	DrawLava();					// add sparkling lava

	if (bannerInFront)
	{
		DrawBanner();
	}	

}

//--------------------------------------------------------------  GameQuitGraphics

void GameQuitGraphics(void)
{
	short h,v;
	
	for(v = 0; v < 480; v += 24)
	{
		for(h = 0; h < 640; h += 32)
		{
				StartPixelShatter(h, v, 0, 0, kShatterLightningDust);
		}
	}

	CopyBits(	&((GrafPtr)origBackSrcMap)->portBits, 
				&((GrafPtr)backSrcMap)->portBits, 
				&backSrcRect, &backSrcRect, srcCopy, 0L);
	#ifdef USE_INVALID_RECTS
	DSpContext_InvalBackBufferRect( gTheContext, &backSrcRect );
	#endif
}

//--------------------------------------------------------------  GameIdleAnimation

void GameIdleAnimation(void)
{
	// do our idle animation
	DSpContext_GetBackBuffer( gTheContext, kDSpBufferKind_Normal, &workSrcMap );
	DumpBackToWorkMap();					// clear the screen
	DrawBanner();
	DrawTorches();
	HandlePixelShatter();
	HandleLava();
	DrawLava();
	
	if (helpOpen)
	{
		CopyBits(&((GrafPtr)helpSrcMap)->portBits, 
			&((GrafPtr)workSrcMap)->portBits, 
			&helpSrc, &helpDest, srcCopy, 0L);
		#ifdef USE_INVALID_RECTS
		DSpContext_InvalBackBufferRect( gTheContext, &helpDest );
		#endif
	}
	
	DrawPixelShatter();
	HandleLightning();

	DSpContext_SwapBuffers( gTheContext, NULL, NULL );	
}

