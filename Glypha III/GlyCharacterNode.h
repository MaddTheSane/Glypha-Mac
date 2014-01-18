//
//  GlyCharacterNode.h
//  Glypha
//
//  Created by C.W. Betts on 1/18/14.
//
//

#import <SpriteKit/SpriteKit.h>

@protocol GlyCharacter <NSObject>
+ (NSString*)fly1TextureName;
+ (NSString*)fly2TextureName;
+ (NSString*)walk1TextureName;
+ (NSString*)walk2TextureName;
@end

@interface GlyCharacterNode : SKSpriteNode
@property (strong) SKTexture *fly1Texture;
@property (strong) SKTexture *fly2Texture;
@property (strong) SKTexture *walk1Texture;
@property (strong) SKTexture *walk2Texture;

@end
