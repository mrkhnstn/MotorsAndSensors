/*
 *  FboSrcTransformModifier.h
 *  TestBed_Chris
 *
 *  Created by Chris Mullany on 21/03/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */



#pragma once

#include "FboModifier.h"

class FboSrcTransformModifier : public FboModifier {
public:
	
	void leftUp(float scale, bool analog=false){
		output->increaseSrcY(scale);
	}
	void leftDown(float scale, bool analog=false){
		output->decreaseSrcY(scale);
	}
	void leftLeft(float scale, bool analog=false){
		output->increaseSrcX(scale);
	}
	void leftRight(float scale, bool analog=false){
		output->decreaseSrcX(scale);
	}
	
	void rightUp(float scale, bool analog=false){
		output->increaseSrcHeight(scale);
	}
	void rightDown(float scale, bool analog=false){
		output->decreaseSrcHeight(scale);
	}
	void rightLeft(float scale, bool analog=false){
		output->increaseSrcWidth(scale);
	}
	void rightRight(float scale, bool analog=false){
		output->decreaseSrcWidth(scale);
	}
	
	string getHUDString(){
		string debugString = "Source Rect";
		debugString += "\nx: " + ofToString(output->src.x, 2);
		debugString += "\ny: " + ofToString(output->src.y, 2);
		debugString += "\nw: " + ofToString(output->src.width, 2);
		debugString += "\nh: " + ofToString(output->src.height, 2);
		return debugString;
	}
};