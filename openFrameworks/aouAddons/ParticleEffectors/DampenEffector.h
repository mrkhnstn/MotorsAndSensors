#pragma once
/*
 *  DampenEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

/**
dampens particles
*/
class DampenEffector: public ParticleEffector{
	
public:
	
	float dampening;
	
	void applyTo(Particle* particle){
		particle->op = particle->op.interpolate(particle->p, dampening);
	}
	
};