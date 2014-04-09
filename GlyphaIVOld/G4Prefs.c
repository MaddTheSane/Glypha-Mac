/*
	File:		G4Prefs.c

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

		 <2>	  7/1/98	sjb		Update to CWPro 2
*/


//============================================================================
//----------------------------------------------------------------------------
//									Prefs.c
//----------------------------------------------------------------------------
//============================================================================

// This is a slick little file that I re-use and re-use.  I wrote it to…
// seemlessly handle System 6 or System 7 with but a single call.  You need…
// to define your own "prefs" struct, but these routines will read and write…
// it to the System folder.

#include "G4Externs.h"
#include <Carbon/Carbon.h>


#define	kPrefCreatorType	'zade'				// Change this to reflect your apps creator.
#define	kPrefFileType		'zadP'				// Change this to reflect your prefs type.
#define	kPrefFileName		"\pGlypha Prefs"	// Change this to reflect the name for your prefs.
#define	kDefaultPrefFName	"\pPreferences"		// Name of prefs folder (System 6 only).
#define kPrefsStringsID		160					// For easy localization.
#define	kPrefsFNameIndex	1					// This one works with the previous constant.


Boolean GetPrefsFPath (long *, short *);
Boolean CreatePrefsFolder (short *);
Boolean WritePrefs (long *, short *, prefsInfo *);
OSErr ReadPrefs (long *, short *, prefsInfo *);
Boolean DeletePrefs (long *, short *);

//==============================================================  Functions
//--------------------------------------------------------------  GetPrefsFPath

// This function gets the file path to the Preferences folder (for System 7).
// It is called only if we can use FindFolder() (see previous function).

Boolean GetPrefsFPath (long *prefDirID, short *systemVolRef)
{
	OSErr		theErr;
									// Here's the wiley FindFolder() call.
	theErr = FindFolder(kUserDomain, kPreferencesFolderType, kCreateFolder,
		systemVolRef, prefDirID);	// It returns to us the directory and volume ref.…
	if (theErr != noErr)			// Assuming it worked at all!
		return(FALSE);
	
	return(TRUE);
}

//--------------------------------------------------------------  WritePrefs

// This is the System 7 version that handles all the above functions when you…
// want to write out the preferences file.  It is called by SavePrefs() below…
// if we're running under System 7.  It creates an FSSpec record to hold…
// information about where the preferences file is located, creates Glypha's…
// preferences if they are not found, opens the prefences file, writes out…
// the preferences, and the closes the prefs.  Bam, bam, bam.

Boolean WritePrefs (long *prefDirID, short *systemVolRef, prefsInfo *thePrefs)
{
	OSErr		theErr;
	short		fileRefNum;
	long		byteCount;
	FSSpec		theSpecs;
	Str255		fileName = kPrefFileName;
	// Create FSSpec record from volume ref and dir ID.
	theErr = FSMakeFSSpec(*systemVolRef, *prefDirID, fileName, &theSpecs);
	if (theErr != noErr)			// See if it failed.
	{								// An fnfErr means file not found error (no prefs).
		if (theErr != fnfErr)		// If that weren't the problem, we're cooked.
			RedAlert("\pPrefs FSMakeFSSpec() Error");
		// If it was an fnfErr, create the prefs.
		theErr = FSpCreate(&theSpecs, kPrefCreatorType, kPrefFileType, smSystemScript);
		if (theErr != noErr)		// If we fail to create the prefs, bail.
			RedAlert("\pPrefs FSpCreate() Error");
	}								// Okay, we either found or made a pref file, open it.
	theErr = FSpOpenDF(&theSpecs, fsRdWrPerm, &fileRefNum);
	if (theErr != noErr)			// As per usual, if we fail, bail.
		RedAlert("\pPrefs FSpOpenDF() Error");
	
	byteCount = sizeof(*thePrefs);	// Get number of bytes to write (your prefs struct).
	// And, write out the preferences.
	theErr = FSWrite(fileRefNum, &byteCount, thePrefs);
	if (theErr != noErr)			// Say no more.
		RedAlert("\pPrefs FSWrite() Error");
	
	theErr = FSCloseFork(fileRefNum);	// Close the prefs file.
	if (theErr != noErr)			// Tic, tic.
		RedAlert("\pPrefs FSClose() Error");
	
	return(TRUE);
}

//--------------------------------------------------------------  WritePrefs6

// This is the System 6 equivalent of the above function.  It handles prefs…
// opening, writing and closing for System 6.

Boolean WritePrefs6 (short *systemVolRef, prefsInfo *thePrefs)
{
	OSErr		theErr;
	short		fileRefNum;
	long		byteCount;
	Str255		fileName = kPrefFileName;
	// Attempt to open prefs file.
	theErr = FSOpen(fileName, *systemVolRef, &fileRefNum);
	if (theErr != noErr)			// If it failed, maybe the prefs don't exist.
	{								// An fnfErr means file not found.
		if (theErr != fnfErr)		// See if in fact that WASN'T the reason.
			RedAlert("\pPrefs FSOpen() Error");
		// If fnfErr WAS the problem, create the prefs.
		theErr = Create(fileName, *systemVolRef, kPrefCreatorType, kPrefFileType);
		if (theErr != noErr)
			RedAlert("\pPrefs Create() Error");
		// Open the prefs file.
		theErr = FSOpen(fileName, *systemVolRef, &fileRefNum);
		if (theErr != noErr)
			RedAlert("\pPrefs FSOpen() Error");
	}
	
	byteCount = sizeof(*thePrefs);	// Get number of bytes to write out.
	// Write the prefs out.
	theErr = FSWrite(fileRefNum, &byteCount, thePrefs);
	if (theErr != noErr)
		RedAlert("\pPrefs FSWrite() Error");
	// And close the prefs file.
	theErr = FSCloseFork(fileRefNum);
	if (theErr != noErr)
		RedAlert("\pPrefs FSClose() Error");
	
	return(TRUE);
}

//--------------------------------------------------------------  SavePrefs

// This is the single function called externally to save the preferences.
// You pass it a pointer to your preferences struct and a version number.
// One of the fields in your preferences struct should be a version number…
// (short prefVersion).  This function determines if we're on System 6 or 7…
// and then calls the appropriate routines.  It returns TRUE if all went well…
// or FALSE if any step failed.

Boolean SavePrefs (prefsInfo *thePrefs, short versionNow)
{
	long		prefDirID;
	short		systemVolRef;
	
	thePrefs->prefVersion = versionNow;			// Set prefVersion to versionNow.
	
	// Get a path to Preferences folder.
	if (!GetPrefsFPath(&prefDirID, &systemVolRef))
		return(FALSE);
	
	// Write out the preferences.
	if (!WritePrefs(&prefDirID, &systemVolRef, thePrefs))
		return(FALSE);
	return(TRUE);
}

//--------------------------------------------------------------  ReadPrefs

// This is the System 7 version for reading in the preferences.  It handles…
// opening the prefs, reading in the data to your prefs struct and closing…
// the file.

OSErr ReadPrefs (long *prefDirID, short *systemVolRef, prefsInfo *thePrefs)
{
	OSErr		theErr;
	short		fileRefNum;
	long		byteCount;
	FSSpec		theSpecs;
	// Get an FSSpec record to the prefs file.
	theErr = FSMakeFSSpec(*systemVolRef, *prefDirID, kPrefFileName, &theSpecs);
	if (theErr != noErr)
	{
		if (theErr == fnfErr)		// If it doesn't exist, return - we'll use defaults.
			return(theErr);
		else						// If some other file error occured, bail.
			RedAlert("\pPrefs FSMakeFSSpec() Error");
	}
	// Open the prefs file.
	theErr = FSpOpenDF(&theSpecs, fsRdWrPerm, &fileRefNum);
	if (theErr != noErr)
		RedAlert("\pPrefs FSpOpenDF() Error");
	
	byteCount = sizeof(*thePrefs);	// Determine the number of bytes to read in.
	// Read 'em into your prefs struct.
	theErr = FSRead(fileRefNum, &byteCount, thePrefs);
	if (theErr != noErr)			// If there was an error reading the file…
	{								// close the file and we'll revert to defaults.
		if (theErr == eofErr)
			theErr = FSCloseFork(fileRefNum);
		else						// If closing failed, bail.
			RedAlert("\pPrefs FSRead() Error");
		return(theErr);
	}
	
	theErr = FSCloseFork(fileRefNum);	// Close the prefs file.
	if (theErr != noErr)
		RedAlert("\pPrefs FSClose() Error");
	
	return(theErr);
}

//--------------------------------------------------------------  DeletePrefs

// It can happen that you introduce a game with only a few preference settings…
// but then later update your game and end up having to add additional settings…
// to be stored in your games preferences.  In this case, the size of the old…
// prefs won't match the size of the new.  Or even if the size is the same, you…
// may have re-ordered the prefs and attempting to load the old prefs will result…
// in garbage.  It is for this reason that I use the "versionNeed" variable and…
// the "prefVersion" field in the prefs struct.  In such a case, the below function…
// will be called to delte the old prefs.  When the prefs are then written out, a…
// new pref file will be created.  This particular function is the System 7 version…
// for deleting the old preferences.

Boolean DeletePrefs (long *dirID, short *volRef)
{
	FSSpec		theSpecs;
	OSErr		theErr;
	// Create an FSSec record.
	theErr = FSMakeFSSpec(*volRef, *dirID, kPrefFileName, &theSpecs);
	if (theErr != noErr)					// Test to see if it worked.
		return(FALSE);
	else									// If it worked…
		theErr = FSpDelete(&theSpecs);		// delete the file.
	
	if (theErr != noErr)
		return(FALSE);
	
	return(TRUE);
}

//--------------------------------------------------------------  LoadPrefs

// Here is the single call for loading in preferences.  It handles all the…
// above function onvolved with opening and reading in preferences.  It…
// determines whether we are on System 6 or 7 (FSSpecs) and makes the right…
// calls.

Boolean LoadPrefs (prefsInfo *thePrefs, short versionNeed)
{
	long		prefDirID;
	OSErr		theErr;
	short		systemVolRef;
	Boolean		noProblems;
	
	// Get a path to the prefs file.
	noProblems = GetPrefsFPath(&prefDirID, &systemVolRef);
	if (!noProblems)
		return(FALSE);
	
	// Attempt to read prefs.
	theErr = ReadPrefs(&prefDirID, &systemVolRef, thePrefs);
	if (theErr == eofErr)			// Fail the read?  Maybe an old prefs version.
	{								// Delete it - we'll create a new one later.
		noProblems = DeletePrefs(&prefDirID, &systemVolRef);
		return(FALSE);				// Meanwhile, we'll use defaults.
	}
	else if (theErr != noErr)
		return(FALSE);
	// Okay, maybe the read worked, but we still…
	// need to check the version number to see…
	// if it's current.
	if (thePrefs->prefVersion != versionNeed)
	{									// We'll delete the file if old version.
		noProblems = DeletePrefs(&prefDirID, &systemVolRef);
		return(FALSE);
	}
	
	return(TRUE);
}

