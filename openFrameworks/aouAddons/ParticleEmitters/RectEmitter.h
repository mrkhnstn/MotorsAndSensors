#pragma once
/*
 *  RectParticleEmitter.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEmitter.h"
#include "ofxSimpleGuiToo.h"

/**
 effector used by RectEmitter
 */
class RectEmitterEffector : public ParticleEffector {

public:
	
	Boundary2f rect;
	
	virtual void applyTo(Particle* particle){
		ofxVec2f p;
		rect.getRandomPoint(p);
		particle->jump(p);
		particle->setState(ALIVE);
	}
	
	void setupGUI(){
		gui.addSlider("left", rect.min.x, -ofGetWidth(), 2 * ofGetWidth());
		gui.addSlider("right", rect.max.x, -ofGetWidth(), 2 * ofGetWidth());
		gui.addSlider("top", rect.min.y, -ofGetHeight(), 2 * ofGetHeight());
		gui.addSlider("bottom", rect.max.y, -ofGetHeight(), 2 * ofGetHeight());
	}
	
};

////////////////////////////////////////////////////////////////////

class RectEmitter: public ParticleEmitter{

public:
	
	RectEmitterEffector rectEmitterEffector;
	ofColor debugColor;
	
	RectEmitter() : ParticleEmitter() {
		name = "RectEmitter";
		addEffector(rectEmitterEffector);
	}
	
	void setupGUI(){
		gui.addSlider("emit rate", _emitRate, 0, 10);
		rectEmitterEffector.setupGUI();
		gui.addColorPicker("debug colour", &debugColor.r, false, true);
	}

	void draw(){
		ofPushStyle();
		ofSetColor(debugColor.r, debugColor.g, debugColor.b);
		ofDrawBitmapString(name, rectEmitterEffector.rect.min.x, rectEmitterEffector.rect.min.y);
		ofSetLineWidth(1);
		ofNoFill();
		rectEmitterEffector.rect.draw();
		ofPopStyle();
	}
	
	Boundary2f& getRect(){
		return rectEmitterEffector.rect;
	}
};


