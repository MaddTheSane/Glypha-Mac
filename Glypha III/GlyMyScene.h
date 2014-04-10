//
//  GlyMyScene.h
//  Glypha III
//

//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <SpriteKit/SpriteKit.h>

@interface GlyMyScene : SKScene
@property BOOL isPlayingGame;
@property BOOL isPausedGame;

- (IBAction)pauseResumeGame:(id)sender;

@end
