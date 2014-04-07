//
//  GlyTopScore.h
//  Glypha
//
//  Created by C.W. Betts on 4/7/14.
//
//

#import <Foundation/Foundation.h>

@interface GlyTopScore : NSObject <NSCoding>
@property (copy) NSString *playerName;
@property int playerScore;
@property short playerLevel;

@end
