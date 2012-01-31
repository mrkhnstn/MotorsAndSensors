#pragma once
/*
 *  SwipeScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 15/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this class can be used as a blue print to create new custom scenes
 just create a new C++ file, copy the code from here and replace SwipeScene with the name of your class
 */
class SwipeScene : public Scene {
	
public:
	
	static int uniqueId;
	int id;
	
	int tgtAngles[4];
	int tgtCounter;
	int motorCounter;
	float lastTickTime;
	float tickDuration;
	
	void setup(){
		Scene::setup();
		id = uniqueId++;
		name = "SwipeScene";
		//tgtAngles = {90,180,90,0};
		tgtAngles[0] = 90;
		tgtAngles[1] = 180;
		tgtAngles[2] = 90;
		tgtAngles[3] = 0;
		tgtCounter = 0;
		motorCounter = 0;
		tickDuration = 1;
		lastTickTime = 0;
	}
	
	void setupGUI(){
		Scene::setupGUI();
	}
	
	void postGUI(){
		Scene::postGUI();
	}
	
	void start(){
		Scene::start();
		tgtCounter = 0;
		motorCounter = 0;
		lastTickTime = ofGetElapsedTimef();
	}
	
	void update(){
		if(!enabled) return;
		Scene::update();
		
		if (ofGetElapsedTimef() > lastTickTime + tickDuration) {
			lastTickTime = ofGetElapsedTimef();
			// do tick
			getMotor(motorCounter).setAngle(tgtAngles[tgtCounter]);
			motorCounter++;
			if(motorCounter > 71){
			
			}
			
			
		}
	}
	
	void draw(){
		Scene::draw();
	}
	
	void stop(){
		Scene::stop();
	}
};
