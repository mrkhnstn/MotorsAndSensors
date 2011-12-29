#pragma once
/*
 *  ForceFieldEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 06/04/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceFieldSet2f.h"
#include "ParticleEffector.h"

class ForceFieldEffector : public ParticleEffector {
	
public:
	
	void applyTo(Particle* particle){
		particle->force(forceFieldSet.getForce(particle->p));
	}
	
	void addField(ForceField2f& field){
		forceFieldSet.addField(field);
	}
	
	void removeField(ForceField2f& field){
		forceFieldSet.removeField(field);
	}
	
protected:
	
	ForceFieldSet2f forceFieldSet;
	
};