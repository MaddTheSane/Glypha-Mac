//
//  GlyphaSoundBridge.h
//  Glypha
//
//  Created by C.W. Betts on 4/9/14.
//
//

#ifndef Glypha_GlyphaSoundBridge_h
#define Glypha_GlyphaSoundBridge_h

enum {
    kBirdSound = 0,
    kBonusSound,
    kBoom1Sound,
    kBoom2Sound,
    kSplashSound,
    kFlapSound,
    kGrateSound,
    kLightningSound,
    kMusicSound,
    kScreechSound,
    kSpawnSound,
    kWalkSound,
    kFlap2Sound,
    kScrape2Sound,
    
    kMaxSounds
};

#ifdef __cplusplus
extern "C" {
#endif
	typedef void* GlyphaSound;
	
	GlyphaSound NewGlyphaSound();
	
	void PlayGlyphaSound(GlyphaSound theSnd, int which);
	
	void DeleteGlyphaSound(GlyphaSound theSnd);
	
#ifdef __cplusplus
}
#endif

#endif
