//
//  EnemyBird.swift
//  Glypha
//
//  Created by C.W. Betts on 5/21/15.
//
//

import Foundation

final class EnemyBird: GlyphaFlyingEntity {
	enum Stage {
		case Owl
		case Parrot
		case Wolf
	}
	
	private(set) var stage: Stage
	private(set) var walkFrames: (left: (String, String), right: (String, String))
	private(set) var flyFrames: (left: (String, String), right: (String, String))
	var mode: ObjectMode = .Idle
	var destination = CGRect.zero

	required convenience init() {
		self.init(stage: .Owl)
	}
	
	private func generateFrameNames() {
		let baseName: String
		switch stage {
		case .Owl:
			baseName = "Owl"
			
		case .Parrot:
			baseName = "Parrot"
			
		case .Wolf:
			baseName = "Wolf"
		}
		walkFrames = ((baseName + "-walk-l1", baseName + "-walk-l2"), (baseName + "-walk-r1", baseName + "-walk-r2"))
		flyFrames = ((baseName + "-fly-l1", baseName + "-fly-l2"), (baseName + "-fly-r1", baseName + "-fly-r2"))
	}
	
	func bumpStage() {
		switch stage {
		case .Owl:
			stage = .Parrot
			
		case .Parrot, .Wolf:
			stage = .Wolf
		}
	}
	
	init(stage: Stage) {
		self.stage = stage
		
		let baseName: String
		switch stage {
		case .Owl:
			baseName = "Owl"
			
		case .Parrot:
			baseName = "Parrot"
			
		case .Wolf:
			baseName = "Wolf"
		}
		walkFrames = ((baseName + "-walk-l1", baseName + "-walk-l2"), (baseName + "-walk-r1", baseName + "-walk-r2"))
		flyFrames = ((baseName + "-fly-l1", baseName + "-fly-l2"), (baseName + "-fly-r1", baseName + "-fly-r2"))
	}
	
	
}
