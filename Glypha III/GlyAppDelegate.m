//
//  GlyAppDelegate.m
//  Glypha III
//
//  Created by C.W. Betts on 11/27/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "GlyAppDelegate.h"
#import "GlyMyScene.h"

@implementation GlyAppDelegate

@synthesize window = _window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	/* Pick a size for the scene */
	GlyMyScene *scene = [GlyMyScene sceneWithSize:CGSizeMake(640, 480)];
	
	/* Set the scale mode to scale to fit the window */
	scene.scaleMode = SKSceneScaleModeAspectFit;
	
	[self.skView presentScene:scene];
#ifdef DEBUG
	self.skView.showsFPS = YES;
	self.skView.showsNodeCount = YES;
#endif
	self.skScene = scene;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return YES;
}

- (IBAction)pauseResumeGame:(id)sender
{
	[self.skScene pauseResumeGame:sender];
}

- (IBAction)newGame:(id)sender {
}

- (IBAction)endGame:(id)sender {
}
@end
