#pragma once
/*
 *  RotationRangeScene.h
 *		This class creates an linear animation where panels move from a minimum to a maximum angle.
 *		Different animation modes will define different versions of the animation
 *
 *  Created by Miquel Lopez on 17/01/12.
 *
 */

#include "Scene.h"

class RotationRangeScene : public Scene {
	
public:
	int direction;						// Two possible states: 1 (increasing) or -1 (decreasing)
	float animationIndex;				// Ranges from -1 to 1
	float minRotation, maxRotation;		// Minimum and maximum rotation
	float velocity;						// Animations velocity
	float angle;

	// Defines the type of animation
	int animationMode;
	static const int NORMAL = 0;
	static const int ODD_EVEN = 1;
	
	// List of variable presets
	bool setPreset1, setPreset2;
		
	void setup(){
		Scene::setup();
		name = "RotationRangeScene";
		
		animationIndex = 0;
		minRotation = 0;
		maxRotation = 180;
		velocity = .03;
		angle = 90;
		direction = 1;
		
		animationMode = NORMAL;
		setPreset1 = false;
		setPreset2 = false;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("minRotation", minRotation, 0, 180);
		gui.addSlider("maxRotation", maxRotation, 0, 180);
		gui.addSlider("velocity", velocity, 0, .1);
		gui.addSlider("animationIndex", animationIndex, -1, 1);
		gui.addSlider("angle", angle, 0, 180);
		
		string modeChoices[2] = {"NORMAL","ODD_EVEN"};
		gui.addComboBox("animationMode", animationMode, 2, modeChoices);
		
		// Presets
		gui.addTitle("Variable presets").setNewColumn(true);
		gui.addButton("Preset 1", setPreset1);
		gui.addButton("Preset 2", setPreset2);
	}
	
	void postGUI(){
		Scene::postGUI();
	}
	
	void start(){
		Scene::start();
	}
	
	void update(){
		Scene::update();
		
		// Detect presets
		if(setPreset1 == true)
			setPreset(1);
		if(setPreset2 == true)
			setPreset(2);
		
		// Animation sequence, animationIndex goes from -1 to 1 and back
		animationIndex += direction*velocity;
		if (animationIndex > 1) {
			direction = -1;
		} else if(animationIndex < -1) {
			direction = 1;
		}
		
		// angle of the motors defined by rotation range (the maximum it rotates from side to side)
		// and animationIndex (between -1 and 1)
		// angle = ofMap(animationIndex, -1, 1, minRotation, maxRotation);
		for(int i=0; i<getMotorCount(); i++){
			switch (animationMode) {
				case NORMAL:
					angle = ofMap(animationIndex, -1, 1, minRotation, maxRotation);
					break;
				case ODD_EVEN:
					if(i%2 == 0){
						angle = ofMap(animationIndex, -1, 1, minRotation, maxRotation);
					}else{
						angle = ofMap(-animationIndex, -1, 1, minRotation, maxRotation);
					}
					break;
				default:
					break;
			}
			setMotorAngle(i, angle);
		}
	}
	
	void draw(){
		Scene::draw();
	}
	
	void stop(){
		Scene::stop();
	}
	
	
	void setPreset(int presetNum){
		switch(presetNum){
			case 1:
				minRotation = 75;
				maxRotation = 105;
				break;
			case 2:
				minRotation = 0;
				maxRotation = 180;
				break;
			default:
				break;
		}
	}
	
};
