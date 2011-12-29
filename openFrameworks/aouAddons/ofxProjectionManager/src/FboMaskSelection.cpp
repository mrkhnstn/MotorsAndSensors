/*
 *  FboMaskSelection.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 26/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboMaskSelection.h"

FboMaskSelection::FboMaskSelection(){
	shapeID = -1;
	pointID = -1;
	valid = false;
	midPoint = false;
}

float FboMaskSelection::distance(ofxVec2f& target){
	return point.distance(target);
}

void FboMaskSelection::setPoint(ofxVec2f& target, int pointID, int shapeID){
	point = target;
	if(pointID != -1){
		this->pointID = pointID;
	}
	if(shapeID != -1){
		this->shapeID = shapeID;
	}
	valid = true;
}
