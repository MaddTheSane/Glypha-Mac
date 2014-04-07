//
//  GlyTopScore.m
//  Glypha
//
//  Created by C.W. Betts on 4/7/14.
//
//

#import "GlyTopScore.h"

#define kGlyHighPlayerName @"PlayerName"
#define kGlyHighPlayerScore @"PlayerScore"
#define kGlyHighPlayerLevel @"PlayerLevel"

@implementation GlyTopScore

- (void)encodeWithCoder:(NSCoder *)aCoder
{
	[aCoder encodeObject:_playerName forKey:kGlyHighPlayerName];
	[aCoder encodeInt:_playerScore forKey:kGlyHighPlayerScore];
	[aCoder encodeInt:_playerLevel forKey:kGlyHighPlayerLevel];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	if (self = [super init]) {
		self.playerName = [aDecoder decodeObjectForKey:kGlyHighPlayerName];
		self.playerScore = [aDecoder decodeIntForKey:kGlyHighPlayerScore];
		self.playerLevel = [aDecoder decodeIntForKey:kGlyHighPlayerLevel];
	}
	
	return self;
}


@end
