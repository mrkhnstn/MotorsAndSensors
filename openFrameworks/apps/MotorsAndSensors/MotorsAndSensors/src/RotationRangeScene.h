#pragma once
/*
 *  RotationRangeScene.h
 *
 *  Created by Miquel Lopez on 17/01/12.
 *
 */

#include "Scene.h"

/*
 This class creates a wave effect animation.
 */
class RotationRangeScene : public Scene {
	
public:
	int direction;						// Two possible states: 1 (increasing) or -1 (decreasing)
	float animationIndex;				// Ranges from -1 to 1
	float minRotation, maxRotation;		// Minimum and maximum rotation
	float angle;
	float velocity;
	
	void setup(){
		Scene::setup();
		name = "RotationRangeScene";
		
		animationIndex = 0;
		minRotation = 75;
		maxRotation = 105;
		velocity = .01;
		
		angle = 90;
		direction = 1;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("minRotation", minRotation, 0, 180);
		gui.addSlider("maxRotation", maxRotation, 0, 180);
		gui.addSlider("velocity", velocity, 0, .1);
		
		gui.addSlider("animationIndex", animationIndex, -1, 1);
		gui.addSlider("angle", angle, 0, 180);
	}
	
	void postGUI(){
		Scene::postGUI();
	}
	
	void start(){
		Scene::start();
	}
	
	void update(){
		Scene::update();
		
		// Animation sequence, animationIndex goes from -1 to 1 and back
		animationIndex += direction*velocity;
		if (animationIndex > 1) {
			direction = -1;
		} else if(animationIndex < -1) {
			direction = 1;
		}
		
		// angle of the motors defined by rotation range (the maximum it rotates from side to side)
		// and animationIndex (between -1 and 1)
		angle = ofMap(animationIndex, -1, 1, minRotation, maxRotation);
		for(int i=0; i<getMotorCount(); i++)
			setMotorAngle(i, angle);
	}
	
	void draw(){
		Scene::draw();
	}
	
	void stop(){
		Scene::stop();
	}
};
