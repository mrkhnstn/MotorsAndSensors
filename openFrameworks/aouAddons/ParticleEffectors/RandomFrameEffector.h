#pragma once
/*
 *  RandomFrameEffector.h
 *  TestBed_Mark
 *
 *  Created by TAQA_CH_2 on 09/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

class RandomFrameEffector : public ParticleEffector {
	
public:
	
	int minFrame;
	int maxFrame;
	
	RandomFrameEffector(){
		minFrame = 0;
		maxFrame = 0;
	}
	
	void applyTo(Particle* particle){
			particle->frame = ofRandom(minFrame, maxFrame);
	}
	
};
