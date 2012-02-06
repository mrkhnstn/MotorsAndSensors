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
	
	int tgtAngles[10];
	int tgtCounter;
	int maxTgts;
	int motorCounter;
	float lastTickTime;
	float tickDuration;
	
	
	void setup(){
		Scene::setup();
		id = uniqueId++;
		name = "SwipeScene_"+ofToString(id);
		
		for(int i=0; i<10; i++)
		{
			tgtAngles[i] = 0;
		}
		maxTgts = 4;
		tgtCounter = 0;
		motorCounter = 0;
		tickDuration = 1;
		lastTickTime = 0;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("tickDuration", tickDuration, 0, 3);
		gui.addDebug("state", tgtCounter);
		gui.addDebug("swipe pos", motorCounter);
		gui.addSlider("maxStates",maxTgts, 1,10);
		string choices[3] = {"black","open","white"};
		for(int i=0; i<10; i++){
			gui.addComboBox("state_"+ofToString(i), tgtAngles[i], 3, choices);
		}
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
		
		
			// do tick
			
			for (int i=0; i<getMotorCount(); i++){
				
					if (i <= motorCounter) {
						switch (tgtAngles[tgtCounter]) {
							case 0:
								if(userInFrontOfMotor(i))
								{
									getMotor(i).doLimitSpeed = false;
									panelOpen(i);
								} else {
									getMotor(i).doLimitSpeed = true;
									panelFront(i);
								}
								break;
							case 1:
								if(userInFrontOfMotor(i))
								{
									getMotor(i).doLimitSpeed = false;
									panelFront(i);
								} else {
									getMotor(i).doLimitSpeed = true;
									panelOpen(i);
								}
								break;
							case 2:
								if(userInFrontOfMotor(i))
								{
									getMotor(i).doLimitSpeed = false;
									panelOpen(i);
								} else {
									getMotor(i).doLimitSpeed = true;
									panelBack(i);
								}
								break;					
							default:
								break;
						}
					} else {
						if (tgtCounter < 1) {
							panelFront(i);
						} else {
							switch (tgtAngles[tgtCounter-1]) {
								case 0:
									if(userInFrontOfMotor(i))
									{
										getMotor(i).doLimitSpeed = false;
										panelOpen(i);
									} else {
										getMotor(i).doLimitSpeed = true;
										panelFront(i);
									}
									break;
								case 1:
									if(userInFrontOfMotor(i))
									{
										getMotor(i).doLimitSpeed = false;
										panelBack(i);
									} else {
										getMotor(i).doLimitSpeed = true;
										panelOpen(i);
									}
									break;
								case 2:
									if(userInFrontOfMotor(i))
									{
										getMotor(i).doLimitSpeed = false;
										panelOpen(i);
									} else {
										getMotor(i).doLimitSpeed = true;
										panelBack(i);
									}
									break;					
								default:
									break;
							}							
						}
					}
										
				
			}
						
			//getMotor(motorCounter).setAngle(tgtAngles[tgtCounter]);
			
		
		if (ofGetElapsedTimef() > lastTickTime + tickDuration) {
			lastTickTime = ofGetElapsedTimef();
			
			motorCounter++;
			if(motorCounter > 71){
				motorCounter = 0;
				tgtCounter++;
				if (tgtCounter >= maxTgts || tgtCounter >= 10) {
					tgtCounter = 0;
					stop();
				}
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
