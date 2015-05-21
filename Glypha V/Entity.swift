//
//  Entity.swift
//  Glypha
//
//  Created by C.W. Betts on 3/13/15.
//
//

import Cocoa
import SpriteKit

protocol GlyphaEntity {
	/*static*/ var walkFrames: (left: (String, String), right: (String, String)) { get }
	/*static*/ var flyFrames: (left: (String, String), right: (String, String)) { get }
	init()
}

class Entity: SKSpriteNode {
	var internalEntity: GlyphaEntity
	required init?(coder aDecoder: NSCoder) {
	    fatalError("init(coder:) has not been implemented")
	}
	
	init(glyphaEntity: GlyphaEntity) {
		internalEntity = glyphaEntity
		
		var anImg = SKTexture(imageNamed: glyphaEntity.walkFrames.left.0)
		
		super.init(texture: anImg, color: NSColor.whiteColor(), size: CGSize.zeroSize)
	}
	
	#if false
	class var walkFrames: (left: (String, String), right: (String, String)) {
		return (("", ""), ("", ""))
	}
	
	class var flyFrames: (left: (String, String), right: (String, String)) {
		return (("", ""), ("", ""))
	}
	#endif
}
