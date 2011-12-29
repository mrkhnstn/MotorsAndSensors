#pragma once
/*
 *  Particle.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 06/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "VerletPoint.h"
#include "ParticleProperty.h"

#define DEAD 0
#define ALIVE 1

class Particle : public VerletPoint {
	
public:
	
	ofColor color;
	float radius;
	float rotation;
	int	  frame;
	float nProperty; //< normalized random factor used for setting other parameters mapped with in a range
	int state;
	int life;
	
	ParticleProperty property;
	
	Particle();
	
	virtual void setup();
	
	virtual void update();
	
	virtual void draw();
	
	void setState(int state_);
	
	int getState();
		
	bool isDead();
	
	bool isAlive();
	
};

