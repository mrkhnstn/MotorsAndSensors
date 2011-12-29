#pragma once
/*
 *  ParticleRepulsionEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 02/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

/**
 particle to particle repulsion based on each particles radius
 brute force method!!!
 */
class ParticleRepulsionEffector : public ParticleEffector{

public:
	
	float strength;
	float radiusScale;
	
	ParticleRepulsionEffector(){
		strength = 0.1;
		radiusScale = 1;
	}
	
	void setupGUI(){
		gui.addSlider("strength",strength,0,1);
		gui.addSlider("radius scale", radiusScale, 0.1, 10);
	}
	
	virtual void apply(set<Particle*>& particles){
		

		
		// particle to particle iteration
		set<Particle*>::iterator it;
		set<Particle*>::iterator it2;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* p1 = *it;
			for(it2 = particles.begin(); it2 != particles.end(); ++it2){
				Particle* p2 = *it2;
				if (p1 != p2) {
					
					float r = p1->radius * radiusScale + p2->radius * radiusScale; //< sum of radiuses
					float r2 = r * r;
					
					
					ofxVec2f	force; //< 
					force = p1->p - p2->p;
					
					float d2 = force.squareLength();
					
					if(d2 < r2){ // if distance larger than radius then have no effect
						float relativeStrength = (r2 - d2) / r2;			
						force *= relativeStrength * strength;
					} else {
						force.set(0,0);
					}
					
					p1->force(force);
					
					
				}
			}
		}			
	}
	
	virtual void apply(vector<Particle*>& particles){
		
		// particle to particle iteration
		vector<Particle*>::iterator it;
		vector<Particle*>::iterator it2;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* p1 = *it;
			for(it2 = particles.begin(); it2 != particles.end(); ++it2){
				Particle* p2 = *it2;
				if (p1 != p2) {
					
					float r = p1->radius * radiusScale + p2->radius * radiusScale; //< sum of radiuses
					float r2 = r * r;
					
					
					ofxVec2f	force; //< 
					force = p1->p - p2->p;
					
					float d2 = force.squareLength();
					
					if(d2 < r2){ // if distance larger than radius then have no effect
						float relativeStrength = (r2 - d2) / r2;			
						force *= relativeStrength * strength;
					} else {
						force.set(0,0);
					}
					
					p1->force(force);
					
					
				}
			}
		}			
	}
	
	// disable
	void applyTo(Particle* particle){}
	
};