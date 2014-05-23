#include "GLSounds.h"
#include <AVFoundation/AVFoundation.h>
#include <list>

struct Context {
    std::list<AVAudioPlayer*> sounds[kMaxSounds];
};

GL::Sounds::Sounds()
{
    initContext();
    load(kBirdSound, @"Bird");
    load(kBonusSound, @"Bonus");
    load(kBoom1Sound, @"Boom 1");
    load(kBoom2Sound, @"Boom 2");
    load(kFlap2Sound, @"Flap 2");
    load(kFlapSound, @"Flap");
    load(kGrateSound, @"Grate");
    load(kLightningSound, @"Lightning");
    load(kMusicSound, @"Music");
    load(kScrape2Sound, @"Scrape 2");
    load(kScreechSound, @"Screech");
    load(kSpawnSound, @"Spawn");
    load(kSplashSound, @"Spawn");
    load(kWalkSound, @"Walk");
}

GL::Sounds::~Sounds()
{
    Context *contex = (Context*)context;
    delete contex;
}

void GL::Sounds::initContext()
{
    context = new Context;
}

void GL::Sounds::play(GlyphaSounds which)
{
    Context *ctx = static_cast<Context*>(context);
    bool found = false;
    for (std::list<AVAudioPlayer*>::const_iterator it = ctx->sounds[which].begin(); it != ctx->sounds[which].end(); ++it) {
        AVAudioPlayer *player = *it;
        if (!player.isPlaying) {
            [player play];
            found = true;
            break;
        }
    }
    if (!found) {
        NSLog(@"Preloaded sound not available for %d", which);
        AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithData:ctx->sounds[which].front().data error:nil];
        if (!player) {
            player = [[AVAudioPlayer alloc] initWithContentsOfURL:ctx->sounds[which].front().url error:NULL];
        }
        ctx->sounds[which].push_back(player);
        [player play];
    }
}

void GL::Sounds::load(GlyphaSounds which, NSString* loadURL)
{
    load(which, [[NSBundle mainBundle] URLForResource:loadURL withExtension:@"aiff"]);
}

void GL::Sounds::load(GlyphaSounds which, NSURL* loadURL)
{
    NSData *curData = [[NSData alloc] initWithContentsOfURL:loadURL];
    load(which, curData);
}

void GL::Sounds::load(GlyphaSounds which, NSData *theData)
{
    Context *ctx = static_cast<Context*>(context);
    int count = preloadCount(which);
    for (int i = 0; i < count; ++i) {
        AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithData:theData error:nil];
        [player prepareToPlay];
        ctx->sounds[which].push_back(player);
    }
}

void GL::Sounds::load(GlyphaSounds which, const unsigned char *buf, unsigned bufLen)
{
    NSData *data = [[NSData alloc] initWithBytesNoCopy:(void*)buf length:bufLen freeWhenDone:NO];
    load(which, data);
}

GlyphaSound NewGlyphaSound()
{
    return new GL::Sounds();
}

void PlayGlyphaSound(GlyphaSound theSnd, GlyphaSounds which)
{
    if (!theSnd) {
        return;
    }

    GL::Sounds *GlySound = (GL::Sounds*)theSnd;
    GlySound->play(which);
}

void DeleteGlyphaSound(GlyphaSound theSnd)
{
    if (!theSnd) {
        return;
    }

    GL::Sounds *GlySound = (GL::Sounds*)theSnd;
    delete GlySound;
}
