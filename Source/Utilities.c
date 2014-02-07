
//============================================================================
//----------------------------------------------------------------------------
//									Utilities.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"

extern WindowPtr mainWindow;
extern Boolean pausing;

#define kActive		0
#define kInactive	255

long tickNext;


//==============================================================  Functions
//--------------------------------------------------------------  RandomInt

short RandomInt(short range)
{
	long rawResult;
	
	rawResult = Random();
	if (rawResult < 0L)
		rawResult *= -1L;
	rawResult = (rawResult * (long)range) / 32768;
	
	return (short)rawResult;
}

//--------------------------------------------------------------  RedAlert

void RedAlert(StringPtr theStr)
{
	ParamText(theStr, "\p", "\p", "\p");
	Alert(kRedAlertID, NULL);
	ExitToShell();
}

//--------------------------------------------------------------  LoadGraphic

void LoadGraphic(short resID)
{
	Rect		bounds;
	PicHandle	thePicture;
	
	thePicture = GetPicture(resID);
	if (thePicture == NULL)
		RedAlert("\pA Graphic Couldn't Be Loaded");
	
	HLock((Handle)thePicture);
	bounds = (*thePicture)->picFrame;
	HUnlock((Handle)thePicture);
	OffsetRect(&bounds, -bounds.left, -bounds.top);
	DrawPicture(thePicture, &bounds);
	
	ReleaseResource((Handle)thePicture);
}

//--------------------------------------------------------------  ZeroRectCorner

void ZeroRectCorner(Rect *theRect)		// Offset rect to (0, 0)
{
	theRect->right -= theRect->left;
	theRect->bottom -= theRect->top;
	theRect->left = 0;
	theRect->top = 0;
}

//--------------------------------------------------------------  LogNextTick

void LogNextTick(long howMany)
{
	tickNext = TickCount() + howMany;
}

//--------------------------------------------------------------  WaitForNextTick

void WaitForNextTick(void)
{
	if (tickNext > TickCount())
		RunCurrentEventLoop((tickNext - TickCount()) / 60.0);
}
//--------------------------------------------------------------  CreateOffScreenPixMap

void CreateOffScreenPixMap(Rect *theRect, GWorldPtr *offScreen)
{
	if (NewGWorld(offScreen, 0, theRect, 0, GetGDevice(), noNewDevice)) {
		RedAlert("\pGWorld could not be successfully created.");
	}
	SetPort(*offScreen); // HERE
}

//--------------------------------------------------------------  CreateOffScreenBitMap

void CreateOffScreenBitMap(Rect *theRect, GWorldPtr *offScreen)
{
	if (NewGWorld(offScreen, 0, theRect, 0, GetGDevice(), noNewDevice)) {
		RedAlert("\pGWorld could not be successfully created.");
	}
	SetPort(*offScreen); // HERE
}

//--------------------------------------------------------------  CenterAlert

void CenterAlert(short alertID)
{
	AlertTHndl	alertHandle;
	Rect		theScreen, alertRect;
	short		horiOff, vertOff;
	Byte		wasState;
	BitMap		screenBits;
	
	GetQDGlobalsScreenBits(&screenBits);
	theScreen = screenBits.bounds;
	theScreen.top += 20;
	
	alertHandle = (AlertTHndl)GetResource('ALRT', alertID);
	if (alertHandle != NULL) {
		wasState = HGetState((Handle)alertHandle);
		HLock((Handle)alertHandle);
		
		alertRect = (**alertHandle).boundsRect;
		OffsetRect(&alertRect, -alertRect.left, -alertRect.top);
		
		horiOff = ((theScreen.right - theScreen.left) - alertRect.right) / 2;
		vertOff = ((theScreen.bottom - theScreen.top) - alertRect.bottom) / 3;
		
		OffsetRect(&alertRect, horiOff, vertOff + 20);
		
		(**alertHandle).boundsRect = alertRect;
		HSetState((Handle)alertHandle, wasState);
	}
}

//--------------------------------------------------------------  RectWide

short RectWide (Rect *theRect)
{
	return (theRect->right - theRect->left);
}

//--------------------------------------------------------------  RectTall

short RectTall(Rect *theRect)
{
	return (theRect->bottom - theRect->top);
}

//--------------------------------------------------------------  CenterRectInRect

void CenterRectInRect(Rect *rectA, Rect *rectB)
{
	short widthA, tallA;
	
	widthA = RectWide(rectA);
	tallA = RectTall(rectA);
	
	rectA->left = rectB->left + (RectWide(rectB) - widthA) / 2;
	rectA->right = rectA->left + widthA;
	
	rectA->top = rectB->top + (RectTall(rectB) - tallA) / 2;
	rectA->bottom = rectA->top + tallA;
}

//--------------------------------------------------------------  PasStringCopy

void PasStringCopy(StringPtr p1, StringPtr p2)
{
	register short stringLength;
	
	stringLength = *p2++ = *p1++;
	while (--stringLength >= 0)
		*p2++ = *p1++;
}

//--------------------------------------------------------------  CenterDialog

void CenterDialog (short dialogID)
{
	DialogTHndl	dlogHandle;
	Rect		theScreen, dlogBounds;
	short		hPos, vPos;
	Byte		wasState;
	BitMap		screenBits;
	
	GetQDGlobalsScreenBits(&screenBits);
	theScreen = screenBits.bounds;
	theScreen.top += 20;
	
	dlogHandle = (DialogTHndl)GetResource('DLOG', dialogID);
	if (dlogHandle != NULL) {
		wasState = HGetState((Handle)dlogHandle);
		HLock((Handle)dlogHandle);
		
		dlogBounds = (**dlogHandle).boundsRect;
		OffsetRect(&dlogBounds, -dlogBounds.left, -dlogBounds.top);
		
		hPos = ((theScreen.right - theScreen.left) - dlogBounds.right) / 2;
		vPos = ((theScreen.bottom - theScreen.top) - dlogBounds.bottom) / 3;
		
		OffsetRect(&dlogBounds, hPos, vPos + 20);
		
		(**dlogHandle).boundsRect = dlogBounds;
		HSetState((Handle)dlogHandle, wasState);
	}
}

//--------------------------------------------------------------  DrawDefaultButton

void DrawDefaultButton (DialogPtr theDialog)
{
	Rect	itemRect;
	Handle	itemHandle;
	short	itemType;
	
	GetDialogItem(theDialog, 1, &itemType, &itemHandle, &itemRect);
	InsetRect(&itemRect, -4, -4);
	PenSize(3, 3);
	FrameRoundRect(&itemRect, 16, 16);
	PenNormal();
}

//--------------------------------------------------------------  PasStringCopyNum

void PasStringCopyNum(StringPtr p1, StringPtr p2, short charsToCopy)
{
	short i;
	
	if (charsToCopy > *p1)		// if trying to copy more chars than there are
		charsToCopy = *p1;		// reduce the number of chars to copy to this size
	
	*p2 = charsToCopy;
	
	*p2++;
	*p1++;
	
	for (i = 0; i < charsToCopy; i++)
		*p2++ = *p1++;
}

//--------------------------------------------------------------  GetDialogString

void GetDialogString(DialogPtr theDialog, short item, StringPtr theString)
{
	Rect	itemRect;
	Handle	itemHandle;
	short	itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	GetDialogItemText(itemHandle, theString);
}

//--------------------------------------------------------------  SetDialogString

void SetDialogString(DialogPtr theDialog, short item, StringPtr theString)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	SetDialogItemText(itemHandle, theString);
}

//--------------------------------------------------------------  SetDialogNumToStr

void SetDialogNumToStr(DialogPtr theDialog, short item, long theNumber)
{
	Str255		theString;
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	NumToString(theNumber, theString);
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	SetDialogItemText(itemHandle, theString);
}

//--------------------------------------------------------------  GetDialogNumFromStr

void GetDialogNumFromStr(DialogPtr theDialog, short item, long *theNumber)
{
	Str255		theString;
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	GetDialogItemText(itemHandle, theString);
	StringToNum(theString, theNumber);
}

//--------------------------------------------------------------  DisableControl

void CustomDisableControl(DialogPtr theDialog, short whichItem)
{
	Rect		iRect;
	Handle		iHandle;
	short		iType;
	
	GetDialogItem(theDialog, whichItem, &iType, &iHandle, &iRect);
	HiliteControl((ControlHandle)iHandle, kInactive);
}

