#pragma once
/*
 *  ParticleDrawer.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 15/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"
#include "ParticlePool.h"

/*
 base drawer class
 */
class ParticleDrawer{
	
public:
	
	bool doDebugDraw; //< if true then also draw particle outlines in white
	
	ParticleDrawer(){
		doDebugDraw = false;
	}
	
	virtual void draw(ParticlePool& pool){
		draw(pool.getAlive());
		if (doDebugDraw) {
			debugDraw(pool.getAlive());
		}
	};
	
	virtual void draw(set<Particle*>& particles){
		set<Particle*>::iterator it;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* particle = *it;
			particle->draw();
		}
	}
	
	virtual void draw(vector<Particle*>& particles){
		vector<Particle*>::iterator it;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* particle = *it;
			particle->draw();
		}
	}
	
	virtual void debugDraw(vector<Particle*>& particles){
		ofPushStyle();
		ofDisableAlphaBlending();
		ofSetColor(255, 255, 255);
		ofSetLineWidth(1);
		ofNoFill();
		vector<Particle*>::iterator it;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* particle = *it;
			ofCircle(particle->p.x, particle->p.y, particle->radius);
		}
		ofPopStyle();
	}
	
};
