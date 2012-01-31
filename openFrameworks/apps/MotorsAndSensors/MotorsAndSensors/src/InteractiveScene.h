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
#include "MotorCtrl.h"

/*
 this scene opens all motors where a user proximity is detected
 */

#define STATIC_FRONT 0
#define STATIC_OPEN 1
#define STATIC_BACK 2

class InteractiveScene : public Scene {
public:
	
	//float closeDelayTime;
	
	//bool doAdjacent;
	float duration;

	int mode; //0: static front, 1: static open, 2: static close

	static int uniqueId;
	
	InteractiveScene(){
		
		mode = uniqueId++;
		name = "InteractiveScene";
		switch (mode) {
			case STATIC_FRONT:
				name = "Static_Black";
				break;
			case STATIC_BACK:
				name = "Static_White";
				break;
			case STATIC_OPEN:
				name = "Static_Open";
				break;
			default:
				break;
		}
	}
	
	void setup(){
		Scene::setup();
		duration = 30;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("duration", duration, 10, 300);
	}
	
	void update(){
		if(!enabled) return;
		Scene::update();

		switch (mode) {
			case STATIC_FRONT:
				name = "Static_Black";
				for (int i=0; i<getMotorCount(); i++){
					if(userInFrontOfMotor(i))
					{
						getMotor(i).doLimitSpeed = false;
						panelOpen(i);
					} else {
						getMotor(i).doLimitSpeed = true;
						panelFront(i);
					}
				}
				break;
			case STATIC_BACK:
				name = "Static_White";
				for (int i=0; i<getMotorCount(); i++){
					if(userInFrontOfMotor(i))
					{
						getMotor(i).doLimitSpeed = false;
						panelOpen(i);
					} else {
						getMotor(i).doLimitSpeed = true;
						panelBack(i);
					}
				}
				break;
			case STATIC_OPEN:
				name = "Static_Open";
				for (int i=0; i<getMotorCount(); i++){
					if(userInFrontOfMotor(i))
					{
						getMotor(i).doLimitSpeed = false;
						panelBack(i);
					} else {
						getMotor(i).doLimitSpeed = true;
						panelOpen(i);
					}
				}
				break;
			default:
				break;
		}
		
		
		// reset all to black
		if (elapsedTime > duration) {
			for (int i=0; i<getMotorCount(); i++){
				if(userInFrontOfMotor(i))
				{
					getMotor(i).doLimitSpeed = false;
					panelBack(i);
				} else {
					getMotor(i).doLimitSpeed = true;
					panelOpen(i);
				}
			}		}

		// stop this scene 3 seconds after duration to allow for reset
		if (elapsedTime > duration + 1.5) {
			stop();
		}
	}

	void start(){
		Scene::start();
	}
	
	void stop(){
		Scene::stop();
		for (int i=0; i<getMotorCount(); i++){
			getMotor(i).doLimitSpeed = true;
		}
	}
	
};
