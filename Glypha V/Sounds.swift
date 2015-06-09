//
//  Sounds.swift
//  Glypha
//
//  Created by C.W. Betts on 3/13/15.
//
//

import Foundation
import AVFoundation
#if false
import OpenAL.AL
import OpenAL.ALC
import OpenAL.OSXExtensions
#endif

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
	private var sounds: [[AVAudioPlayer]]
	
	init() {
		sounds = [[AVAudioPlayer]]()
		
		for i in 0 ..< GlyphaSound.MaxSounds.rawValue {
			sounds.append([AVAudioPlayer]())
		}
		
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
	
	func load(which: GlyphaSound, named: String, fileExtension: String? = "aiff") {
		if let aURL = NSBundle.mainBundle().URLForResource(named, withExtension: fileExtension) {
			load(which, loadURL: aURL)
		}
	}
	
	func load(which: GlyphaSound, loadURL: NSURL) {
		if let curData = NSData(contentsOfURL: loadURL) {
			load(which, data: curData)
		}
	}
	
	func load(which: GlyphaSound, data: NSData) {
		let count = which.preloadCount
		for i in 0 ..< count {
			let player = AVAudioPlayer(data: data, error: nil)
			player.prepareToPlay()
			
			sounds[which.rawValue].append(player)
		}
	}
	
	func load(which: GlyphaSound, buffer: UnsafePointer<()>, size: Int, copyData: Bool = true) {
		var data: NSData
		if copyData {
			data = NSData(bytes: buffer, length: size)
		} else {
			data = NSData(bytesNoCopy: UnsafeMutablePointer<Void>(buffer), length: size, freeWhenDone: false)
		}
		load(which, data: data)
	}
	
	func play(which: GlyphaSound) {
		var found = false
		let anArray = sounds[which.rawValue]
		for player in anArray {
			if !player.playing {
				player.play()
				found = true
				break;
			}
		}
		
		if !found {
			println("Preloaded sound not available for \(which), \(which.rawValue)")
			if let audioData = anArray.first!.data {
				let player = AVAudioPlayer(data: audioData, error: nil)
				sounds[which.rawValue].append(player)
				player.play()
			} else if let urlPath = anArray.first!.url {
				let player = AVAudioPlayer(contentsOfURL: urlPath, error: nil)
				sounds[which.rawValue].append(player)
				player.play()
			}
		}
	}
}
