//
//  Sounds.swift
//  Glypha
//
//  Created by C.W. Betts on 3/13/15.
//
//

import Foundation
import SpriteKit

enum GlyphaSound: Int {
	case Bird = 0
	case Bonus
	case Boom1
	case Boom2
	case Splash
	case Flap1
	case Flap2
	case Grate
	case Lightning
	case Music
	case Screech
	case Spawn
	case Walk
	case Scrape2
	
	case MaxSounds
	
	var preloadCount: Int {
		switch (self) {
		case .Bonus, .Flap1, .Grate, .Lightning, .Spawn, .Walk:
			return 3;
		case .Flap2, .Scrape2, .Screech:
			return 8;
		default:
			return 1;
		}
	}
}

class SoundManager {
	private var sounds: [SKAction]

	init() {
		
		sounds = [SKAction](count: GlyphaSound.MaxSounds.rawValue, repeatedValue: SKAction())
		
		load(.Bird, named: "Bird")
		load(.Bonus, named: "Bonus");
		load(.Boom1, named: "Boom 1");
		load(.Boom2, named: "Boom 2");
		load(.Splash, named: "Splash");
		load(.Flap1, named: "Flap");
		load(.Flap2, named: "Flap 2");
		load(.Grate, named: "Grate");
		load(.Lightning, named: "Lightning");
		load(.Music, named: "Music");
		load(.Screech, named: "Screech");
		load(.Spawn, named: "Spawn");
		load(.Walk, named: "Walk");
		load(.Scrape2, named: "Scrape 2");
	}
	
	private func load(which: GlyphaSound, named: String, fileExtension: String? = "aiff") {
		var fileName = named
		if let fileExtension = fileExtension {
			fileName = (fileName as NSString).stringByAppendingPathExtension(fileExtension)!
		}
		
		sounds[which.rawValue] = SKAction.playSoundFileNamed(fileName, waitForCompletion: false)
	}
	
	func play(which: GlyphaSound, node: SKNode? = nil) {
		let anObj = sounds[which.rawValue]
		
		if let node = node {
			node.runAction(anObj)
		} else {
			let appDel = NSApp.delegate as! AppDelegate
			appDel.skView.scene?.runAction(anObj)
		}
	}
}
