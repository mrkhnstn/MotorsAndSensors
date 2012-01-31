#pragma once
/*
 *  MovingWindowScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 15/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this class can be used as a blue print to create new custom scenes
 just create a new C++ file, copy the code from here and replace MovingWindowScene with the name of your class
 */
class MovingWindowScene : public Scene {
	
public:
	
	static int uniqueId;
	int id;
	
	int motorCounter;
	float lastTickTime;
	float tickDuration;
	
	int windowState;
	int wallState;
	int windowSize;
	
	int windowMin;
	int windowMax;
	int windowMin1;
	int windowMax1;
	int windowMin2;
	int windowMax2;
	
	int revolutions;
	int maxRevolutions;
	
	
	void setup(){
		Scene::setup();
		id = uniqueId++;
		name = "MovingWindowScene_"+ofToString(id);
		motorCounter = 0;
		tickDuration = 1;
		lastTickTime = 0;
		wallState = 0;
		windowState = 1;
		windowSize = 10;
		
		windowMin = 0;
		windowMax = 0;
		windowMin1 = 0;
		windowMax1 = 0;
		windowMin2 = 0;
		windowMax2 = 0;
		
		revolutions = 0;
		maxRevolutions = 1;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("tickDuration", tickDuration, 0, 3);
		gui.addDebug("window pos", motorCounter);
		gui.addSlider("window size", windowSize, 1, getMotorCount() / 2);
		string choices[3] = {"black","open","white"};
		gui.addComboBox("windowState", windowState, 3, choices);
		gui.addComboBox("wallState", wallState, 3, choices);
		
		gui.addDebug("revolutions", revolutions);
		gui.addSlider("max revolutions", maxRevolutions, 1, 10);
		
		gui.addTitle("debug window");
		gui.addDebug("windowMin", windowMin);
		gui.addDebug("windowMax", windowMax);
		gui.addDebug("windowMin1", windowMin1);
		gui.addDebug("windowMax1", windowMax1);
		gui.addDebug("windowMin1", windowMin1);
		gui.addDebug("windowMax1", windowMax1);
	}
	
	void postGUI(){
		Scene::postGUI();
	}
	
	void start(){
		Scene::start();
		motorCounter = 0;
		revolutions = 0;
		lastTickTime = ofGetElapsedTimef();
	}
	
	void update(){
		if(!enabled) return;
		Scene::update();
		
		if (ofGetElapsedTimef() > lastTickTime + tickDuration) {
			lastTickTime = ofGetElapsedTimef();
			motorCounter++;
			if(motorCounter >= getMotorCount()){
				motorCounter = 0;
				revolutions++;
			}			
		}
			// do tick
			
			windowMin = motorCounter;
			windowMax = motorCounter + windowSize;
			
						
			if (windowMin < 0) {
				windowMin1 = 0;
				windowMax1 = windowMax;
				windowMin2 = getMotorCount() + windowMin;
				windowMax2 = getMotorCount();
			} else if (windowMax >= getMotorCount()) {
				windowMin1 = windowMin;
				windowMax1 = getMotorCount();
				windowMin2 = 0;
				windowMax2 = windowMax - getMotorCount();
			} else {
				windowMin1 = windowMin;
				windowMax1 = windowMax;
				windowMin2 = windowMin;
				windowMax2 = windowMax;
			}
			
			for (int i=0; i<getMotorCount(); i++){
				
				// if within winodw
				if((i >= windowMin1 && i < windowMax1) || (i >= windowMin2 && i < windowMax2)){
					switch (windowState) {
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
					switch (wallState) {
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
				}
				
				
				
				
			}
			
			//getMotor(motorCounter).setAngle(tgtAngles[tgtCounter]);
			
			if (revolutions >= maxRevolutions) {
				stop();
			}
			
	}
	
	void draw(){
		Scene::draw();
	}
	
	void stop(){
		Scene::stop();
	}
};
