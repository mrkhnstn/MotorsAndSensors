/*
 *  FboDstPointModifier.h
 *  TestBed_Chris
 *
 *  Created by TAQA_CH_1 on 10/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "FboModifier.h"

class FboDstPointModifier : public FboModifier {
public:
	void leftUp(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		if(!analog)
		output->changeSelection(0, -1);
	}
	void leftDown(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		if(!analog)
		output->changeSelection(0, 1);
	}
	void leftLeft(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		if(!analog)
		output->changeSelection(-1, 0);
	}
	void leftRight(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		if(!analog)
		output->changeSelection(1, 0);
	}
	void rightUp(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		output->shiftSelection(0, -scale);
	}
	void rightDown(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		output->shiftSelection(0, scale);
	}
	void rightLeft(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		output->shiftSelection(-scale, 0);
	}
	void rightRight(float scale, bool analog=false){
		output->editMode = WARP_EDIT_POINT;
		output->shiftSelection(scale, 0);
	}
	string getHUDString(){
		string debugString = "Point";
		debugString += "\noffset: " + ofToString(output->grid.warpGrid[output->editingPoint].x, 2) + ", " 
		+ ofToString(output->grid.warpGrid[output->editingPoint].y, 2);
		return debugString;
	}
};

class FboDstRowModifier : public FboModifier {
public:
	
	void leftUp(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		if(!analog)
		output->changeSelection(0, -1);
	}
	void leftDown(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		if(!analog)
		output->changeSelection(0, 1);
	}
	void leftLeft(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		output->scaleSelection(scale * 0.05);
	}
	void leftRight(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		output->scaleSelection(-scale * 0.05);
	}
	void rightUp(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		output->shiftSelection(0, -scale);
	}
	void rightDown(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		output->shiftSelection(0, scale);
	}
	void rightLeft(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		output->shiftSelection(-scale, 0);
	}
	void rightRight(float scale, bool analog=false){
		output->editMode = WARP_EDIT_ROW;
		output->shiftSelection(scale, 0);
	}
	string getHUDString(){
		string debugString = "Modify Rows";
		return debugString;
	}
};

class FboDstColModifier : public FboModifier {
public:
	
	void leftUp(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		output->scaleSelection(scale*0.1);
	}
	void leftDown(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		output->scaleSelection(-scale*0.05);
	}
	void leftLeft(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		if(!analog)
		output->changeSelection(-1, 0);
	}
	void leftRight(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		if(!analog)
		output->changeSelection(1, 0);
	}
	void rightUp(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		output->shiftSelection(0, -scale);
	}
	void rightDown(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		output->shiftSelection(0, scale);
	}
	void rightLeft(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		output->shiftSelection(-scale, 0);
	}
	void rightRight(float scale, bool analog=false){
		output->editMode = WARP_EDIT_COLUMN;
		output->shiftSelection(scale, 0);
	}
	string getHUDString(){
		string debugString = "Modify Columns";
		return debugString;
	}
};