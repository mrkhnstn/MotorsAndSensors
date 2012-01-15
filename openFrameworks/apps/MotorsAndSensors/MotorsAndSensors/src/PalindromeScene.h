#pragma once
/*
 *  PalindromeScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 31/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this scene moves all motors in a palindrome fashion (back and forth) using a global speed
 */
class PalindromeScene : public Scene {
public:
	
	float angle;
	float angleSpeed;
	float dir;		// if 1 then increment otherwise decrement angle
	int counter;	// increments on each directional change. if larger then 2 then stop()
	
	void setup(){
		Scene::setup();
		name = "PalindromeScene";
		
		angle = 0;
		angleSpeed = 0;
		dir = 1;
		
		counter = 0;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		
		gui.addSlider("angle", angle, 0, 180);
		gui.addSlider("angleSpeed", angleSpeed, 0, 1);
	}
	
	void update(){
		Scene::update();
		
		angle += dir * angleSpeed;
		if (angle > 180) {
			angle = 180 - (angle - 180);
			dir = -1;
			counter++;
		} else if (angle < 0) {
			angle *= -1;
			dir = 1;
			counter++;
		}
		
		for (int i=0; i<getMotorCount(); i++) {
			setMotorAngle(i, angle);
		}
		
		if(counter > 2)
			stop();
	}
	
	void start(){
		Scene::start();
 		counter = 0;
	}
	
	void stop(){
		Scene::stop();
	}
	
};
