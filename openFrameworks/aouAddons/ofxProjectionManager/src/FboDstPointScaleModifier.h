/*
 *  FboDstPointScaleModifier.h
 *  TestBed_Chris
 *
 *  Created by TAQA_CH_1 on 10/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "FboModifier.h"

class FboDstPointScaleModifier : public FboModifier {
public:
	
	void leftUp(float scale, bool analog=false){
		if(!analog)
		output->changeSelection(0, -scale);
	}
	void leftDown(float scale, bool analog=false){
		if(!analog)
		output->changeSelection(0, scale);
	}
	void leftLeft(float scale, bool analog=false){
		if(!analog)
		output->changeSelection(-scale, 0);
	}
	void leftRight(float scale, bool analog=false){
		if(!analog)
		output->changeSelection(scale, 0);
	}
	void rightUp(float scale, bool analog=false){
		output->scaleSelection(scale);
	}
	void rightDown(float scale, bool analog=false){
		output->scaleSelection(-scale);
	}
	void rightLeft(float scale, bool analog=false){
	}
	void rightRight(float scale, bool analog=false){
	}
	string getHUDString(){
		return "FboDstPointScaleModifier";
	}
};