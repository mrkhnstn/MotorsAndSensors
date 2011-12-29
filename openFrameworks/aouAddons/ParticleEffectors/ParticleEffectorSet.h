#pragma once
/*
 *  ParticleEffectorSet.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

/**
 A container for multiple particle effectors
 */
class ParticleEffectorSet: public ParticleEffector{

public:
	
	set<ParticleEffector*> effectors;
	
	virtual void apply(set<Particle*>& particles){
		for(set<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->apply(particles);
		}
	}
	
	virtual void apply(vector<Particle*>& particles){
		for(set<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->apply(particles);
		}
	}

	virtual void applyTo(Particle* particle){
		for(set<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->applyTo(particle);
		}
	}
	
	void addEffector(ParticleEffector& effector){
		effectors.insert(&effector);
	}
	
	void removeEffector(ParticleEffector& effector){
		set<ParticleEffector*>::iterator it = effectors.find(&effector);
		if(it != effectors.end())
			effectors.erase(it);
	}

};