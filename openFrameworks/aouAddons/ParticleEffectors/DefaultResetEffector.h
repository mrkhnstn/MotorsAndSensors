#pragma once
/*
 *  DefaultResetEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

class DefaultResetEffector : public ParticleEffector {

public:
	
	DefaultResetEffector(){
		name = "DefaultResetEffector";
	}
	
	virtual void applyTo(Particle* particle){
		/*
		particle->setup();
		particle->jump(ofRandomuf()*ofGetWidth(),ofRandomuf()*ofGetHeight());
		particle->setState(ALIVE);
		*/
		particle->setState(DEAD);
	}
	
};

