//
//  Entity.swift
//  Glypha
//
//  Created by C.W. Betts on 3/13/15.
//
//

import Cocoa
import SpriteKit


enum ObjectMode: Int16 {
	/// enemy & player mode
	case Idle = -1
	
	/// enemy & player mode
	case Flying
	
	/// enemy & player mode
	case Walking
	
	/// player mode
	case Sinking
	
	/// enemy mode
	case Spawning
	
	/// enemy mode & player mode
	case Falling
	
	/// enemy mode
	case EggTimer
	
	/// enemy mode
	case DeadAndGone
	
	/// player mode
	case Bones
	
	/// hand mode
	case Lurking = 10
	
	/// hand mode
	case OutGrabeth
	
	/// hand mode
	case Clutching
	
	/// eye mode
	case Waiting = 15
	
	/// eye mode
	case Stalking
};

protocol GlyphaEntity {
	var mode: ObjectMode {get}
	var destination: CGRect {get}
	
	init()
}

protocol GlyphaFlyingEntity: GlyphaEntity {
	/*static*/ var walkFrames: (left: (String, String), right: (String, String)) { get }
	/*static*/ var flyFrames: (left: (String, String), right: (String, String)) { get }
	init()
	
}

class Entity: SKSpriteNode {
	var internalEntity: GlyphaFlyingEntity
	required init?(coder aDecoder: NSCoder) {
	    fatalError("init(coder:) has not been implemented")
	}
	
	init(glyphaEntity: GlyphaFlyingEntity) {
		internalEntity = glyphaEntity
		
		let anImg = SKTexture(imageNamed: glyphaEntity.walkFrames.left.0)
		
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
