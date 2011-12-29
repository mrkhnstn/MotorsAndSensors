#pragma once
/*
 *  HorizontalBoundaryEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 06/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

/**
 - applies a force to particles who are outside of two horizontal boundaries
 - takes into account the radius of a particle
 */
class HorizontalBoundaryEffector : public ParticleEffector {

public:
	
	float strength;
	float minY;
	float maxY;
	
	HorizontalBoundaryEffector(){
		minY = 0;
		maxY = ofGetWidth();
		strength = 0.1;
	}
	
	void setupGUI(){
		gui.addSlider("min y",minY,-1200,2400);
		gui.addSlider("max y",minY,-1200,2400);
		gui.addSlider("strength",strength,0,0.1);
	}
	
	void applyTo(Particle* particle){
		if ((particle->p.y - particle->radius) < minY) {
			particle->force(0,strength);
		} else if ((particle->p.y + particle->radius) > maxY) {
			particle->force(0,-strength);
		}
	}
	
};