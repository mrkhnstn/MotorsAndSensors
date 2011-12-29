#pragma once
/*
 *  RectLiveAreaEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"
#include "Boundary2f.h"


/**
- kills all particles outside a defined rectangle
- only checks whether particle centre is contained within rectangle 
*/
class RectLiveAreaEffector : public ParticleEffector {

public:
	
	Boundary2f boundary;
	ofColor debugColor;
	
	RectLiveAreaEffector(){
		name = "RectLiveAreaEffector";
		boundary.set(0,1024,0,768);
	}
	
	void applyTo(Particle* particle){
		if(!boundary.contains(particle->p.x, particle->p.y)){
			particle->setState(DEAD);
		}
	}
	
	void setupGUI(){
		gui.addSlider("left", boundary.min.x, -ofGetWidth(), 2 * ofGetWidth());
		gui.addSlider("right", boundary.max.x, -ofGetWidth(), 2 * ofGetWidth());
		gui.addSlider("top", boundary.min.y, -ofGetHeight(), 2 * ofGetHeight());
		gui.addSlider("bottom", boundary.max.y, -ofGetHeight(), 2 * ofGetHeight());
		gui.addColorPicker("debug colour", &debugColor.r, false, true);
	}
	
	void draw(){
		ofPushStyle();
		ofSetColor(debugColor.r, debugColor.g, debugColor.b);
		ofDrawBitmapString(name, boundary.min.x, boundary.min.y);
		ofSetLineWidth(1);
		ofNoFill();
		boundary.draw();
		ofPopStyle();
	}

};

// RadiusRectLiveAreaEffector /////////////////////////////////////////////////////////////////

/**
 same as RectLiveAreaEffector but takes radius of particle into account
 */
class RadiusRectLiveAreaEffector : public RectLiveAreaEffector {

public:
	
	void applyTo(Particle* particle){
		if(particle->p.x + particle->radius < boundary.min.x || particle->p.x - particle->radius > boundary.max.x 
		   || particle->p.y + particle->radius < boundary.min.y || particle->p.y - particle->radius > boundary.max.y){
			particle->setState(DEAD);
		}
	}
	
};