//
//  GlyAppDelegate.h
//  Glypha III
//

//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <SpriteKit/SpriteKit.h>

@class GlyMyScene;

@interface GlyAppDelegate : NSObject <NSApplicationDelegate>

@property (weak) IBOutlet NSWindow *window;
@property (weak) IBOutlet SKView *skView;
@property (weak) GlyMyScene *skScene;

- (IBAction)pauseResumeGame:(id)sender;
- (IBAction)newGame:(id)sender;
- (IBAction)endGame:(id)sender;
- (IBAction)showHighScores:(id)sender;
- (IBAction)showLeaderboardScores:(id)sender;
- (IBAction)resetLocalScores:(id)sender;
- (IBAction)toggleSound:(id)sender;
- (IBAction)showGlyphaHelp:(id)sender;

@end
