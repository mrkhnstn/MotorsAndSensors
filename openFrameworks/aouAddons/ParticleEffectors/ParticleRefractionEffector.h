/*
 *  ParticleRefractionEffector.h
 *  Chris
 *
 *  Created by Chris Mullany on 09/02/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */

#pragma once

#include "ParticleEffector.h"
#include "ParticleBinPool.h"
#include "RadialConstForceField.h"

/**
 Changes the vector of particles when they pass through specified refrator particles.
 !IMPORTANT: particles must be set to damping=1 with a force set only at birth for this effector to function correctly
 */

class ParticleRefractionEffector : public ParticleEffector {
	
public:
	
	ParticlePool* pool;
	RadialConstForceField* exitForceField;
	
	// effective radius is calculated out of radius of refractor particle multiplied by radiusScale
	float radiusScale;
	
	// strenght of refraction
	float strength;
	
	int mode;
	
	ParticleRefractionEffector(){
		pool = NULL;
		radiusScale = 2;
		strength = 0.8;
		mode = 0;
	}
	
	void setupGUI(){
		gui.addSlider("mode", mode, 0, 2);
		gui.addSlider("radius scale", radiusScale, 0, 10);
		gui.addSlider("strength", strength, 0, 2);
	}
	
	/**
	 override this function to specify what particles to use as gravity particles
	 */
	virtual vector<Particle*>& getRefractorParticles(){
		return pool->getAlive();
	}
	
	/*
	 void apply(ParticlePool& pool){
	 
	 if (typeid(pool) != typeid(ParticleBinPool)) return;
	 
	 ParticleBinPool& binPool = (ParticleBinPool&) pool;
	 vector<Particle*>& tRefractorParticles = getRefractorParticles();
	 
	 for(vector<Particle*>::iterator it = tRefractorParticles.begin(); it != tRefractorParticles.end(); ++it){
	 Particle& refractorParticle = **it;
	 
	 vector<Particle*> neighborParticles = binPool.getNeighbors(refractorParticle,refractorParticle.radius);
	 
	 for(vector<Particle*>::iterator it2 = neighborParticles.begin(); it2 != neighborParticles.end(); ++it2){
	 Particle& p2 = (**it2);
	 
	 if (!p2.property.solar.isInUser) {
	 p2.property.solar.isInUser = true;
	 ofxVec2f nDirection = p2.p - p2.op;
	 nDirection.normalize();
	 
	 ofxVec2f nPerpDireaction;
	 nPerpDireaction.set(nDirection.y, -nDirection.x);
	 nPerpDireaction *= 0.5;
	 
	 p2.force(nPerpDireaction);
	 }
	 
	 p2.color.r = 1;
	 p2.color.g = 1;
	 p2.color.b = 1;
	 p2.color.a = 1;
	 
	 
	 }
	 }
	 }
	 */
	
	void apply(set<Particle*>& particles){
		if (pool == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	void apply(vector<Particle*>& particles){
		if (pool == NULL) return; //< prevent usage if no pool is set
		ParticleEffector::apply(particles);
	}
	
	void applyTo(Particle* particle){
		
		vector<Particle*>& tRefractorParticles = getRefractorParticles();
		bool isHit = false;
		
		// current normalized direction
		ofxVec2f nDirection = particle->p - particle->op;
		nDirection.normalize();
		// Final force to apply
		ofxVec2f force;
		// Target point to direct the particle towards
		ofxVec2f targetPoint;
		
		
		for(vector<Particle*>::iterator it = tRefractorParticles.begin(); it != tRefractorParticles.end(); ++it){
			
			Particle& refractorParticle = **it;
			
			// Refractor particle effective radius
			float r = refractorParticle.radius * radiusScale;
			float r2 = r * r;
			
			// Distance to current particle
			ofxVec2f d = refractorParticle.p - particle->p;
			float d2 = d.squareLength();
			
			// Check to see if particle is in refractor particle
			if(d2 < r2){
				isHit = true;
				
				if (!particle->property.solar.isInUser) {
					
					// Particle is in the refractor particle for the first time, change the directional vector
					// update isInUser: we only want to apply force once as damping=1
					particle->property.solar.isInUser = true;
					// Stop the particle
					particle->jump(particle->p);
					
					// Start to calculate the force to apply
					force = refractorParticle.p - exitForceField->centre;
					force.normalize();
					
					switch (mode) {
						case 0:
							// Mode 0 aligns all particle vectors with the vector joining the ambient emitter to the refractor centre
							force *= exitForceField->strength;
							particle->force(force);
							break;
						case 1:
							// Mode 1 refracts particles to a point on the opposite side of the refractor
							force *= (refractorParticle.radius * strength);
							// Calculate a target point for the refraction
							targetPoint = refractorParticle.p + force;
							// Calculate a vector from the particle to the target point
							force = targetPoint - particle->p;
							force.normalize();
							force *= exitForceField->strength * 2;
							// Apply the new force
							particle->force(force);
							break;
						case 2:
							break;
						default:
							break;
					}
				}
				
			}
			
		}
		
		
		// Check if particle is inside any of the refractors
		if (!isHit) {
			if (particle->property.solar.isInUser) {
				
				// Particle is leaving the refractor particle
				// Revert to previous directional vector and reset isInUser flag
				particle->property.solar.isInUser = false;
				particle->jump(particle->p);
				particle->force(exitForceField->getForce(particle->p) * 2);
				
			}
		}
		
	}
};