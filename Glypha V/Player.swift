//
//  Player.swift
//  Glypha
//
//  Created by C.W. Betts on 3/13/15.
//
//

import Foundation

class Player: GlyphaEntity {
	var walkFrames: (left: (String, String), right: (String, String)) {
		return (("Player-walk-l1", "Player-walk-l2"), ("Player-walk-r1", "Player-walk-r2"))
	}
	
	var flyFrames: (left: (String, String), right: (String, String)) {
		return (("", ""), ("", ""))
	}

	required init() {
		
	}
}
