//
//  GlyMyScene.h
//  Glypha III
//

//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <SpriteKit/SpriteKit.h>
#include "GlyphaSoundBridge.h"

@interface GlyMyScene : SKScene
@property BOOL isPlayingGame;
@property BOOL isPausedGame;

- (IBAction)pauseResumeGame:(id)sender;
- (IBAction)newGame:(id)sender;
- (IBAction)endGame:(id)sender;
- (IBAction)showHighScores:(id)sender;
- (IBAction)showLeaderboardScores:(id)sender;
- (IBAction)resetLocalScores:(id)sender;
- (IBAction)toggleSound:(id)sender;
- (IBAction)showGlyphaHelp:(id)sender;

- (void)playSoundWithID:(GlyphaSounds)which;

@end
