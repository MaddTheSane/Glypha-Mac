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

        void initContext();
        void load(GlyphaSounds which, NSString* bundLoad);
        void load(GlyphaSounds which, NSURL* loadURL);
        void load(GlyphaSounds which, const unsigned char *buf, unsigned bufLen, bool copyData = false);
        void load(GlyphaSounds which, NSData *theData);
        void play(GlyphaSounds which);

    private:
        static int preloadCount(GlyphaSounds which) {
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
                default:
                    break;
            }
            return 1;
        }
        void *context;
    };
}

#endif