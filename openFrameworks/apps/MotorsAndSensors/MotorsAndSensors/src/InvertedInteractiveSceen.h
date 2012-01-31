#pragma once
/*
 *  InvertedInteractiveScene.h
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
class InvertedInteractiveScene : public Scene {
public:
	
	//float closeDelayTime;
	
	bool doAdjacent;
	float duration;
	
	void setup(){
		Scene::setup();
		name = "InvertedInteractiveScene";
		doAdjacent = false;
		//closeDelayTime = 3;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addToggle("doAdjacent", doAdjacent);
		//gui.addSlider("closeDelayTime",closeDelayTime,0,10);
	}
	
	void update(){
		Scene::update();
		
		if(doAdjacent){
			for (int i=0; i<getMotorCount(); i++){
				int left = (i == 0) ? getMotorCount()-1 : i - 1;
				int right = (i == getMotorCount()-1) ? 0 : i + 1;
				if(userInFrontOfMotor(i) || userInFrontOfMotor(left) || userInFrontOfMotor(right))
				{
					getMotor(i).doLimitSpeed = false;
					panelBack(i);
				} else {
					getMotor(i).doLimitSpeed = true;
					panelOpen(i);
				}
			}
		} else {
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
		}
		
		if (elapsedTime > duration) {
			stop();
		}
	}
	
	void start(){
		Scene::start();
		//Motor::doLimitSpeed = true;
	}
	
	void stop(){
		Scene::stop();
		for (int i=0; i<getMotorCount(); i++){
			getMotor(i).doLimitSpeed = true;
		}
	}
	
};
