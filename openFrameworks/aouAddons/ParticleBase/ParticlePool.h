#pragma once
/*
 *  ParticlePool.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 14/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"
#include <list>
#include <set>
#include <vector>
#include "Utils.h"
#include "ofxBaseObject.h"

//template<class T>
typedef Particle T;
class ParticlePool : public ofxBaseObject{
public:
	
	ParticlePool(){
		particleCount = 0;
	}
	
	~ParticlePool(){
		ClearPointerVector(particles);
	}
	
	virtual void setup(int particleCount){
		if(particleCount != this->particleCount){
			this->particleCount = particleCount;
			setup();
		}
	}
	
	virtual void setup(){
		ClearPointerVector(particles);
		
		particles.clear();
		alive.clear();
		dead.clear();
		
		particles.reserve(particleCount);
		alive.reserve(particleCount);
		dead.reserve(particleCount);
		
		for(int i = 0; i < particleCount; ++i){
			Particle* particle = new Particle();
			particles.push_back(particle);
		}
	}
	
	/**
	 separate all particles into alive and dead particle lists
	 */
	virtual void update(){
		
		alive.clear();
		dead.clear();
		
		vector<T*>::iterator it;
		T* particle;
		for(it = particles.begin(); it != particles.end(); ++it){
			particle = *it;
			if(particle->isAlive()){
				alive.push_back(particle);
			} else {
				dead.push_back(particle);
			}
		}
		
	}
	
	virtual int getAliveCount(){
		return alive.size();
	}
	
	virtual int getDeadCount(){
		return dead.size();
	}
	
	virtual int getTotalCount(){
		return particles.size();
	}
	
	virtual vector<T*>& getParticles(){
		return particles;
	}
	
	virtual vector<T*>& getAlive(){
		return alive;
	}
	
	virtual vector<T*>& getDead(){
		return dead;
	}
	
	Particle* popDeadParticle(){
		if(dead.size() == 0) return NULL;
		Particle* particle = dead.back();
		dead.pop_back();
		return particle;
	}
	
	vector<T*> hitTest(vector<T*>& _particles, float x, float y){
		
		vector<T*> hit;
		for (vector<T*>::iterator it = _particles.begin(); it != _particles.end(); ++it) {
			Particle& particle = **it;
			float dx = x - particle.p.x;
			float dy = y - particle.p.y;
			float d2 = dx * dx + dy * dy;
			float r2 = particle.radius * particle.radius;
			if (r2 > d2) { // if distance between particle and x,y smaller then radius
				hit.push_back(&particle);
			}
		}
		
		return hit;
	}
	
protected:

	vector<T*> particles;
	vector<T*> alive;
	vector<T*> dead;
	
	int particleCount;

};