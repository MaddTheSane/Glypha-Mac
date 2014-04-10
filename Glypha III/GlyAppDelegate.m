//
//  GlyAppDelegate.m
//  Glypha III
//
//  Created by C.W. Betts on 11/27/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "GlyAppDelegate.h"
#import "GlyMyScene.h"
#import <GameKit/GameKit.h>

@interface GlyAppDelegate ()
@property GKLocalPlayer *glyLocalPlayer;
- (void)authenticatedPlayer:(GKLocalPlayer*)localPlayer;
- (void)showAuthenticationDialogWhenReasonable:(NSViewController*)vc;
- (void)disableGameCenter;
@end

@implementation GlyAppDelegate
@synthesize window = _window;

- (void)authenticateLocalPlayer
{
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
	self.glyLocalPlayer = localPlayer;
    localPlayer.authenticateHandler = ^(NSViewController *viewController, NSError *error){
		if (viewController != nil) {
			//showAuthenticationDialogWhenReasonable: is an example method name. Create your own method that displays an authentication view when appropriate for your app.
			[self showAuthenticationDialogWhenReasonable: viewController];
		} else if (self.glyLocalPlayer.isAuthenticated) {
			//authenticatedPlayer: is an example method name. Create your own method that is called after the loacal player is authenticated.
			[self authenticatedPlayer:self.glyLocalPlayer];
		} else {
			[self disableGameCenter];
		}
	};
}

- (void)authenticatedPlayer:(GKLocalPlayer*)localPlayer
{
	
}

- (void)showAuthenticationDialogWhenReasonable:(NSViewController*)vc
{
	
}

- (void)disableGameCenter
{
	self.glyLocalPlayer = nil;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	/* Pick a size for the scene */
	GlyMyScene *scene = [GlyMyScene sceneWithSize:CGSizeMake(640, 480)];
	
	[self authenticateLocalPlayer];
	
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

- (IBAction)newGame:(id)sender
{
	
}

- (IBAction)endGame:(id)sender
{
	
}

- (IBAction)showHighScores:(id)sender
{
	
}

- (IBAction)showLeaderboardScores:(id)sender
{
	
}

- (IBAction)resetLocalScores:(id)sender
{
	
}

- (IBAction)toggleSound:(id)sender
{
	
}

- (IBAction)showGlyphaHelp:(id)sender
{
	
}

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem
{
	SEL menuAction = [menuItem action];
	if (menuAction == @selector(pauseResumeGame:)) {
		switch ([menuItem tag]) {
			case 1:
				if (_skScene.isPlayingGame) {
					if (_skScene.isPausedGame) {
						menuItem.hidden = YES;
					} else {
						menuItem.hidden = NO;
					}
					return YES;
				} else {
					menuItem.hidden = NO;
					return NO;
				}

				break;
				
			case 2:
				if (_skScene.isPlayingGame) {
					if (_skScene.isPausedGame) {
						menuItem.hidden = NO;
					} else {
						menuItem.hidden = YES;
					}
					return YES;
				} else {
					menuItem.hidden = YES;
					return NO;
				}
				break;
			default:
				return NO;
				break;
		}
	} else if (menuAction == @selector(newGame:)) {
		if (_skScene.isPlayingGame) {
			return NO;
		} else {
			return YES;
		}
	} else {
		return YES;
	}
}
@end
