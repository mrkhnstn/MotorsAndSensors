#pragma once
/*
 *  ConstForceField2f.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 03/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"

class ConstForceField2f : public ForceField2f {

public:
	
	ofxVec2f force;
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		return force * strength;
	}
	
};