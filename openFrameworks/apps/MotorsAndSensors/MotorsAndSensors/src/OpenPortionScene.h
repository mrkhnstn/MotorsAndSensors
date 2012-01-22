#pragma once
/*
 *  OpenPortionScene.h
 *		This Scene calls events (e.g.: "Rotate panel X") being repeated over time.  
 *
 *  Created by Miquel Lopez on 19/01/12.
 *
 */

#include "Scene.h"

class OpenPortionScene : public Scene {
	
public:
	// Time variables
	float startTime, lastTime, frameDuration;
	float minAngle, maxAngle, targetAngle;
	int tickCounter;
	int numPortions;
	int possiblePortionsIndex;
	
	bool setPreset1, setPreset2, setPreset3, setPreset4;
	
	void setup(){
		Scene::setup();
		name = "OpenPortionScene";
		
		targetAngle = 0;
		tickCounter = 1;
		numPortions = 3;
		
		setPreset1 = setPreset2 = setPreset3 = setPreset4 = false;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		
		gui.addSlider("minAngle", minAngle, 0, 180);
		gui.addSlider("maxAngle", maxAngle, 0, 180);
		//gui.addSlider("targetAngle", targetAngle, 0, 180);
		gui.addSlider("frameDuration", frameDuration, .05, 3);
		gui.addSlider("tickCounter", tickCounter, 1, 36);
		
		string possiblePortions[11] = {"1","2","3","4","6","8","9","12","18","24","36"};
		gui.addComboBox("possiblePortionsIndex", possiblePortionsIndex, 11, possiblePortions);
		
		
		// Variable presets
		gui.addTitle("Variable presets").setNewColumn(true);
		gui.addButton("Preset 1", setPreset1);
		//gui.addButton("Preset 2", setPreset2);
		//gui.addButton("Preset 3", setPreset3);
		//gui.addButton("Preset 4", setPreset4);
	}
	
	void postGUI(){
		Scene::postGUI();
	}
	
	void start(){
		Scene::start();
		
		startTime = ofGetElapsedTimef();
		lastTime = ofGetElapsedTimef();
	}
	
	void update(){
		Scene::update();
		
		// Detect presets
		if(setPreset1 == true)
			setPreset(1);
		if(setPreset2 == true)
			setPreset(2);
		if(setPreset3 == true)
			setPreset(3);
		if(setPreset4 == true)
			setPreset(4);
		
		if((ofGetElapsedTimef()-lastTime) > frameDuration){
			string possiblePortionsb[11] = {"1","2","3","4","6","8","9","12","18","24","36"};
			string numPortionsString = possiblePortionsb[possiblePortionsIndex];
			numPortions = atoi(numPortionsString.c_str());
			
			tickCounter++;
			if( tickCounter > numPortions ){
				tickCounter = 1;
				if(targetAngle == minAngle){
					targetAngle = maxAngle;
				}else{
					targetAngle = minAngle;
				}
			}

			
			int motorMax = ( getMotorCount() / numPortions ) * tickCounter;
			for(int i=0; i<getMotorCount(); i++){
				if( i < motorMax ){
					setMotorAngle(i, targetAngle);
				}
			}
			lastTime = ofGetElapsedTimef();
		}
	}
	
	void draw(){
		Scene::draw();
	}
	
	void stop(){
		Scene::stop();
	}
	
	
	void setPreset(int presetNum){
		setPreset1 = setPreset2 = setPreset3 = setPreset4 = false;
		switch(presetNum){
			case 1:
				// One third opens on every loop (Mercedes)
				minAngle = 0;
				maxAngle = 90;
				frameDuration = 1.5;
				tickCounter = 1;
				possiblePortionsIndex = 2;
				break;
			case 2:
				// Similar to moving wave
				minAngle = 0;
				maxAngle = 180;
				frameDuration = 0.18;
				tickCounter = 1;
				possiblePortionsIndex = 2;
				break;
			case 3:
				break;
			case 4:
				break;
			default:
				break;
		}
	}
};
