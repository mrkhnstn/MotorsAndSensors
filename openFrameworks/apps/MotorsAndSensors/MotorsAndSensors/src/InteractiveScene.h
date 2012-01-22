#pragma once
/*
 *  InteractiveScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 22/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this scene opens all motors where a user proximity is detected
 */
class InteractiveScene : public Scene {
public:
	
	float closeDelayTime;
	
	void setup(){
		Scene::setup();
		name = "InteractiveScene";
		closeDelayTime = 3;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("closeDelayTime",closeDelayTime,0,10);
	}
	
	void update(){
		Scene::update();
		for (int i=0; i<getMotorCount(); i++)
			if (userInFrontOfMotor(i) || ofGetElapsedTimef() < getMotor(i).userInproximityOffTime + closeDelayTime) {
				panelOpen(i);
			} else {
				panelFront(i);
			}
	}
	
};
