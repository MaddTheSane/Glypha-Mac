
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
#define kPrefsStringsID		160
#define	kPrefsFNameIndex	1

Boolean GetPrefsFPath (long *, short *);
Boolean CreatePrefsFolder (short *);
Boolean WritePrefs (long *, short *, prefsInfo *);
OSErr ReadPrefs (long *, short *, prefsInfo *);
Boolean DeletePrefs (long *, short *);


//==============================================================  Functions
//--------------------------------------------------------------  GetPrefsFPath

Boolean GetPrefsFPath (long *prefDirID, short *systemVolRef)
{
	OSErr		theErr;
	
	theErr = FindFolder(kOnSystemDisk, kPreferencesFolderType, kCreateFolder, 
		systemVolRef, prefDirID);
	if (theErr != noErr)
		return(FALSE);
	
	return(TRUE);
}

//--------------------------------------------------------------  CreatePrefsFolder

Boolean CreatePrefsFolder (short *systemVolRef)
{
	HFileParam	fileParamBlock;
	Str255		folderName;
	OSErr		theErr;
	
	GetIndString(folderName, kPrefsStringsID, kPrefsFNameIndex);
	
	fileParamBlock.ioVRefNum = *systemVolRef;
	fileParamBlock.ioDirID = 0;
	fileParamBlock.ioNamePtr = folderName;
	fileParamBlock.ioCompletion = 0L;
	
	theErr = PBDirCreateSync((HParmBlkPtr)&fileParamBlock);
	if (theErr != noErr)
	{
		RedAlert("\pPrefs Creation Error");
		return(FALSE);
	}
	return(TRUE);
}

//--------------------------------------------------------------  WritePrefs

Boolean WritePrefs (long *prefDirID, short *systemVolRef, prefsInfo *thePrefs)
{
	OSErr		theErr;
	FSIORefNum	fileRefNum;
	long		byteCount;
	FSSpec		theSpecs;
	Str255		fileName = kPrefFileName;
	
	theErr = FSMakeFSSpec(*systemVolRef, *prefDirID, fileName, &theSpecs);
	if (theErr != noErr)
	{
		if (theErr != fnfErr)
		{
			RedAlert("\pPrefs FSMakeFSSpec() Error");
		}
		theErr = FSpCreate(&theSpecs, kPrefCreatorType, kPrefFileType, smSystemScript);
		if (theErr != noErr)
		{
			RedAlert("\pPrefs FSpCreate() Error");
		}
	}
	theErr = FSpOpenDF(&theSpecs, fsRdWrPerm, &fileRefNum);
	if (theErr != noErr)
	{
		RedAlert("\pPrefs FSpOpenDF() Error");
	}
	
	byteCount = sizeof(*thePrefs);
	
	theErr = FSWrite(fileRefNum, &byteCount, thePrefs);
	if (theErr != noErr)
	{
		RedAlert("\pPrefs FSWrite() Error");
	}
	
	theErr = FSCloseFork(fileRefNum);
	if (theErr != noErr)
	{
		RedAlert("\pPrefs FSCloseFork() Error");
	}
	
	return(TRUE);
}

//--------------------------------------------------------------  SavePrefs

Boolean SavePrefs (prefsInfo *thePrefs, short versionNow)
{
	long		prefDirID;
	short		systemVolRef;
	
	thePrefs->prefVersion = versionNow;
	
	if (!GetPrefsFPath(&prefDirID, &systemVolRef))
		return(FALSE);
	if (!WritePrefs(&prefDirID, &systemVolRef, thePrefs))
		return(FALSE);

	return(TRUE);
}

//--------------------------------------------------------------  ReadPrefs

OSErr ReadPrefs (long *prefDirID, short *systemVolRef, prefsInfo *thePrefs)
{
	OSErr		theErr;
	short		fileRefNum;
	long		byteCount;
	FSSpec		theSpecs;
	Str255		fileName = kPrefFileName;
	
	theErr = FSMakeFSSpec(*systemVolRef, *prefDirID, fileName, &theSpecs);
	if (theErr != noErr)
	{
		if (theErr == fnfErr)
			return(theErr);
		else
		{
			RedAlert("\pPrefs FSMakeFSSpec() Error");
		}
	}
	
	theErr = FSpOpenDF(&theSpecs, fsRdWrPerm, &fileRefNum);
	if (theErr != noErr)
	{
		RedAlert("\pPrefs FSpOpenDF() Error");
	}
	
	byteCount = sizeof(*thePrefs);
	
	theErr = FSRead(fileRefNum, &byteCount, thePrefs);
	if (theErr != noErr)
	{
		if (theErr == eofErr)
			theErr = FSCloseFork(fileRefNum);
		else
		{
			RedAlert("\pPrefs FSRead() Error");
		}
		return(theErr);
	}
	
	theErr = FSCloseFork(fileRefNum);
	if (theErr != noErr)
	{
		RedAlert("\pPrefs FSCloseFork() Error");
	}
	
	return(theErr);
}

//--------------------------------------------------------------  DeletePrefs

Boolean DeletePrefs (long *dirID, short *volRef)
{
	FSSpec		theSpecs;
	Str255		fileName = kPrefFileName;
	OSErr		theErr;
	
	theErr = FSMakeFSSpec(*volRef, *dirID, fileName, &theSpecs);
	if (theErr != noErr)
		return(FALSE);
	else
		theErr = FSpDelete(&theSpecs);
	
	if (theErr != noErr)
		return(FALSE);
	
	return(TRUE);
}

//--------------------------------------------------------------  LoadPrefs

Boolean LoadPrefs (prefsInfo *thePrefs, short versionNeed)
{
	long		prefDirID;
	OSErr		theErr;
	short		systemVolRef;
	Boolean		noProblems;
	
	noProblems = GetPrefsFPath(&prefDirID, &systemVolRef);
	if (!noProblems)
		return(FALSE);
	theErr = ReadPrefs(&prefDirID, &systemVolRef, thePrefs);
	if (theErr == eofErr)
	{
		noProblems = DeletePrefs(&prefDirID, &systemVolRef);
		return(FALSE);
	}
	else if (theErr != noErr)
		return(FALSE);
	
	if (thePrefs->prefVersion != versionNeed)
	{
		noProblems = DeletePrefs(&prefDirID, &systemVolRef);
		return(FALSE);
	}
	
	return(TRUE);
}

