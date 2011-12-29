#pragma once
/*
 *  VerticalSplitConstForceField.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 24/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"

/**
- applies a negative horizontal force to particles left of an vertical axis and a positive horizontal force to particles right of the axis
- the force is defined by strength 
 */
class VerticalSplitConstForceField : public ForceField2f {
	
public:
	
	float axisX;
	float strength;
	
	VerticalSplitConstForceField(){
		axisX = 0;
		strength = 0;
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		ofxVec2f force;
		if (pos_.x <= axisX) {
			force.x = -strength;
		} else {
			force.x = strength;
		}
		return force;
	}
	
};

