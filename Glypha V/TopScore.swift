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


final class ScoreList {
	struct TopScore {
		var playerName: String
		var playerScore: Int32
		var playerLevel: Int32
		
		init(name: String, score: Int32, level: Int32) {
			playerName = name
			playerScore = score
			playerLevel = level
		}
	}
}
