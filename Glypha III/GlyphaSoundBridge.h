//
//  GlyphaSoundBridge.h
//  Glypha
//
//  Created by C.W. Betts on 4/9/14.
//
//

#ifndef Glypha_GlyphaSoundBridge_h
#define Glypha_GlyphaSoundBridge_h

#include <CoreFoundation/CoreFoundation.h>

typedef CF_ENUM(int, GlyphaSounds) {
    kBirdSound = 0,
    kBonusSound,
    kBoom1Sound,
    kBoom2Sound,
    kSplashSound,
    kFlapSound,
    kFlap2Sound,
    kGrateSound,
    kLightningSound,
    kMusicSound,
    kScreechSound,
    kSpawnSound,
    kWalkSound,
    kScrape2Sound,
    
    kMaxSounds
};

#ifdef __cplusplus
extern "C" {
#endif

typedef void* GlyphaSound;

GlyphaSound NewGlyphaSound();

void PlayGlyphaSound(GlyphaSound theSnd, GlyphaSounds which);

void DeleteGlyphaSound(GlyphaSound theSnd);

#ifdef __cplusplus
}
#endif

#endif
