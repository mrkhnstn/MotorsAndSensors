/*
 *  FboDstTransformModifier.h
 *  TestBed_Chris
 *
 *  Created by Chris Mullany on 21/03/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */


#pragma once

#include "FboModifier.h"

class FboDstTransformModifier : public FboModifier {
public:
	
	void leftUp(float scale, bool analog=false){
		output->decreaseDstY(scale);
	}
	void leftDown(float scale, bool analog=false){
		output->increaseDstY(scale);
	}
	void leftLeft(float scale, bool analog=false){
		output->decreaseDstX(scale);
	}
	void leftRight(float scale, bool analog=false){
		output->increaseDstX(scale);
	}
	
	void rightUp(float scale, bool analog=false){
		output->decreaseDstHeight(scale);
	}
	void rightDown(float scale, bool analog=false){
		output->increaseDstHeight(scale);
	}
	void rightLeft(float scale, bool analog=false){
		output->decreaseDstWidth(scale);
	}
	void rightRight(float scale, bool analog=false){
		output->increaseDstWidth(scale);
	}
	
	string getHUDString(){
		string debugString = "Destination Rect";
		debugString += "\nx: " + ofToString(output->dst.x, 2);
		debugString += "\ny: " + ofToString(output->dst.y, 2);
		debugString += "\nw: " + ofToString(output->dst.width, 2);
		debugString += "\nh: " + ofToString(output->dst.height, 2);
		return debugString;
	}
};