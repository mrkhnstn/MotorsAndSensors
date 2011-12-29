#pragma once

/*
 *  MappedPropertiesEffector.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleEffector.h"
#include "Boundary1f.h"

// on start up enabled should be set to true after loading the gui
// enabled will be set to false on frame 2

#define MappedPropertiesEffector_NO_APPLY		0
#define MappedPropertiesEffector_APPLY_DIRECT	1
#define MappedPropertiesEffector_APPLY_TO_BASE	2

class MappedPropertiesEffector : public ParticleEffector {
	
public:
	
	Boundary1f mass;
	Boundary1f damping;
	Boundary1f radius;
	Boundary1f maxLife;
	ofColor color1;
	ofColor color2;
	
	// apply states: 0 = don't apply, 1 = apply directly to particle property, 2 = apply to base property
	int applyMass;
	int applyDamping;
	int applyRadius;
	int applyColor;
	int applyMaxLife;
	
	MappedPropertiesEffector(){
		
		name = "MappedPropertiesEffector";
		
		mass.set(1,1);
		damping.set(0.8,0.9);
		radius.set(1,20);
		
		color1.r = 1;
		color1.g = 1;
		color1.b = 1;
		color1.a = 1;
		
		color2.r = 1;
		color2.g = 1;
		color2.b = 1;
		color2.a = 1;
		
		applyMass = MappedPropertiesEffector_APPLY_DIRECT;
		applyDamping = MappedPropertiesEffector_APPLY_DIRECT;
		applyRadius = MappedPropertiesEffector_APPLY_DIRECT;
		applyColor = MappedPropertiesEffector_APPLY_DIRECT;
		applyMaxLife = MappedPropertiesEffector_APPLY_DIRECT;
	}
	
	virtual void setupGUI(){
		string applyNames[3] = {"disabled", "direct", "to base"};
		
		gui.addToggle("enabled", enabled);
		gui.addComboBox("apply mass", applyMass, 3, applyNames);
		mass.addToGUI("mass", 0.5, 2.5);
		gui.addComboBox("apply damping", applyDamping, 3, applyNames);
		damping.addToGUI("damping", 0.5, 2.5);
		gui.addComboBox("apply radius", applyRadius, 3, applyNames);
		radius.addToGUI("radius", 1, 64);
		gui.addComboBox("apply color", applyColor, 3, applyNames);
		gui.addColorPicker("color 1", &color1.r);
		gui.addColorPicker("color 2", &color2.r);
		gui.addComboBox("apply max life", applyMaxLife, 3, applyNames);
		maxLife.addToGUI("max life", 0, 30000);
	}
	
	/*
	virtual void apply(vector<Particle*>& particles){
		vector<Particle*>::iterator it;
		for(it = particles.begin(); it != particles.end(); ++it){
			Particle* particle = *it;
			applyTo(particle);
		}			
	}
	*/
	
	void applyTo(Particle* particle){
	}
	
	virtual void apply(vector<Particle*>& particles){
		
		// mass //////////////////
		switch (applyMass) {
			case MappedPropertiesEffector_APPLY_DIRECT:
				applyMassDirect(particles);
				break;
			case MappedPropertiesEffector_APPLY_TO_BASE:
				applyMassDirect(particles);
			default:
				break;
		}
		
		// damping //////////////////
		switch (applyDamping) {
			case MappedPropertiesEffector_APPLY_DIRECT:
				applyDampingDirect(particles);
				break;
			case MappedPropertiesEffector_APPLY_TO_BASE:
				applyDampingDirect(particles);
			default:
				break;
		}
		
		// radius //////////////////
		switch (applyRadius) {
			case MappedPropertiesEffector_APPLY_DIRECT:
				applyRadiusDirect(particles);
				break;
			case MappedPropertiesEffector_APPLY_TO_BASE:
				applyRadiusBase(particles);
			default:
				break;
		}
		
		// colour ///////////////////
		switch (applyColor) {
			case MappedPropertiesEffector_APPLY_DIRECT:
				applyColourDirect(particles);
				break;
			case MappedPropertiesEffector_APPLY_TO_BASE:
				applyColourBase(particles);
			default:
				break;
		}
		
		// max life //////////////////
		switch (applyMaxLife) {
			case MappedPropertiesEffector_APPLY_DIRECT:
			case MappedPropertiesEffector_APPLY_TO_BASE:
				applyMaxLifeDirect(particles);
				break;
			default:
				break;
		}
	}

	void applyMassDirect(vector<Particle*>& particles){
		for(vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it){
			applyMassDirect(*it);
		}	
	}
	
	void applyMassDirect(Particle* particle){
		particle->setMass(mass.getMapped(particle->nProperty, 0, 1));
	}
	
	void applyDampingDirect(vector<Particle*>& particles){
		for(vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it){
			applyMassDirect(*it);
		}	
	}
	
	void applyDampingDirect(Particle* particle){
		particle->setDamping(damping.getMapped(particle->nProperty, 0, 1));
	}
	
	void applyRadiusDirect(vector<Particle*>& particles){
		for(vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it){
			applyRadiusDirect(*it);
		}	
	}
	
	void applyRadiusDirect(Particle* particle){
		particle->radius = radius.getMapped(particle->nProperty, 0, 1);	
	}

	void applyRadiusBase(vector<Particle*>& particles){
		for(vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it){
			applyRadiusBase(*it);
		}	
	}	
	
	void applyRadiusBase(Particle* particle){
		particle->property.base.radius = radius.getMapped(particle->nProperty, 0, 1);
	}
	
	void applyColourDirect(vector<Particle*>& particles){
		for(vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it){
			applyColourDirect(*it);
		}	
	}	
	
	void applyColourDirect(Particle* particle){
		particle->color.r = ofMap(particle->nProperty, 0, 1, color1.r, color2.r);
		particle->color.g = ofMap(particle->nProperty, 0, 1, color1.g, color2.g);
		particle->color.b = ofMap(particle->nProperty, 0, 1, color1.b, color2.b);
		particle->color.a = ofMap(particle->nProperty, 0, 1, color1.a, color2.a);		
	}
	
	void applyColourBase(vector<Particle*>& particles){
		for(vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it){
			applyColourBase(*it);
		}	
	}	
	
	void applyColourBase(Particle* particle){
		particle->property.base.r = ofMap(particle->nProperty, 0, 1, color1.r, color2.r);
		particle->property.base.g = ofMap(particle->nProperty, 0, 1, color1.g, color2.g);
		particle->property.base.b = ofMap(particle->nProperty, 0, 1, color1.b, color2.b);
		particle->property.base.a = ofMap(particle->nProperty, 0, 1, color1.a, color2.a);		
	}
	
	void applyMaxLifeDirect(vector<Particle*>& particles){
		for(vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it){
			applyMaxLifeDirect(*it);
		}	
	}	
	
	void applyMaxLifeDirect(Particle* particle){
		particle->property.base.maxLife = maxLife.getMapped(particle->nProperty, 0, 1);
	}
	
	void update(){
		/* TODO: automatic switching off of enabled
		/*
		if(ofGetFrameNum() == 10){
			enabled = false;
		}
		*/
		
	}
	
};