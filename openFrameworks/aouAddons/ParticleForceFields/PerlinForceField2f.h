#pragma once
/*
 *  PerlinForceField2f.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 05/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForceField2f.h"
#include "MSAPerlin.h"
#include "ofxSimpleGuiToo.h"

class PerlinForceField2f : public ForceField2f {
	
public:
	
	struct PerlinParameters {
		
		int octaves;
		float freq;
		float amp;
		int seed;
		float z;
		float zSpeed;
		MSA::Perlin perlin;
		
		PerlinParameters(){
			octaves = 4;
			freq = 2;
			amp = 0.5;
			seed = 1;
			z = 0;
			zSpeed = 0.1;
			perlin.setup(octaves, freq, amp, seed, true);
		}

		float get(float x, float y){
			return perlin.get(x,y,z);
		}

		/*
		float get(float x, float y, float z){
			return perlin.get(x,y,z);
		}
		*/
		
		void update(){
			z += zSpeed;
		}
		
		void updatePerlin(){
			perlin.setup(octaves, freq, amp, seed, false);
		}
		
		void setupGUI(string name_ = ""){
			gui.addSlider(name_+" octaves", octaves, 1, 10);
			gui.addSlider(name_+" frequency", freq, 0, 0.1);
			gui.addSlider(name_+" amplitude", amp, 0, 10);
			gui.addSlider(name_+" seed", seed, 1, 256);
			gui.addSlider(name_+" z", z, 0, 1024);
			gui.addSlider(name_+" zSpeed", zSpeed, 0, 1);
		}
		
	} perlinX, perlinY;
	
	
	PerlinForceField2f(){
		updateOnce = false;
		alwaysUpdate = false;
		strength = 1;
		scale.set(1, 1);
	}
	
	virtual void setupGUI(string name_ = "PerlinForceEffector2f"){
		gui.addSlider("strength", strength, 0, 1);
		perlinX.setupGUI("x");
		gui.addSlider("x scale", scale.x, 0, 8);
		perlinY.setupGUI("y");
		gui.addSlider("y scale", scale.y, 0, 8);
		gui.addButton("update once", updateOnce);
		gui.addToggle("always update", alwaysUpdate);
	}
	
	void update(ofEventArgs& e){
		update();
	}
	
	virtual void update(){
		if(alwaysUpdate || updateOnce){
			perlinX.updatePerlin();
			perlinY.updatePerlin();
			updateOnce = false;
		}
		
		perlinX.update();
		perlinY.update();
	}
	
	void enableUpdate(){
		ofAddListener(ofEvents.update, this, &PerlinForceField2f::update);
	}
	
	void disableUpdate(){
		ofRemoveListener(ofEvents.update, this, &PerlinForceField2f::update);
	}
	
	virtual ofxVec2f getForce(ofxVec2f& pos_){
		ofxVec2f pos = pos_*scale;
		ofxVec2f force(perlinX.get(pos.x, pos.y), perlinY.get(pos.x, pos.y));
		return force * strength;
	}
	
	//set z for both perlin x and perlin y
	virtual void setZ(float z_){
		perlinX.z = z_;
		perlinY.z = z_;
	}
	
	ofxVec2f scale;
	
	bool updateOnce;
	bool alwaysUpdate;
};