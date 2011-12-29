#pragma once

/*
 *  WrapEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"
#include "Boundary2f.h"


/**
 - positions  of particles outside a defined rectangle are wrapped around the boundaries of the rectangle
 - this effector needs to go at the end of the pipe line
 */

class WrapEffector : public ParticleEffector {

public:
	
	Boundary2f boundary;
	float shiftH;
	float shiftV;
	
	WrapEffector(){
		boundary.set(0,1024,0,768);
	}
	
	void update(){
		shiftH = boundary.max.x - boundary.min.x;
		shiftV = boundary.max.y - boundary.min.y;
	}
	
	void applyTo(Particle* particle){
		
		float tempShiftH = shiftH;
		float tempShiftV = shiftV;
		
		if(particle->p.x < boundary.min.x){
			tempShiftH = shiftH;
		} else if(particle->p.x > boundary.max.x){
			tempShiftH = -shiftH;
		} else {
			tempShiftH = 0;
		}
		
		if(particle->p.y < boundary.min.y){
			tempShiftV = shiftV;
		} else if(particle->p.y > boundary.max.y){
			tempShiftV = -shiftV;
		} else {
			tempShiftV = 0;
		}
		
		particle->shift(tempShiftH, tempShiftV);
		
	}
	
};