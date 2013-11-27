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
	SKScene *scene = [GlyMyScene sceneWithSize:CGSizeMake(1024, 768)];
	
	/* Set the scale mode to scale to fit the window */
	scene.scaleMode = SKSceneScaleModeAspectFit;
	
	[self.skView presentScene:scene];
#ifdef DEBUG
	self.skView.showsFPS = YES;
	self.skView.showsNodeCount = YES;
#endif
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return YES;
}

@end
