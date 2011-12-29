#pragma once
/*
 *  ParticleGravityEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 01/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

/**
 attracts / repulses particles to / from a list of particles
 */

class ParticleGravityEffector : public ParticleEffector {

public:
	
	vector<Particle*> gravityParticles; //< particles used as centre of gravitation (just for default usage)
	float radiusScale; //< effective radius is calculated out of radius of gravity particle multiplied by radiusScale
	float strength; //< strenght of attraction / repulsion
	
	ParticleGravityEffector(){
		radiusScale = 2;
		strength = 1;
	}
	
	void setupGUI(){
		gui.addSlider("radius scale", radiusScale, 0, 50);
		gui.addSlider("strength", strength, -0.8, 0.8);
	}
	
	/**
	override this function to specify what particles to use as gravity particles
	*/
	virtual vector<Particle*>& getGravityParticles(){
		return gravityParticles;
	}
	
	void applyTo(Particle* particle){
		
		vector<Particle*>& _gravityParticles = getGravityParticles();
		
		for(vector<Particle*>::iterator it = _gravityParticles.begin(); it != _gravityParticles.end(); ++it){
			Particle& gravityParticle = **it;
			
			float r = gravityParticle.radius * radiusScale;
			float r2 = r * r;
			
			ofxVec2f	force;
			force = gravityParticle.p - particle->p;
			float d2 = force.squareLength();
			
			if(d2 < r2){ // if distance larger than radius then have no effect
				float relativeStrength = (r2 - d2) / r2;			
				force *= relativeStrength * strength;
			} else {
				force.set(0,0);
			}
			
			particle->force(force);
		}	
		
	}
};

// repulsion effector ////////////////////////////////////////////////

/*
class ParticleRepulsionEffector : public ParticleEffector {
	
public:
	
	vector<Particle*> gravityParticles; //< particles used as centre of gravitation (just for default usage)
	float radiusScale; //< effective radius is calculated out of radius of gravity particle multiplied by radiusScale
	float strength; //< strenght of attraction / repulsion
	
	ParticleRepulsionEffector(){
		radiusScale = 2;
		strength = 1;
	}
	
	void setupGUI(){
		gui.addSlider("radius scale", radiusScale, 0, 50);
		gui.addSlider("strength", strength, -0.8, 0.8);
	}
	
	virtual vector<Particle*>& getGravityParticles(){
		return gravityParticles;
	}
	
	void applyTo(Particle* particle){
		
		vector<Particle*>& _gravityParticles = getGravityParticles();
		
		for(vector<Particle*>::iterator it = _gravityParticles.begin(); it != _gravityParticles.end(); ++it){
			Particle& gravityParticle = **it;
			
			float r = gravityParticle.radius * radiusScale + particle->radius * radiusScale;
			float r2 = r * r;
			
			ofxVec2f	force;
			force = gravityParticle.p - particle->p;
			float d2 = force.squareLength();
			
			if(d2 < r2){ // if distance larger than radius then have no effect
				float relativeStrength = (r2 - d2) / r2;			
				force *= relativeStrength * strength;
			} else {
				force.set(0,0);
			}
			
			particle->force(force);
		}	
		
	}
};
*/

/*
class AliveParticleGravityEffector : public ParticleGravityEffector {
	
public:
	
	ParticlePool* pool;
	
	virtual vector<Particle*>& getGravityParticles(){
		return pool->getAlive();
	}
	
};
*/
