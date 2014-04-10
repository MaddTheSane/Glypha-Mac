#ifndef GLSOUNDS_H
#define GLSOUNDS_H

//TODO: use OpenAL: This is more of a stopgap method than the final implementation.
#import <Cocoa/Cocoa.h>

#include "GlyphaSoundBridge.h"

namespace GL {

class Sounds {
public:
    Sounds();
    ~Sounds();
	
    int preloadCount(int which) {
        switch (which) {
            case kBonusSound:
            case kFlapSound:
            case kGrateSound:
            case kLightningSound:
            case kSpawnSound:
            case kWalkSound:
                return 3;
            case kFlap2Sound:
            case kScrape2Sound:
            case kScreechSound:
                return 8;
        }
        return 1;
    }
    
    void initContext();
	void load(int which, NSString* bundLoad);
	void load(int which, NSURL* loadURL);
    void load(int which, const unsigned char *buf, unsigned bufLen);
    void play(int which);
    
private:
    void *context;
};
    
}

#endif