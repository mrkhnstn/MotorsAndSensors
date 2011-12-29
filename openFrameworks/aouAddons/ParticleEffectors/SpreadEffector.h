#pragma once
/*
 *  SpreadEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 25/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"
#include "SpreadForceField2f.h"

class SpreadEffector: public ParticleEffector{

public:
	
	SpreadForceField2f spreadForceField;
	bool isSetup;
	
	SpreadEffector(){
		isSetup = false;
	}
	
	void setup(int width, int height, float bucketSize, bool interpolate, float offsetX = 0, float offsetY = 0){
		
		if(isSetup) {
			ofLog(OF_LOG_ERROR, "SpreadEffector already setup!");
			return;
		}
		
		spreadForceField.setup(width, height, bucketSize, offsetX, offsetY);
		spreadForceField.interpolate = true;
		spreadForceField.textureField->interpolate = true;
		isSetup = true;
	}

	virtual void apply(set<Particle*>& particles){
		if(spreadForceField.strength == 0 || !isSetup) return;
		spreadForceField.preprocess();
		ParticleEffector::apply(particles);
		spreadForceField.process();
	}
	
	virtual void apply(vector<Particle*>& particles){
		if(spreadForceField.strength == 0 || !isSetup) return;
		spreadForceField.preprocess();
		ParticleEffector::apply(particles);
		spreadForceField.process();		
	}
	
	void applyTo(Particle* particle){
		spreadForceField.accumulate(particle->p, particle->radius*4);
	}
	
};