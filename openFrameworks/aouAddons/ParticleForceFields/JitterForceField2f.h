#pragma once
/*
 *  JitterForceField2f.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 16/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"

class JitterForceField2f : public ForceField2f {
	
public:
	
	ofxVec2f strength;
	
	JitterForceField2f(){
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		ofxVec2f force(strength.x*(ofRandomuf()-0.5), strength.y*(ofRandomuf()-0.5));
		return force;
	}
	
};

////////////////////////////////////////////////////////////////////////

class JitterForceField2fPlus : public ForceField2f {
	
public:
	
	ofxVec2f strength;
	float probability;
	
	JitterForceField2fPlus(){
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		if(ofRandomuf() < probability){
			ofxVec2f force(strength.x*(ofRandomuf()-0.5), strength.y*(ofRandomuf()-0.5));
			return force;
		} else {
			return ofxVec2f();
		}
	}
	
};