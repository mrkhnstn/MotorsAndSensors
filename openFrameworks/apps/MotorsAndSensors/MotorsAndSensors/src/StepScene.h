#pragma once
/*
 *  StepScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 31/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this scene moves all motors in regular intervals by 90 degrees
 */
class StepScene : public Scene {
public:
	
	int		state;		// 0: side 1, 1: open, 2: side 2
	float	interval;	// time in seconds to change state
	float	changeTime; // if ofElapsedTimef() > changeTime then change state
	int		counter;	// increments on directional change, if larger then 2 then stop();
	
	void setup(){
		Scene::setup();
		name = "StepScene";

		state = 0;
		interval = 2;
		changeTime = ofGetElapsedTimef() + interval;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		
		gui.addSlider("interval", interval, 0.5, 3);
		gui.addSlider("state", state, 0, 2);
	}
	
	void start(){
		Scene::start();
		counter = 0;
	}
	
	void update(){
		Scene::update();
		
		if (ofGetElapsedTimef() > changeTime) {
			
			// change state
			state++;
			if (state > 2) {
				state = 0;
			}
			
			counter++; 
			
			// set next change time
			changeTime = ofGetElapsedTimef() + interval;
			
		}
		
		switch (state) {
			case 0:
				for (int i=0; i<getMotorCount(); i++)
					panelFront(i);
				break;
			case 1:
				for (int i=0; i<getMotorCount(); i++)
					panelOpen(i);
				break;
			case 2:
				for (int i=0; i<getMotorCount(); i++)
					panelBack(i);
				break;
			default:
				break;
		}
		
		if(counter > 2)
			stop();

	}
	
	
};


