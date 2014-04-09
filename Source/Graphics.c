
//============================================================================
//----------------------------------------------------------------------------
//									Graphics.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"

#define kUpperEyeHeight			100
#define kLowerEyeHeight			200
#define kNumLightningPts		8
#define kMaxNumUpdateRects		32

static void CheckPlayerWrapAround (void);
static void DrawHand (void);
static void DrawEye (void);
static void DrawPlayer (void);
static void CheckEnemyWrapAround (short);
static void DrawEnemies (void);

Rect		backSrcRect, workSrcRect, obSrcRect, playerSrcRect;
Rect		numberSrcRect, idleSrcRect, enemyWalkSrcRect, enemyFlySrcRect;
Rect		obeliskRects[4], playerRects[11], numbersSrc[11], numbersDest[11];
Rect		updateRects1[kMaxNumUpdateRects], updateRects2[kMaxNumUpdateRects];
Rect		flameSrcRect, flameDestRects[2], flameRects[4], eggSrcRect;
Rect		platformSrcRect, platformCopyRects[9], helpSrcRect, eyeSrcRect;
Rect		helpSrc, helpDest, handSrcRect, handRects[2], eyeRects[4];
Point		leftLightningPts[kNumLightningPts], rightLightningPts[kNumLightningPts];
GWorldPtr	backSrcMap, workSrcMap, obeliskSrcMap, playerSrcMap, eyeSrcMap;
GWorldPtr	numberSrcMap, idleSrcMap, enemyWalkSrcMap, enemyFlySrcMap;
GWorldPtr	flameSrcMap, eggSrcMap, platformSrcMap, helpSrcMap, handSrcMap;
GWorldPtr	playerMaskMap, enemyWalkMaskMap, enemyFlyMaskMap, eggMaskMap;
GWorldPtr	handMaskMap, eyeMaskMap;
RgnHandle	playRgn;
short		numUpdateRects1, numUpdateRects2;
Boolean		whichList, helpOpen, scoresOpen;

//==============================================================  Functions
//--------------------------------------------------------------  DrawPlatforms

void DrawPlatforms(short howMany)
{
	if (howMany > 3) {
		CopyBits(GetPortBitMapForCopyBits(platformSrcMap),
				 GetPortBitMapForCopyBits(backSrcMap),
				 &platformCopyRects[2], &platformCopyRects[7], srcCopy, playRgn);
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &platformCopyRects[7], &platformCopyRects[7], srcCopy, playRgn);
		AddToUpdateRects(&platformCopyRects[7]);
		
		CopyBits(GetPortBitMapForCopyBits(platformSrcMap),
				 GetPortBitMapForCopyBits(backSrcMap),
				 &platformCopyRects[4], &platformCopyRects[8], srcCopy, playRgn);
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &platformCopyRects[8], &platformCopyRects[8], srcCopy, playRgn);
		AddToUpdateRects(&platformCopyRects[8]);
	} else {
		CopyBits(GetPortBitMapForCopyBits(platformSrcMap),
				 GetPortBitMapForCopyBits(backSrcMap),
				 &platformCopyRects[3], &platformCopyRects[7], srcCopy, playRgn);
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &platformCopyRects[7], &platformCopyRects[7], srcCopy, playRgn);
		AddToUpdateRects(&platformCopyRects[7]);
		
		CopyBits(GetPortBitMapForCopyBits(platformSrcMap),
				 GetPortBitMapForCopyBits(backSrcMap),
				 &platformCopyRects[5], &platformCopyRects[8], srcCopy, playRgn);
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &platformCopyRects[8], &platformCopyRects[8], srcCopy, playRgn);
		AddToUpdateRects(&platformCopyRects[8]);
	}
	
	if (howMany > 5) {
		CopyBits(GetPortBitMapForCopyBits(platformSrcMap),
				 GetPortBitMapForCopyBits(backSrcMap),
				 &platformCopyRects[0], &platformCopyRects[6], srcCopy, playRgn);
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &platformCopyRects[6], &platformCopyRects[6], srcCopy, playRgn);
		AddToUpdateRects(&platformCopyRects[6]);
	} else {
		CopyBits(GetPortBitMapForCopyBits(platformSrcMap),
				 GetPortBitMapForCopyBits(backSrcMap),
				 &platformCopyRects[1], &platformCopyRects[6], srcCopy, playRgn);
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &platformCopyRects[6], &platformCopyRects[6], srcCopy, playRgn);
		AddToUpdateRects(&platformCopyRects[6]);
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  ScrollHelp

void ScrollHelp(short scrollDown)
{
	OffsetRect(&helpSrc, 0, scrollDown);
	
	if (helpSrc.bottom > 398) {
		helpSrc.bottom = 398;
		helpSrc.top = helpSrc.bottom - 199;
	} else if (helpSrc.top < 0) {
		helpSrc.top = 0;
		helpSrc.bottom = helpSrc.top + 199;
	}
	
	CopyBits(GetPortBitMapForCopyBits(helpSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &helpSrc, &helpDest, srcCopy, NULL);
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  OpenHelp

void OpenHelp(void)
{
	Rect	wallSrc, wallDest;
	short	i;
	
	SetRect(&helpSrc, 0, 0, 231, 0);
	helpDest = helpSrc;
	OffsetRect(&helpDest, 204, 171);
	
	SetRect(&wallSrc, 0, 0, 231, 199);
	OffsetRect(&wallSrc, 204, 171);
	wallDest = wallSrc;
	
	for (i = 0; i < 199; i ++) {
		LogNextTick(1L);
		helpSrc.bottom++;
		helpDest.bottom++;
		wallSrc.bottom--;
		wallDest.top++;
		
		CopyBits(GetPortBitMapForCopyBits(helpSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &helpSrc, &helpDest, srcCopy, NULL);
		
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &wallSrc, &wallDest, srcCopy, NULL);
		
		WaitForNextTick();
	}
	helpOpen = TRUE;
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}
 
//--------------------------------------------------------------  CloseWall

void CloseWall(void)
{
	Rect	wallSrc, wallDest;
	short	i;
	
	SetRect(&wallSrc, 0, 0, 231, 0);
	wallDest = wallSrc;
	OffsetRect(&wallDest, 204, 370);
	OffsetRect(&wallSrc, 204, 171);
	
	for (i = 0; i < 199; i ++) {
		wallSrc.bottom++;
		wallDest.top--;
		
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &wallSrc, &wallDest, srcCopy, NULL);
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  OpenHighScores

void OpenHighScores (void)
{
	RGBColor	theRGBColor, wasColor;
	Rect		wallSrc, wallDest;
	Rect		scoreSrc, scoreDest;
	Str255		scoreStr;
	short		i, scoreWide;
	
	SetRect(&scoreSrc, 0, 0, 231, 0);
	OffsetRect(&scoreSrc, 204, 171);
	scoreDest = scoreSrc;
	
	SetRect(&wallSrc, 0, 0, 231, 199);
	OffsetRect(&wallSrc, 204, 171);
	wallDest = wallSrc;
	
	SetPort(workSrcMap); // HERE
	PaintRect(&wallSrc);
	
	GetForeColor(&wasColor);
	
	TextFont(applFont);
	TextSize(12);
	TextFace(bold);
	
	Index2Color(132, &theRGBColor);
	RGBForeColor(&theRGBColor);
	MoveTo(scoreSrc.left + 36, scoreSrc.top + 20);
	DrawString("\pGlypha III High Scores");
	
	TextFont(applFont);
	TextSize(9);
	TextFace(bold);
	
	for (i = 0; i < 10; i++) {
		Index2Color(133, &theRGBColor);
		RGBForeColor(&theRGBColor);
		NumToString(i + 1, scoreStr);
		MoveTo(scoreSrc.left + 8, scoreSrc.top + 40 + (i * 16));
		DrawString(scoreStr);
		
		Index2Color(128, &theRGBColor);
		RGBForeColor(&theRGBColor);
		MoveTo(scoreSrc.left + 32, scoreSrc.top + 40 + (i * 16));
		DrawString(thePrefs.highNames[i]);
		
		Index2Color(164, &theRGBColor);
		RGBForeColor(&theRGBColor);
		NumToString(thePrefs.highScores[i], scoreStr);
		scoreWide = StringWidth(scoreStr);
		MoveTo(scoreSrc.left + 191 - scoreWide, scoreSrc.top + 40 + (i * 16));
		DrawString(scoreStr);
		
		Index2Color(134, &theRGBColor);
		RGBForeColor(&theRGBColor);
		NumToString(thePrefs.highLevel[i], scoreStr);
		scoreWide = StringWidth(scoreStr);
		MoveTo(scoreSrc.left + 223 - scoreWide, scoreSrc.top + 40 + (i * 16));
		DrawString(scoreStr);
	}
	
	RGBForeColor(&wasColor);
	
	SetPortWindowPort(mainWindow);
	
	for (i = 0; i < 199; i ++) {
		LogNextTick(1);
		scoreSrc.bottom++;
		scoreDest.bottom++;
		wallSrc.bottom--;
		wallDest.top++;
		
		CopyBits(GetPortBitMapForCopyBits(workSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &scoreSrc, &scoreDest, srcCopy, NULL);
		
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &wallSrc, &wallDest, srcCopy, NULL);
		
		WaitForNextTick();
	}
	
	scoresOpen = TRUE;
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  UpdateLivesNumbers

void UpdateLivesNumbers (void)
{
	short digit = (livesLeft - 1) / 10;
	digit = digit % 10L;
	if ((digit == 0) && ((livesLeft - 1) < 100))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[0], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[0], &numbersDest[0], srcCopy, NULL);
	
	digit = (livesLeft - 1) % 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[1], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[1], &numbersDest[1], srcCopy, NULL);
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  UpdateScoreNumbers

void UpdateScoreNumbers (void)
{
	long digit = theScore / 100000L;
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 1000000L))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[2], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[2], &numbersDest[2], srcCopy, NULL);
	
	digit = theScore / 10000L;
	if (digit > wasTensOfThousands) {
		livesLeft++;
		UpdateLivesNumbers();
		wasTensOfThousands = digit;
	}
	
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 100000L))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[3], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[3], &numbersDest[3], srcCopy, NULL);
	
	digit = theScore / 1000L;
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 10000L))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[4], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[4], &numbersDest[4], srcCopy, NULL);
	
	digit = theScore / 100L;
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 1000L))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[5], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[5], &numbersDest[5], srcCopy, NULL);
	
	digit = theScore / 10L;
	digit = digit % 10L;
	if ((digit == 0) && (theScore < 100L))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[6], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[6], &numbersDest[6], srcCopy, NULL);
	
	digit = theScore % 10L;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[7], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[7], &numbersDest[7], srcCopy, NULL);
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  UpdateLevelNumbers

void UpdateLevelNumbers(void)
{
	short digit = (levelOn + 1) / 100;
	
	digit = digit % 10L;
	if ((digit == 0) && ((levelOn + 1) < 1000))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[8], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[8], &numbersDest[8], srcCopy, NULL);
	
	digit = (levelOn + 1) / 10;
	digit = digit % 10L;
	if ((digit == 0) && ((levelOn + 1) < 100))
		digit = 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[9], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[9], &numbersDest[9], srcCopy, NULL);
	
	digit = (levelOn + 1) % 10;
	CopyBits(GetPortBitMapForCopyBits(numberSrcMap),
			 GetPortBitMapForCopyBits(backSrcMap),
			 &numbersSrc[digit], &numbersDest[10], srcCopy, NULL);
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 &numbersDest[10], &numbersDest[10], srcCopy, NULL);
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  GenerateLightning

void GenerateLightning (short h, short v)
{
#define kLeftObeliskH		172
#define kLeftObeliskV		250
#define kRightObeliskH		468
#define kRightObeliskV		250
#define kWander				16
	
	int i;
	short leftDeltaH = h - kLeftObeliskH;
	short rightDeltaH = h - kRightObeliskH;
	short leftDeltaV = v - kLeftObeliskV;
	short rightDeltaV = v - kRightObeliskV;
	short range;
	
	//leftDeltaH = h - kLeftObeliskH;				// determine the h and v distances between…
	//rightDeltaH = h - kRightObeliskH;			// obelisks and the target point
	//leftDeltaV = v - kLeftObeliskV;
	//rightDeltaV = v - kRightObeliskV;
	
	for (i = 0; i < kNumLightningPts; i++)		// calculate an even spread of points between…
	{											// obelisk tips and the target point
		leftLightningPts[i].h = (leftDeltaH * i) / (kNumLightningPts - 1) + kLeftObeliskH;
		leftLightningPts[i].v = (leftDeltaV * i) / (kNumLightningPts - 1) + kLeftObeliskV;
		rightLightningPts[i].h = (rightDeltaH * i) / (kNumLightningPts - 1) + kRightObeliskH;
		rightLightningPts[i].v = (rightDeltaV * i) / (kNumLightningPts - 1) + kRightObeliskV;
	}
	
	range = kWander * 2 + 1;					// randomly scatter the points vertically…
	for (i = 1; i < kNumLightningPts - 1; i++)	// but NOT the 1st or last points
	{
		leftLightningPts[i].v += RandomInt(range) - kWander;
		rightLightningPts[i].v += RandomInt(range) - kWander;
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  FlashObelisks

void FlashObelisks(Boolean flashThem)
{
	if (flashThem) {
		CopyBits(GetPortBitMapForCopyBits(obeliskSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &obeliskRects[0], &obeliskRects[2],
				 srcCopy, NULL);
		CopyBits(GetPortBitMapForCopyBits(obeliskSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &obeliskRects[1], &obeliskRects[3],
				 srcCopy, NULL);
	} else {
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &obeliskRects[2], &obeliskRects[2],
				 srcCopy, NULL);
		CopyBits(GetPortBitMapForCopyBits(backSrcMap),
				 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
				 &obeliskRects[3], &obeliskRects[3],
				 srcCopy, NULL);
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  StrikeLightning

void StrikeLightning(void)
{
	short		i;
	
	SetPortWindowPort(mainWindow);
	PenSize(1, 2);							// use a tall pen
	PenMode(patXor);						// use XOR mode
											// draw lightning bolts with inverted pen
	MoveTo(leftLightningPts[0].h, leftLightningPts[0].v);
	for (i = 0; i < kNumLightningPts - 1; i++) {
		MoveTo(leftLightningPts[i].h, leftLightningPts[i].v);
		LineTo(leftLightningPts[i + 1].h - 1, leftLightningPts[i + 1].v);
	}
	
	MoveTo(rightLightningPts[0].h, rightLightningPts[0].v);
	for (i = 0; i < kNumLightningPts - 1; i++) {
		MoveTo(rightLightningPts[i].h, rightLightningPts[i].v);
		LineTo(rightLightningPts[i + 1].h - 1, rightLightningPts[i + 1].v);
	}
	
	PenNormal();
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DumpBackToWorkMap

void DumpBackToWorkMap()
{
	CopyBits(GetPortBitMapForCopyBits(backSrcMap),
			 GetPortBitMapForCopyBits(workSrcMap),
			 &backSrcRect, &backSrcRect, srcCopy, NULL);
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DumpMainToWorkMap

void DumpMainToWorkMap()
{
	CopyBits(GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			 GetPortBitMapForCopyBits(workSrcMap),
			 &backSrcRect, &backSrcRect, srcCopy, NULL);
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  AddToUpdateRects

void AddToUpdateRects(Rect *theRect)
{
	if (whichList)
	{
		if (numUpdateRects1 < (kMaxNumUpdateRects - 1))
		{
			updateRects1[numUpdateRects1] = *theRect;
			numUpdateRects1++;
			if (updateRects1[numUpdateRects1].left < 0)
				updateRects1[numUpdateRects1].left = 0;
			else if (updateRects1[numUpdateRects1].right > 640)
				updateRects1[numUpdateRects1].right = 640;
			if (updateRects1[numUpdateRects1].top < 0)
				updateRects1[numUpdateRects1].top = 0;
			else if (updateRects1[numUpdateRects1].bottom > 480)
				updateRects1[numUpdateRects1].bottom = 480;
		}
	}
	else
	{
		if (numUpdateRects2 < (kMaxNumUpdateRects - 1))
		{
			updateRects2[numUpdateRects2] = *theRect;
			numUpdateRects2++;
			if (updateRects2[numUpdateRects2].left < 0)
				updateRects2[numUpdateRects2].left = 0;
			else if (updateRects2[numUpdateRects2].right > 640)
				updateRects2[numUpdateRects2].right = 640;
			if (updateRects2[numUpdateRects2].top < 0)
				updateRects2[numUpdateRects2].top = 0;
			else if (updateRects2[numUpdateRects2].bottom > 480)
				updateRects2[numUpdateRects2].bottom = 480;
		}
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  CheckPlayerWrapAround

void CheckPlayerWrapAround()
{
	Rect wrapRect, wasWrapRect, src;
	
	if (thePlayer.dest.right > 640) {
		thePlayer.wrapping = TRUE;
		wrapRect = thePlayer.dest;
		wrapRect.left -= 640;
		wrapRect.right -= 640;
		
		wasWrapRect = thePlayer.wasDest;
		wasWrapRect.left -= 640;
		wasWrapRect.right -= 640;
		
		if (thePlayer.mode == kBones) {
			src = playerRects[thePlayer.srcNum];
			src.bottom = src.top + thePlayer.frame;
			CopyMask(GetPortBitMapForCopyBits(playerSrcMap),
					 GetPortBitMapForCopyBits(playerMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &src, &src, &wrapRect);
		} else {
			CopyMask(GetPortBitMapForCopyBits(playerSrcMap),
					 GetPortBitMapForCopyBits(playerMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &playerRects[thePlayer.srcNum],
					 &playerRects[thePlayer.srcNum],
					 &wrapRect);
		}
		thePlayer.wrap = wrapRect;
		AddToUpdateRects(&wrapRect);
	} else if (thePlayer.dest.left < 0) {
		thePlayer.wrapping = TRUE;
		wrapRect = thePlayer.dest;
		wrapRect.left += 640;
		wrapRect.right += 640;
		
		wasWrapRect = thePlayer.wasDest;
		wasWrapRect.left += 640;
		wasWrapRect.right += 640;
		
		if (thePlayer.mode == kBones)
		{
			src = playerRects[thePlayer.srcNum];
			src.bottom = src.top + thePlayer.frame;
			CopyMask(GetPortBitMapForCopyBits(playerSrcMap),
					 GetPortBitMapForCopyBits(playerMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &src, &src, &wrapRect);
		} else {
			CopyMask(GetPortBitMapForCopyBits(playerSrcMap),
					 GetPortBitMapForCopyBits(playerMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &playerRects[thePlayer.srcNum],
					 &playerRects[thePlayer.srcNum],
					 &wrapRect);
		}
		thePlayer.wrap = wrapRect;
		AddToUpdateRects(&wrapRect);
	} else
		thePlayer.wrapping = FALSE;
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DrawTorches

void DrawTorches()
{
	short who = RandomInt(4);
	
	if (evenFrame) {
		CopyBits(GetPortBitMapForCopyBits(flameSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &flameRects[who], &flameDestRects[0], srcCopy, NULL);
		AddToUpdateRects(&flameDestRects[0]);
	} else {
		CopyBits(GetPortBitMapForCopyBits(flameSrcMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &flameRects[who], &flameDestRects[1], srcCopy, NULL);
		AddToUpdateRects(&flameDestRects[1]);
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DrawHand

void DrawHand (void)
{
	if (theHand.mode == kOutGrabeth) {
		CopyMask(GetPortBitMapForCopyBits(handSrcMap),
				 GetPortBitMapForCopyBits(handMaskMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &handRects[0],
				 &handRects[0],
				 &theHand.dest);
		AddToUpdateRects(&theHand.dest);
	} else if (theHand.mode == kClutching) {
		CopyMask(GetPortBitMapForCopyBits(handSrcMap),
				 GetPortBitMapForCopyBits(handMaskMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &handRects[1],
				 &handRects[1],
				 &theHand.dest);
		AddToUpdateRects(&theHand.dest);
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DrawEye

void DrawEye (void)
{
	if (theEye.mode == kStalking) {
		CopyMask(GetPortBitMapForCopyBits(eyeSrcMap),
				 GetPortBitMapForCopyBits(eyeMaskMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &eyeRects[theEye.srcNum],
				 &eyeRects[theEye.srcNum],
				 &theEye.dest);
		AddToUpdateRects(&theEye.dest);
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  CopyAllRects

void CopyAllRects()
{
	short i;
	
	if (whichList) {
		for (i = 0; i < numUpdateRects1; i++) {
			CopyBits(GetPortBitMapForCopyBits(workSrcMap),
					 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
					 &updateRects1[i], &updateRects1[i], srcCopy, playRgn);
		}
		
		for (i = 0; i < numUpdateRects2; i++) {
			CopyBits(GetPortBitMapForCopyBits(workSrcMap),
					 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
					 &updateRects2[i], &updateRects2[i], srcCopy, playRgn);
		}
		
		for (i = 0; i < numUpdateRects1; i++) {
			CopyBits(GetPortBitMapForCopyBits(backSrcMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &updateRects1[i], &updateRects1[i], srcCopy, playRgn);
		}
		
		numUpdateRects2 = 0;
		whichList = !whichList;
	} else {
		for (i = 0; i < numUpdateRects2; i++) {
			CopyBits(GetPortBitMapForCopyBits(workSrcMap),
					 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
					 &updateRects2[i], &updateRects2[i], srcCopy, playRgn);
		}
		
		for (i = 0; i < numUpdateRects1; i++) {
			CopyBits(GetPortBitMapForCopyBits(workSrcMap),
					 GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
					 &updateRects1[i], &updateRects1[i], srcCopy, playRgn);
		}
		
		for (i = 0; i < numUpdateRects2; i++) {
			CopyBits(GetPortBitMapForCopyBits(backSrcMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &updateRects2[i], &updateRects2[i], srcCopy, playRgn);
		}
		
		numUpdateRects1 = 0;
		whichList = !whichList;
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DrawPlayer

void DrawPlayer()
{
	Rect		src;
	
	if ((evenFrame) && (thePlayer.mode == kIdle))
	{
		CopyMask(GetPortBitMapForCopyBits(idleSrcMap),
				 GetPortBitMapForCopyBits(playerMaskMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &idleSrcRect,
				 &playerRects[thePlayer.srcNum],
				 &thePlayer.dest);
	}
	else if (thePlayer.mode == kBones)
	{
		src = playerRects[thePlayer.srcNum];
		src.bottom = src.top + thePlayer.frame;
		CopyMask(GetPortBitMapForCopyBits(playerSrcMap),
				 GetPortBitMapForCopyBits(playerMaskMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &src, &src, &thePlayer.dest);
	}
	else
	{
		CopyMask(GetPortBitMapForCopyBits(playerSrcMap),
				 GetPortBitMapForCopyBits(playerMaskMap),
				 GetPortBitMapForCopyBits(workSrcMap),
				 &playerRects[thePlayer.srcNum],
				 &playerRects[thePlayer.srcNum],
				 &thePlayer.dest);
	}
	AddToUpdateRects(&thePlayer.dest);
	
	thePlayer.wasH = thePlayer.h;
	thePlayer.wasV = thePlayer.v;
	
	thePlayer.wasDest = thePlayer.dest;
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  CheckEnemyWrapAround

void CheckEnemyWrapAround(short who)
{
	Rect		wrapRect, wasWrapRect, src;
	
	if (theEnemies[who].dest.right > 640)
	{
		wrapRect = theEnemies[who].dest;
		wrapRect.left -= 640;
		wrapRect.right -= 640;
		
		wasWrapRect = theEnemies[who].wasDest;
		wasWrapRect.left -= 640;
		wasWrapRect.right -= 640;
		
		if ((theEnemies[who].mode == kFalling) || (theEnemies[who].mode == kEggTimer))
		{
			if ((theEnemies[who].mode == kEggTimer) && (theEnemies[who].frame < 24))
			{
				src = eggSrcRect;
				src.bottom = src.top + theEnemies[who].frame;
			}
			else
				src = eggSrcRect;
			CopyMask(GetPortBitMapForCopyBits(eggSrcMap),
					 GetPortBitMapForCopyBits(eggMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &src, &src, &wrapRect);
		}
		else
		{
			CopyMask(GetPortBitMapForCopyBits(enemyFlySrcMap),
					 GetPortBitMapForCopyBits(enemyFlyMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &enemyRects[theEnemies[who].srcNum],
					 &enemyRects[theEnemies[who].srcNum],
					 &wrapRect);
		}
		AddToUpdateRects(&wrapRect);
	}
	else if (theEnemies[who].dest.left < 0)
	{
		wrapRect = theEnemies[who].dest;
		wrapRect.left += 640;
		wrapRect.right += 640;
		
		wasWrapRect = theEnemies[who].wasDest;
		wasWrapRect.left += 640;
		wasWrapRect.right += 640;
		if ((theEnemies[who].mode == kFalling) || (theEnemies[who].mode == kEggTimer))
		{
			if ((theEnemies[who].mode == kEggTimer) && (theEnemies[who].frame < 24))
			{
				src = eggSrcRect;
				src.bottom = src.top + theEnemies[who].frame;
			}
			else
				src = eggSrcRect;
			CopyMask(GetPortBitMapForCopyBits(eggSrcMap),
					 GetPortBitMapForCopyBits(eggMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &src, &src, &wrapRect);
		}
		else
		{
			CopyMask(GetPortBitMapForCopyBits(enemyFlySrcMap),
					 GetPortBitMapForCopyBits(enemyFlyMaskMap),
					 GetPortBitMapForCopyBits(workSrcMap),
					 &enemyRects[theEnemies[who].srcNum],
					 &enemyRects[theEnemies[who].srcNum],
					 &wrapRect);
		}
		AddToUpdateRects(&wrapRect);
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DrawEnemies

void DrawEnemies()
{
	Rect		src;
	short		i;
	
	for (i = 0; i < numEnemies; i++)
	{
		switch (theEnemies[i].mode)
		{
			case kSpawning:
				src = enemyRects[theEnemies[i].srcNum];
				src.bottom = src.top + theEnemies[i].frame;
				CopyMask(GetPortBitMapForCopyBits(enemyWalkSrcMap),
						 GetPortBitMapForCopyBits(enemyWalkMaskMap),
						 GetPortBitMapForCopyBits(workSrcMap),
						 &src, &src, &theEnemies[i].dest);
				AddToUpdateRects(&theEnemies[i].dest);
				theEnemies[i].wasDest = theEnemies[i].dest;
				theEnemies[i].wasH = theEnemies[i].h;
				theEnemies[i].wasV = theEnemies[i].v;
				break;
				
			case kFlying:
				CopyMask(GetPortBitMapForCopyBits(enemyFlySrcMap),
						 GetPortBitMapForCopyBits(enemyFlyMaskMap),
						 GetPortBitMapForCopyBits(workSrcMap),
						 &enemyRects[theEnemies[i].srcNum], &enemyRects[theEnemies[i].srcNum],
						 &theEnemies[i].dest);
				AddToUpdateRects(&theEnemies[i].dest);
				CheckEnemyWrapAround(i);
				theEnemies[i].wasDest = theEnemies[i].dest;
				theEnemies[i].wasH = theEnemies[i].h;
				theEnemies[i].wasV = theEnemies[i].v;
				break;
				
			case kWalking:
				CopyMask(GetPortBitMapForCopyBits(enemyWalkSrcMap),
						 GetPortBitMapForCopyBits(enemyWalkMaskMap),
						 GetPortBitMapForCopyBits(workSrcMap),
						 &enemyRects[theEnemies[i].srcNum], &enemyRects[theEnemies[i].srcNum],
						 &theEnemies[i].dest);
				AddToUpdateRects(&theEnemies[i].dest);
				theEnemies[i].wasDest = theEnemies[i].dest;
				theEnemies[i].wasH = theEnemies[i].h;
				theEnemies[i].wasV = theEnemies[i].v;
				break;
				
			case kFalling:
				CopyMask(GetPortBitMapForCopyBits(eggSrcMap),
						 GetPortBitMapForCopyBits(eggMaskMap),
						 GetPortBitMapForCopyBits(workSrcMap),
						 &eggSrcRect, &eggSrcRect, &theEnemies[i].dest);
				AddToUpdateRects(&theEnemies[i].dest);
				CheckEnemyWrapAround(i);
				theEnemies[i].wasDest = theEnemies[i].dest;
				theEnemies[i].wasH = theEnemies[i].h;
				theEnemies[i].wasV = theEnemies[i].v;
				break;
				
			case kEggTimer:
				if (theEnemies[i].frame < 24)
				{
					src = eggSrcRect;
					src.bottom = src.top + theEnemies[i].frame;
				}
				else
					src = eggSrcRect;
				CopyMask(GetPortBitMapForCopyBits(eggSrcMap),
						 GetPortBitMapForCopyBits(eggMaskMap),
						 GetPortBitMapForCopyBits(workSrcMap),
						 &src, &src, &theEnemies[i].dest);
				AddToUpdateRects(&theEnemies[i].dest);
				CheckEnemyWrapAround(i);
				theEnemies[i].wasDest = theEnemies[i].dest;
				theEnemies[i].wasH = theEnemies[i].h;
				theEnemies[i].wasV = theEnemies[i].v;
				break;
		}
	}
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

//--------------------------------------------------------------  DrawFrame

void DrawFrame()
{
	DrawTorches();
	DrawHand();
	DrawEye();
	DrawPlayer();
	CheckPlayerWrapAround();
	DrawEnemies();
	CopyAllRects();
	QDFlushPortBuffer(GetWindowPort(mainWindow), NULL);
}

