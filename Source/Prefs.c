
//============================================================================
//----------------------------------------------------------------------------
//									Prefs.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"

#define	kPrefCreatorType	'zade'
#define	kPrefFileType		'pref'
#define	kPrefFileName		"\pGlypha Preferences"
#define	kDefaultPrefFName	"\pPreferences"

static Boolean GetPrefsFPath(SInt32 *, FSVolumeRefNum *);
static Boolean WritePrefs(SInt32 *, FSVolumeRefNum *, prefsInfo *);
static OSErr ReadPrefs(SInt32 *, FSVolumeRefNum *, prefsInfo *);
static Boolean DeletePrefs(SInt32 *, FSVolumeRefNum *);

//==============================================================  Functions
//--------------------------------------------------------------  GetPrefsFPath

Boolean GetPrefsFPath(SInt32 *prefDirID, FSVolumeRefNum *systemVolRef)
{
	OSErr theErr;
	
	theErr = FindFolder(kUserDomain, kPreferencesFolderType, kCreateFolder, systemVolRef, prefDirID);
	if (theErr != noErr)
		return FALSE;
	
	return TRUE;
}

//--------------------------------------------------------------  CreatePrefsFolder

//--------------------------------------------------------------  WritePrefs

Boolean WritePrefs(SInt32 *prefDirID, FSVolumeRefNum *systemVolRef, prefsInfo *thePrefs)
{
	OSErr		theErr;
	FSIORefNum	fileRefNum;
	ByteCount	byteCount;
	FSSpec		theSpecs;
	
	theErr = FSMakeFSSpec(*systemVolRef, *prefDirID, kPrefFileName, &theSpecs);
	if (theErr != noErr) {
		if (theErr != fnfErr) {
			RedAlert("\pPrefs FSMakeFSSpec() Error");
		}
		theErr = FSpCreate(&theSpecs, kPrefCreatorType, kPrefFileType, smSystemScript);
		if (theErr != noErr) {
			RedAlert("\pPrefs FSpCreate() Error");
		}
	}
	theErr = FSpOpenDF(&theSpecs, fsRdWrPerm, &fileRefNum);
	if (theErr != noErr) {
		RedAlert("\pPrefs FSpOpenDF() Error");
	}
	
	byteCount = sizeof(*thePrefs);
	
	theErr = FSWriteFork(fileRefNum, fsAtMark, 0, byteCount, thePrefs, &byteCount);
	if (theErr != noErr) {
		RedAlert("\pPrefs FSWrite() Error");
	}
	
	theErr = FSCloseFork(fileRefNum);
	if (theErr != noErr) {
		RedAlert("\pPrefs FSCloseFork() Error");
	}
	
	return TRUE;
}

//--------------------------------------------------------------  SavePrefs

Boolean SavePrefs(prefsInfo *thePrefs, short versionNow)
{
	long		prefDirID;
	short		systemVolRef;
	
	thePrefs->prefVersion = versionNow;
	
	if (!GetPrefsFPath(&prefDirID, &systemVolRef))
		return FALSE;
	if (!WritePrefs(&prefDirID, &systemVolRef, thePrefs))
		return FALSE;
	
	return TRUE;
}

//--------------------------------------------------------------  ReadPrefs

OSErr ReadPrefs(SInt32 *prefDirID, FSVolumeRefNum *systemVolRef, prefsInfo *thePrefs)
{
	OSErr		theErr;
	FSIORefNum	fileRefNum;
	ByteCount	byteCount;
	FSSpec		theSpecs;
	
	theErr = FSMakeFSSpec(*systemVolRef, *prefDirID, kPrefFileName, &theSpecs);
	if (theErr != noErr) {
		if (theErr == fnfErr)
			return theErr;
		else {
			RedAlert("\pPrefs FSMakeFSSpec() Error");
		}
	}
	
	theErr = FSpOpenDF(&theSpecs, fsRdWrPerm, &fileRefNum);
	if (theErr != noErr) {
		RedAlert("\pPrefs FSpOpenDF() Error");
	}
	
	byteCount = sizeof(*thePrefs);
	
	theErr = FSReadFork(fileRefNum, fsAtMark, 0, byteCount, thePrefs, &byteCount);
	if (theErr != noErr) {
		if (theErr == eofErr)
			theErr = FSCloseFork(fileRefNum);
		else {
			RedAlert("\pPrefs FSRead() Error");
		}
		return theErr;
	}
	
	theErr = FSCloseFork(fileRefNum);
	if (theErr != noErr) {
		RedAlert("\pPrefs FSCloseFork() Error");
	}
	
	return theErr;
}

//--------------------------------------------------------------  DeletePrefs

Boolean DeletePrefs(SInt32 *dirID, FSVolumeRefNum *volRef)
{
	FSSpec		theSpecs;
	OSErr		theErr;
	
	theErr = FSMakeFSSpec(*volRef, *dirID, kPrefFileName, &theSpecs);
	if (theErr != noErr)
		return FALSE;
	else
		theErr = FSpDelete(&theSpecs);
	
	if (theErr != noErr)
		return FALSE;
	
	return TRUE;
}

//--------------------------------------------------------------  LoadPrefs

Boolean LoadPrefs(prefsInfo *thePrefs, short versionNeed)
{
	long		prefDirID;
	OSErr		theErr;
	short		systemVolRef;
	Boolean		noProblems;
	
	noProblems = GetPrefsFPath(&prefDirID, &systemVolRef);
	if (!noProblems)
		return FALSE;
	theErr = ReadPrefs(&prefDirID, &systemVolRef, thePrefs);
	if (theErr == eofErr) {
		noProblems = DeletePrefs(&prefDirID, &systemVolRef);
		return FALSE;
	} else if (theErr != noErr)
		return FALSE;
	
	if (thePrefs->prefVersion != versionNeed) {
		noProblems = DeletePrefs(&prefDirID, &systemVolRef);
		return FALSE;
	}
	
	return TRUE;
}

