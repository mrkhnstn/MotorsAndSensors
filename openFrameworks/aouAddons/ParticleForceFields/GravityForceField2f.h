#pragma once
/*
 *  ConstForceField2f.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 03/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"
#include "ofxBaseObject.h"

class GravityForceField2f : public ForceField2f, public ofxBaseObject {
	
public:
	
	ofxVec2f center;
	float radius;
	float strength;
	
	GravityForceField2f(){
		radius = 1;
		strength = 0;
	}
	
	void setupGUI(){
		gui.addSlider("center x", center.x, -2048, 2048);
		gui.addSlider("center y", center.y, -2048, 2048);
		gui.addSlider("radius", radius, 1, 2048);
		gui.addSlider("strength", strength, -0.01, 0.01);
		gui.addToggle("debug draw", debugDraw);
	}
	
	virtual void draw(){
		if(!debugDraw) return;
		ofPushStyle();
		ofSetColor(255, 0, 0);
		ofNoFill();
		ofCircle(center.x, center.y, radius);
		ofPopStyle();
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		ofxVec2f	force;
		force = center - pos_;
		float d2 = force.squareLength();
		float r2 = radius * radius;
		if(d2 < r2){ // if distance larger than radius then have no effect
			float relativeStrength = (r2 - d2) / r2;			
			force *= relativeStrength * strength;
		} else {
			force.set(0,0);
		}

		return force;
	}
	
protected:
	
	
	bool		debugDraw;
					  
};