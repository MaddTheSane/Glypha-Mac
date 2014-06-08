//
//  enums.h
//  Glypha
//
//  Created by C.W. Betts on 2/7/14.
//
//

#ifndef Glypha_enums_h
#define Glypha_enums_h

enum KeyMaps{
	kHelpKeyASCII = 0x05,
	kPageUpKeyASCII = 0x0B,
	kPageDownKeyASCII = 0x0C,
	kUpArrowKeyASCII = 0x1E,
	kDownArrowKeyASCII = 0x1F,
	
	kDownArrowKeyMap = 122,		// key map offset for down arrow
	kRightArrowKeyMap = 123,	// key map offset for right arrow
	kLeftArrowKeyMap = 124,		// key map offset for left arrow
	
	kAKeyMap = 7,
	kEKeyMap = 9,
	kPKeyMap = 36,
	kQKeyMap = 11,
	kSKeyMap = 6,
	kColonMap = 0x2E,
	kQuoteMap = 0x20,
	kCommandKeyMap = 48,
	kEscKeyMap = 50,
	kSpaceBarMap = 54
};

enum SoundIndexes {
	kAhnkSound = 0,
	kBirdSound,
	kBonusSound,
	kBoom1Sound,
	kBoom2Sound,
	kSplashSound,
	kFlapSound,
	kFlipSound,
	kGrateSound,
	kLightningSound,
	kMusicSound,
	kRezSound,
	kScreechSound,
	kSpawnSound,
	kWalkSound,
	kFlap2Sound,
	kScrape2Sound
};

enum SoundEnums {
	kBaseBufferSoundID	= 1000,
	kSoundDone			= 913,
	kSoundDone2			= 749,
	kMaxSounds			= 17,
	kBirdPriority		= 80,
	kBonusPriority		= 85,
	kBoom1Priority		= 115,
	kBoom2Priority		= 110,
	kSplashPriority		= 75,
	kFlapPriority		= 70,
	kGratePriority		= 40,
	kLightningPriority	= 100,
	kMusicPriority		= 120,
	kScreechPriority	= 50,
	kSpawnPriority		= 90,
	kWalkPriority		= 30,
	kFlap2Priority		= 20,
	kScrape2Priority	= 10
};

#define kLavaHeight	456
#define kRoofHeight	2
#define kGravity	4

enum objectModes {
	kIdle = -1,		// enemy & player mode
	kFlying,		// enemy & player mode
	kWalking,		// enemy & player mode
	kSinking,		// player mode
	kSpawning,		// enemy mode
	kFalling,		// enemy mode & player mode
	kEggTimer,		// enemy mode
	kDeadAndGone,	// enemy mode
	kBones,			// player mode
	kLurking = 10,	// hand mode
	kOutGrabeth,	// hand mode
	kClutching,		// hand mode
	kWaiting = 15,	// eye mode
	kStalking		// eye mode
};

#define kInitNumLives	5
#define kMaxEnemies		8
#define kDontFlapVel	8

enum EnemyTypes {
	kOwl = 0,
	kWolf,
	kJackal
};

enum GameMenuVal {
	iNewGame = 1,
	iPauseGame,
	iEndGame,
	iQuit = 5
};

enum OptionsMenuVal {
	iHelp = 1,
	iHighScores = 3,
	iScoreReset,
	iAboutSource = 6,
	iSoundItem
};

enum GlyphaMenuIDs {
	kAppleMenuID = 128,
	kGameMenuID,
	kOptionsMenuID
};

enum GlyphaPictNums {
	kHandPictID = 128,
	kHandMaskID,
	kBackgroundPictID,
	kHelpPictID,
	kAboutPictID,
	kObeliskPictID,
	kPlayerPictID,
	kPlayerMaskID,
	kNumberPictID,
	kIdlePictID,
	kEnemyWalkPictID,
	kEnemyFlyPictID,
	kEnemyWalkMaskID,
	kEnemyFlyMaskID,
	kFlamePictID,
	kEggPictID,
	kEggMaskID,
	kPlatformPictID,
	kEyePictID,
	kEyeMaskID
};

enum {
	kRedAlertID = 128,
	kHighNameDialogID,
	kScoreResetDialog,
	kAboutSourceDialog
};

#endif
