//
//  TopScore.swift
//  Glypha
//
//  Created by C.W. Betts on 10/23/14.
//
//

import Foundation

private let kGlyHighPlayerName = "PlayerName"
private let kGlyHighPlayerScore = "PlayerScore"
private let kGlyHighPlayerLevel = "PlayerLevel"


class TopScore: NSObject, NSSecureCoding {
	let playerName: String
	let playerScore: Int32
	let playerLevel: Int16
	
	init(name: String, score: Int32, level: Int16) {
		playerName = name
		playerScore = score
		playerLevel = level
		
		super.init()
	}
	
	class func supportsSecureCoding() -> Bool {
		return true
	}

	func encodeWithCoder(aCoder: NSCoder) {
		aCoder.encodeObject(playerName, forKey: kGlyHighPlayerName)
		aCoder.encodeInt(playerScore, forKey: kGlyHighPlayerScore)
		aCoder.encodeInt(Int32(playerLevel), forKey: kGlyHighPlayerLevel)
	}

	required init(coder aDecoder: NSCoder) {
		self.playerName = aDecoder.decodeObjectForKey(kGlyHighPlayerName) as String
		self.playerScore = aDecoder.decodeIntForKey(kGlyHighPlayerScore)
		self.playerLevel = Int16(aDecoder.decodeIntForKey(kGlyHighPlayerLevel))

		
		super.init()
	}
}
