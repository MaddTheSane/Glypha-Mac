//
//  Hand.swift
//  Glypha
//
//  Created by C.W. Betts on 5/21/15.
//
//

import Foundation

class Hand: GlyphaEntity {
	var mode: ObjectMode = .Idle
	var destination = CGRect.zeroRect
	
	required init() {
		
	}
}

