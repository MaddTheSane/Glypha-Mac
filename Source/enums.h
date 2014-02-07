//
//  enums.h
//  Glypha
//
//  Created by C.W. Betts on 2/7/14.
//
//

#ifndef Glypha_enums_h
#define Glypha_enums_h

#define kHelpKeyASCII				0x05
#define kPageUpKeyASCII				0x0B
#define kPageDownKeyASCII			0x0C
#define	kUpArrowKeyASCII			0x1E
#define kDownArrowKeyASCII			0x1F

#define kDownArrowKeyMap			122		// key map offset for down arrow
#define kRightArrowKeyMap			123		// key map offset for right arrow
#define kLeftArrowKeyMap			124		// key map offset for left arrow

#define kAKeyMap					7
#define	kEKeyMap					9
#define	kPKeyMap					36
#define	kQKeyMap					11
#define kSKeyMap					6
#define kColonMap					0x2E
#define kQuoteMap					0x20
#define	kCommandKeyMap				48
#define	kEscKeyMap					50
#define kSpaceBarMap				54

enum SoundEnums {
	kBirdSound = 1,
	kBonusSound,
	kBoom1Sound,
	kBoom2Sound,
	kSplashSound,
	kFlapSound,
	kGrateSound = 8,
	kLightningSound,
	kMusicSound,
	kScreechSound = 12,
	kSpawnSound,
	kWalkSound,
	kFlap2Sound,
	kScrape2Sound
};

#define kBirdPriority					80
#define kBonusPriority					85
#define kBoom1Priority					115
#define kBoom2Priority					110
#define kSplashPriority					75
#define kFlapPriority					70
#define kGratePriority					40
#define kLightningPriority				100
#define kMusicPriority					120
#define kScreechPriority				50
#define kSpawnPriority					90
#define kWalkPriority					30
#define kFlap2Priority					20
#define kScrape2Priority				10

#define kLavaHeight					456
#define kRoofHeight					2
#define kGravity					4

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


#define kInitNumLives				5
#define kMaxEnemies					8
#define kDontFlapVel				8

#define kOwl						0
#define kWolf						1
#define kJackal						2

#define iQuit						5
#define iSoundItem					7

enum GlyphaMenuIDs {
	kAppleMenuID = 128,
	kGameMenuID,
	kOptionsMenuID
};

enum GlyphaPictNums {
	kHandPictID =		128,
	kHandMaskID,
	kBackgroundPictID,
	kHelpPictID,
	kUnknownImageID1,
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
	kAboutSourceDialog = 134
};

#endif
