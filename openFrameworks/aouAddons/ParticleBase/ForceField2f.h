#pragma once
/*
 *  ForceField2f.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 03/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVectorMath.h"


class ForceField2f {

public:
	
	float strength; //< scales output force
	
	ForceField2f(){
		strength = 1;
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		ofxVec2f force;
		return strength * force;
	}	
};

