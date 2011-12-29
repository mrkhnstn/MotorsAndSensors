#pragma once
/*
 *  ParticleEffector.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 16/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"
#include "ParticlePool.h"
#include "ofxBaseObject.h"
#include <set>

class ParticleEffector : public ofxBaseObject {
public:
		
	ParticleEffector() : ofxBaseObject(){
		enabled = true;
		name = "ParticleEffector";
	}
	
	virtual void apply(ParticlePool& pool){
		//cout << "X" << endl;
		apply(pool.getAlive());
	}
	
	virtual void apply(set<Particle*>& particles){
		set<Particle*>::iterator it;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* particle = *it;
			applyTo(particle);
		}			
	}
	
	virtual void apply(vector<Particle*>& particles){
		vector<Particle*>::iterator it;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* particle = *it;
			applyTo(particle);
		}			
	}
	
	virtual void applyTo(Particle* particle){
	}
	
};






