//
//  Eye.swift
//  Glypha
//
//  Created by C.W. Betts on 5/21/15.
//
//

import Foundation

class Eye: GlyphaEntity {
	var mode: ObjectMode = .Waiting
	private(set) var destination = CGRect.zero
	private var frame: Int = 0
	private(set) var srcNum = 0
	private(set) var opening = 1
	private(set) var killed = false
	private(set) var entering = false
	required init() {
		/*
SetRect(&theEye.dest, 0, 0, 48, 31);
OffsetRect(&theEye.dest, 296, 97);
theEye.frame = (numOwls + 2) * 720;
*/
	}
}

/*
typedef struct {
Rect		dest;
objectModes mode;
short		opening;
short		srcNum, frame;
Boolean		killed, entering;
} eyeInfo;
*/
