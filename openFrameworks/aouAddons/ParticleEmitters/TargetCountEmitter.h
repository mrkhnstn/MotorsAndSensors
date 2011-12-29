#pragma once
/*
 *  RegulateCountEmitter.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 01/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEmitter.h"

/**
only emits particles if alive count of pool is smaller than target count
 */
class TargetCountEmitter : public ParticleEmitter {

public:
	
	int targetCount; //< desired number of alive particles in pool
	
	TargetCountEmitter(){
		targetCount = 0;
	}
	
	int emit(){
		if(_pool == NULL) return 0; //< do nothing if no pool has been set
		
		_emitted += _emitRate;
		
		if (_emitted < 1) return 0;
		
		vector<T*> emitParticles;
		
		vector<T*>& deadParticles = _pool->getDead();
		
		int aliveCount = _pool->getAlive().size();
		
		for (int i = 0; i < _emitted && deadParticles.size() > 0 && aliveCount < targetCount; i++) {
			emitParticles.push_back(deadParticles.back());
			deadParticles.pop_back();
			aliveCount++;
		}
		
		for (vector<ParticleEffector*>::iterator it = effectors.begin(); it != effectors.end(); ++it) {
			(*it)->apply(emitParticles);
		}
		
		_emitted -= emitParticles.size();
		
		return emitParticles.size();
	}
	
	void setupGUI(){
		gui.addSlider("emit rate", _emitRate, 0, 100);
		gui.addSlider("target count", targetCount, 0, 65536);
	}
};