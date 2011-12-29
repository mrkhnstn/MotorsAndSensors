#pragma once
/*
 *  ParticleEffectorList.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 26/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

/**
 A container for multiple particle effectors
 */
class ParticleEffectorList: public ParticleEffector{
	
public:
	
	list<ParticleEffector*> effectors;
	
	void update(){
		for(list<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->update();
		}
	}
	
	void apply(ParticlePool& pool){
		for(list<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->apply(pool);
		}		
	}
	
	virtual void apply(vector<Particle*>& particles){
		for(list<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->apply(particles);
		}
	}
	
	virtual void apply(set<Particle*>& particles){
		for(list<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->apply(particles);
		}
	}
	
	virtual void applyTo(Particle* particle){
		for(list<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it){
			ParticleEffector* effector = *it;
			effector->applyTo(particle);
		}
	}
	
	void addEffector(ParticleEffector& effector){
		effectors.push_back(&effector);
	}
	
	void removeEffector(ParticleEffector& effector){
		effectors.remove(&effector);
	}
	
	void clearEffectors(){
		effectors.clear();
	}
	
};