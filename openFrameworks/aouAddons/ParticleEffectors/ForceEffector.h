#pragma once
/*
 *  ForceEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"
#include "ForceField2f.h"

/**
 - applies a force to each particle
 - the forces for each particle are retrieved from a force field in relation to the position of the particle
 - IMPORTANT: the force field has to be set in advance either using the constructor or by manually setting the force field pointer
 */
class ForceEffector: public ParticleEffector{
public:
	
	ForceField2f* field;
	
	ForceEffector(){
		this->field = NULL;
	}
	
	ForceEffector(ForceField2f* field){
		this->field = field;
	}
	
	void applyTo(Particle* particle){
		particle->force(field->getForce(particle->p));
	}
	
};

