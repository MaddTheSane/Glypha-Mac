//
//  GlyMyScene.m
//  Glypha III
//
//  Created by C.W. Betts on 11/27/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "GlyMyScene.h"
#include "GlyphaSoundBridge.h"

@implementation GlyMyScene
{
	GlyphaSound theSound;
}

- (IBAction)pauseResumeGame:(id)sender
{
	self.isPausedGame = !_isPausedGame;
}

- (void)playSoundWithID:(GlyphaSounds)which
{
	PlayGlyphaSound(theSound, which);
}

- (id)initWithSize:(CGSize)size
{
	if (self = [super initWithSize:size]) {
		self.isPlayingGame = NO;
		theSound = NewGlyphaSound();
		/* Setup your scene here */
		
		self.backgroundColor = [SKColor colorWithRed:0.15 green:0.15 blue:0.3 alpha:1.0];
		
		SKLabelNode *myLabel = [SKLabelNode labelNodeWithFontNamed:@"Chalkduster"];
		
		myLabel.text = @"Hello, World!";
		myLabel.fontSize = 65;
		myLabel.position = CGPointMake(CGRectGetMidX(self.frame),
									   CGRectGetMidY(self.frame));
		
		[self addChild:myLabel];
	}
	return self;
}

- (void)dealloc
{
	DeleteGlyphaSound(theSound);
}

- (void)mouseDown:(NSEvent *)theEvent
{
	/* Called when a mouse click occurs */
	
	CGPoint location = [theEvent locationInNode:self];
	SKSpriteNode *sprite = [SKSpriteNode spriteNodeWithImageNamed:@"Player-death-fly"];
	
	sprite.position = location;
	sprite.scale = 1.0;
	
	SKAction *action = [SKAction rotateByAngle:M_PI duration:1];
	
	[sprite runAction:[SKAction repeatActionForever:action]];
	
	[self playSoundWithID:kLightningSound];
	
	[self addChild:sprite];
}

- (void)update:(CFTimeInterval)currentTime
{
	/* Called before each frame is rendered */
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

@end
