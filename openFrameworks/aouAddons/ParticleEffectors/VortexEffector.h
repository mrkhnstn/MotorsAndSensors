#pragma once
/*
 *  VortexEffector.h
 *  TestBed_Mark
 *
 *  Created by TAQA_CH_0 on 28/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"
#include "ForceField2f.h"
#include "Boundary1f.h"
class VortexForceField : public ForceField2f {
	
public:
	
	ofxVec2f centre;
	float radius;
	float gravity;
	float swirl;
	Boundary1f swirlFactorRange;
	
	VortexForceField(){
		radius = 100;
		gravity = 1;
		swirl = 1;
	}
	
	ofxVec2f getForce(ofxVec2f& pos_){
		
		ofxVec2f force;
		
		if (strength == 0) return force;
		
		ofxVec2f delta = centre - pos_;
		float d2 = delta.lengthSquared();
		float d3 = d2;
		
		if (d2 > radius * radius) {
			d2 = swirl / ofClamp(d2, swirlFactorRange.min, swirlFactorRange.max);
			force.set(delta.y * d2, - delta.x * d2);
		}
		
		if (d3 > radius * radius) {
			delta *= gravity;
			force += delta;
		}
		 
		return force * strength;
	}
	
	
};
