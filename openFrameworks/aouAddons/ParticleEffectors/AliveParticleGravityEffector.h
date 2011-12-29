#pragma once
/*
 *  AliveParticleGravityEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 01/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "ParticleGravityEffector.h"
#include "ParticlePool.h"

class AliveParticleGravityEffector : public ParticleGravityEffector {

public:

	ParticlePool* pool;
	
	AliveParticleGravityEffector(){
		pool = NULL;
	}
	
	void apply(set<Particle*>& particles){
		if (pool == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	void apply(vector<Particle*>& particles){
		if (pool == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	vector<Particle*>& getGravityParticles(){
		return pool->getAlive();
	}
};

// alive repulsion ////////////////////////////////////////////////

/*
class AliveParticleRepulsionEffector : public ParticleGravityEffector {
	
public:
	
	ParticlePool* pool;
	
	AliveParticleRepulsionEffector(){
		pool = NULL;
	}
	
	void apply(set<Particle*>& particles){
		if (pool == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	void apply(vector<Particle*>& particles){
		if (pool == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	vector<Particle*>& getGravityParticles(){
		return pool->getAlive();
	}
};
*/