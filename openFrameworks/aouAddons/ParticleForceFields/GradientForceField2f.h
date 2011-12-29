#pragma once
/*
 *  GradientForceField2f.h
 *  atmosphere
 *
 *  Created by Robin Beitra (robin@beitra.net) on 17/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ForceField2f.h"
#include "ofxSimpleGuiToo.h"

class GradientForceField2f : public ForceField2f {
	
public:
	
	
	
	struct GUIControl{
		void setupGUI(string name){
			
			float forceMin = -1;
			float forceMax = 1;
			
			gui.addPage(name);
			

			
			gui.addSlider("a.x", a.x, 0, 1920);
			gui.addSlider("a.y", a.y, 0, 1200);
			
			gui.addSlider("b.x", b.x, 0, 1920);
			gui.addSlider("b.y", b.y, 0, 1200);
			
			gui.addSlider("fa.x", fa.x, forceMin, forceMax);
			gui.addSlider("fa.y", fa.y, forceMin, forceMax);

			gui.addSlider("fb.x", fb.x, forceMin, forceMax);
			gui.addSlider("fb.y", fb.y, forceMin, forceMax);

			gui.addSlider("strength", s, 0, 1);
			
			gui.addTitle("debug");
			gui.addToggle("draw debug", drawDebug);
			gui.addColorPicker("debug color", &debugColor.r);
		}
		ofxVec2f a, b, fa, fb;
		float s;
		bool drawDebug;
		ofColor debugColor;
	} guiControl;
	
	float strength;
	
	ofxVec2f pointMul;//used to determine the axis of the gradient
	float offset;
	
	ofxVec2f force;
	ofxVec2f forceB;
	
	GradientForceField2f(){
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		float p = pos_.x*pointMul.x + pos_.y*pointMul.y + offset;
		p = p<0.0f?0.0f:p;
		p = p>1.0f?1.0f:p;
		return strength*(force + forceB*p);
	}
	
	void setup(ofxVec2f _pointA, ofxVec2f _pointB, ofxVec2f _forceA, ofxVec2f _forceB){
		ofxVec2f delta = _pointB - _pointA;
		float d2 = delta.lengthSquared();
		
		if(d2 == 0){
			d2 = 1;
		}
		
		pointMul = delta/d2;
		offset = -(_pointA.x*pointMul.x + _pointA.y*pointMul.y);
		
		force = _forceA;
		forceB = _forceB - _forceA;
		
	}
	
	void draw(){
		if(guiControl.drawDebug){
			ofPushStyle();
			glColor3f(guiControl.debugColor.r, guiControl.debugColor.g, guiControl.debugColor.b);
			float radius = 10;
			ofRect(guiControl.a.x - radius, guiControl.a.y - radius, radius * 2, radius * 2);
			ofCircle(guiControl.b.x, guiControl.b.y, radius);
			ofLine(guiControl.a.x, guiControl.a.y,guiControl.b.x, guiControl.b.y);
			ofPopStyle();
		}
	}
	
	void updateFromGui(){
		setup(guiControl.a, guiControl.b, guiControl.fa, guiControl.fb);
		strength = guiControl.s;
	}
	
};