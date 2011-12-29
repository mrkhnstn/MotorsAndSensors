/*
 *  RadialConstForceField.h
 *  Chris
 *
 *  Created by Chris Mullany on 09/02/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */

#pragma once

#include "ForceField2f.h"

/**
 Applies forces from a specified centre point, thus creating a radial constant force
 */
class RadialConstForceField : public ForceField2f {
	
public:
	
	float x, y;
	float strength;
	ofxVec2f centre;
	
	float xOffset;
	ofxVec2f offset;
	
	RadialConstForceField(){
		x = ofGetWidth() / 2;
		y = ofGetHeight() / 2;
		strength = 1;
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		ofxVec2f force;
		
		centre.set(x, y);
		offset.set(xOffset, 0);
		
		if (pos_.x <= x)
			force = pos_ - (centre + offset);
		else
			force = pos_ - (centre - offset);

		
		//force = pos_ - centre;
		force.normalize();
		force *= strength;
		
		return force;
	}
	
};