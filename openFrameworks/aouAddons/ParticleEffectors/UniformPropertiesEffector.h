#pragma once
/*
 *  UniformPropertiesEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"

// on start up enabled should be set to true after loading the gui
// enabled will be set to false on frame 2

class UniformPropertiesEffector : public ParticleEffector {
	
public:
	
	float mass;
	float damping;
	float radius;
	ofColor color;
	
	UniformPropertiesEffector(){
		name = "UniformPropertiesEffector";
		mass = 1;
		damping = 0.8;
		radius = 1;
		color.r = 1;
		color.g = 1;
		color.b = 1;
		color.a = 1;
	}
	
	void setupGUI(){
		//gui.page(1).addPageShortcut(gui.addPage(name));
		gui.addToggle("enabled", enabled);
		gui.addSlider("mass", mass, 0.5, 2.5);
		gui.addSlider("damping", damping, 0.1, 0.99);
		gui.addSlider("radius", radius, 1, 64);
		gui.addColorPicker("color", &color.r);
	}
	
	virtual void applyTo(Particle* particle){
		particle->setMass(mass);
		particle->setDamping(damping);
		particle->color = color;
		particle->radius = radius;
	}
	
	void update(){
		/* TODO: automatic switching off of enabled
		if(ofGetFrameNum() == 10){
			enabled = false;
		}
		*/
	}
	
};