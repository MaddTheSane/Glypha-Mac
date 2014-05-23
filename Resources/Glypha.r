#include <Carbon/Carbon.r>
#include "enums.h"

resource 'DLOG' (kScoreResetDialog, "Configure Game") {
	{120, 144, 216, 428},
	dBoxProc,
	visible,
	noGoAway,
	0x3,
	133,
	"Reset Scores...",
	alertPositionMainScreen
};

resource 'DLOG' (kHighNameDialogID, "High Score Name") {
	{120, 190, 248, 470},
	dBoxProc,
	visible,
	noGoAway,
	0x3E7,
	130,
	"HiScoreName",
	centerMainScreen
};

resource 'DLOG' (kAboutSourceDialog, "About The Source Code") {
	{40, 40, 184, 360},
	dBoxProc,
	visible,
	goAway,
	0x0,
	kAboutSourceDialog,
	"",
	centerMainScreen
};

resource 'ICON' (997) {
	$"000F F000 0070 0E00 0183 C180 030F F0C0"
	$"040E 7020 081C 3810 1018 1808 3018 180C"
	$"2018 1804 401C 3802 400C 3002 400E 7002"
	$"8007 E001 8003 C001 8001 8001 80FF FF01"
	$"8001 8001 8001 8001 8001 8001 8001 8001"
	$"4001 8002 4001 8002 4001 8002 2001 8004"
	$"3001 800C 1001 8008 0801 8010 0401 8020"
	$"0301 80C0 0181 8180 0070 0E00 000F F0"
};

resource 'ICON' (137) {
	$"0001 8000 0003 C000 0003 C000 0006 6000"
	$"0006 6000 000C 3000 000C 3000 0018 1800"
	$"0019 9800 0033 CC00 0033 CC00 0063 C600"
	$"0063 C600 00C3 C300 00C3 C300 0183 C180"
	$"0183 C180 0303 C0C0 0303 C0C0 0603 C060"
	$"0601 8060 0C01 8030 0C00 0030 1800 0018"
	$"1801 8018 3003 C00C 3003 C00C 6001 8006"
	$"6000 0006 C000 0003 FFFF FFFF 7FFF FFFE"
};

resource 'DITL' (kScoreResetDialog, "Configure Game") {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{60, 44, 80, 120},
		Button {
			enabled,
			"Yes"
		},
		/* [2] */
		{60, 164, 80, 240},
		Button {
			enabled,
			"No"
		},
		/* [3] */
		{12, 64, 44, 264},
		StaticText {
			enabled,
			"Are you sure you want to reset Glypha's "
			"scores?"
		},
		/* [4] */
		{12, 20, 44, 52},
		Icon {
			disabled,
			2
		}
	}
};

resource 'DITL' (kHighNameDialogID, "High Score Name") {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{88, 102, 108, 178},
		Button {
			enabled,
			"Okay"
		},
		/* [2] */
		{58, 72, 74, 210},
		EditText {
			enabled,
			"Cyclops                                 "
			"                                        "
			"                                        "
			"                                        "
			"                                        "
			"                                        "
		},
		/* [3] */
		{16, 64, 48, 280},
		StaticText {
			enabled,
			"Your score #^0 of the ten best!  Enter y"
			"our name (15 chars.)."
		},
		/* [4] */
		{16, 20, 48, 52},
		Icon {
			disabled,
			1
		}
	}
};

resource 'DITL' (kRedAlertID, "Red Alert") {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{80, 223, 100, 281},
		Button {
			enabled,
			"Okay"
		},
		/* [2] */
		{8, 8, 72, 241},
		StaticText {
			disabled,
			"^0"
		},
		/* [3] */
		{76, 8, 92, 211},
		StaticText {
			disabled,
			"^1"
		},
		/* [4] */
		{8, 249, 40, 281},
		Icon {
			disabled,
			998
		}
	}
};

resource 'DITL' (kAboutSourceDialog) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{104, 122, 124, 198},
		Button {
			enabled,
			"Thanks"
		},
		/* [2] */
		{40, 25, 88, 295},
		StaticText {
			disabled,
			"The source code for this application sho"
			"uld be distributed as part of the direct"
			"ory in which you originally found Glypha"
			" III."
		},
		/* [3] */
		{16, 102, 32, 218},
		StaticText {
			disabled,
			"Source Code"
		}
	}
};

resource 'WIND' (128, "mainWindow") {
	{90, 30, 550, 670},
	noGrowDocProc,
	visible,
	noGoAway,
	0x1,
	"Glypha III",
	centerMainScreen
};

resource 'WIND' (129, "aboutWindow") {
	{0, 0, 318, 325},
	plainDBox,
	visible,
	noGoAway,
	0x0,
	"New Window",
	centerMainScreen
};

resource 'MENU' (kAppleMenuID) {
	kAppleMenuID,
	textMenuProc,
	allEnabled,
	enabled,
	apple,
	{	/* array: 1 elements */
		/* [1] */
		"About Glypha III...", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (kGameMenuID) {
	kGameMenuID,
	textMenuProc,
	0x7FFFFFF7,
	enabled,
	"Game",
	{	/* array: 5 elements */
		/* [1] */
		"New Game", noIcon, "N", noMark, plain,
		/* [2] */
		"Pause Game", noIcon, "P", noMark, plain,
		/* [3] */
		"End Game", noIcon, "E", noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain,
		/* [5] */
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (kOptionsMenuID) {
	kOptionsMenuID,
	textMenuProc,
	0x7FFFFFED,
	enabled,
	"Options",
	{	/* array: 7 elements */
		/* [1] */
		"Help", noIcon, "H", noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain,
		/* [3] */
		"High Scores", noIcon, "S", noMark, plain,
		/* [4] */
		"Reset Scores...", noIcon, noKey, noMark, plain,
		/* [5] */
		"-", noIcon, noKey, noMark, plain,
		/* [6] */
		"Source Code...", noIcon, noKey, noMark, plain,
		/* [7] */
		"No Sound", noIcon, noKey, noMark, plain
	}
};

read 'PICT' (kAboutPictID) "132.pict";
read 'PICT' (kBackgroundPictID) "130.pict";
read 'PICT' (kPlayerPictID) "135.pict";
read 'PICT' (kPlayerMaskID) "136.pict";
read 'PICT' (kNumberPictID) "137.pict";
read 'PICT' (kIdlePictID) "138.pict";
read 'PICT' (kEnemyWalkPictID) "139.pict";
read 'PICT' (kEnemyWalkMaskID) "141.pict";
read 'PICT' (kEnemyFlyPictID) "140.pict";
read 'PICT' (kEnemyFlyMaskID) "142.pict";
read 'PICT' (kFlamePictID) "143.pict";
read 'PICT' (kEggPictID) "144.pict";
read 'PICT' (kEggMaskID) "145.pict";
read 'PICT' (kPlatformPictID) "146.pict";
read 'PICT' (kHelpPictID) "131.pict";
read 'PICT' (kHandPictID) "128.pict";
read 'PICT' (kHandMaskID) "129.pict";
read 'PICT' (kEyePictID) "147.pict";
read 'PICT' (kEyeMaskID) "148.pict";
read 'PICT' (kObeliskPictID) "134.pict";

read 'snd ' (kBaseBufferSoundID + 0, "Ahnk", purgeable) "Ahnk.snd";
read 'snd ' (kBaseBufferSoundID + kBirdSound, "Bird", purgeable) "Bird.snd";
read 'snd ' (kBaseBufferSoundID + kBonusSound, "Bonus", purgeable) "Bonus.snd";
read 'snd ' (kBaseBufferSoundID + kBoom1Sound, "Boom 1", purgeable) "Boom 1.snd";
read 'snd ' (kBaseBufferSoundID + kBoom2Sound, "Boom 2", purgeable) "Boom 2.snd";
read 'snd ' (kBaseBufferSoundID + kSplashSound, "Drip", purgeable) "Drip.snd";
read 'snd ' (kBaseBufferSoundID + kFlapSound, "Flap", purgeable) "Flap.snd";
read 'snd ' (kBaseBufferSoundID + kFlap2Sound, "Flap 2", purgeable) "Flap 2.snd";
read 'snd ' (kBaseBufferSoundID + kFlipSound, "Flip", purgeable) "Flip.snd";
read 'snd ' (kBaseBufferSoundID + kGrateSound, "Grating", purgeable) "Grating.snd";
read 'snd ' (kBaseBufferSoundID + kLightningSound, "Lightning", purgeable) "Lightning.snd";
read 'snd ' (kBaseBufferSoundID + kMusicSound, "Music", purgeable) "Music.snd";
read 'snd ' (kBaseBufferSoundID + kRezSound, "Rez", purgeable) "Rez.snd";
read 'snd ' (kBaseBufferSoundID + kScreechSound, "Screech", purgeable) "Screech.snd";
read 'snd ' (kBaseBufferSoundID + kSpawnSound, "Spawn", purgeable) "Spawn.snd";
read 'snd ' (kBaseBufferSoundID + kWalkSound, "Walk", purgeable) "Walk.snd";
read 'snd ' (kBaseBufferSoundID + kScrape2Sound, "Scrape 2", purgeable) "Scrape 2.snd";

data 'ALRT' (kRedAlertID, "Red Alert") {
	$"004D 005F 00B9 0180 0080 5555"                      /* .M._.¹.€.€UU */
};

resource 'STR#' (128) {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"Sorry, Glypha II needs a color Mac.  Try"
		" Glypha 3.0 (b&w).  Exit to Finder.",
		/* [2] */
		"Sorry, Glypha II requires a monitor at l"
		"east 640 x 480.  Exit to Finder. ",
		/* [3] */
		"Glypha II requires the monitor set to 16"
		" colors (4 bit).  Relaunch Glypha when y"
		"ou have switched to 16 colors.",
		/* [4] */
		"Yow!  Error initializing (nil handle).  "
		"We may need more memory or Glypha is dam"
		"aged.  Exit to Finder."
	}
};

resource 'STR#' (129) {
	{	/* array StringArray: 2 elements */
		/* [1] */
		"You are about to completely erase all th"
		"e high scores.  Continue?",
		/* [2] */
		"You are about to quit in the middle of a"
		" game.  Quit Glypha?"
	}
};

data 'zade' (0, "Owner resource") {
	$"1153 6368 6568 6572 617A 6164 6520 3139"            /* .Scheherazade 19 */
	$"3935"                                               /* 95 */
};

resource 'wctb' (128, "mainWindow") {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 0, 0, 0,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'wctb' (129) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 0, 0, 0,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dlgx' (kScoreResetDialog) {
	versionZero {
		11
	}
};

resource 'dlgx' (kHighNameDialogID) {
	versionZero {
		11
	}
};

resource 'dlgx' (kAboutSourceDialog) {
	versionZero {
		11
	}
};

resource 'dftb' (kAboutSourceDialog) {
	versionZero {
		{	/* array FontStyle: 3 elements */
			/* [1] */
			skipItem {

			}			,
			/* [2] */
			dataItem {
				65,
				-2,
				0,
				0,
				0,
				1,
				0,
				0,
				0,
				0,
				0,
				0,
				""
			},
			/* [3] */
			dataItem {
				64,
				0,
				0,
				0,
				0,
				1,
				0,
				0,
				0,
				0,
				0,
				0,
				""
			}
		}
	}
};

