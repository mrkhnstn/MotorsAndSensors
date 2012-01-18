#pragma once
/*
 *  WaveScene.h
 *
 *  Created by Miquel Lopez on 17/01/12.
 *
 */

#include "Scene.h"

/*
 This class creates a wave effect animation.
 */
class WaveScene : public Scene {
	
public:
	int direction;			// Two possible states: 1 (increasing) or -1 (decreasing)
	float animationIndex;	// Ranges from 0 to 1, then 0 to 1 again
	float initialRotation;	// Rotation when position is 0
	float maxRotation;		// Maximum rotation (+/-), usually added to initialRotation
	float velocity;
	
	float reactiveAngle;
	float maxDistance;
	
	void setup(){
		Scene::setup();
		name = "WaveScene";
		
		initialRotation = 0;
		maxRotation = 30;
		animationIndex = 0;
		velocity = .05;
		direction = 1;
		
		reactiveAngle = 0;
		maxDistance = 15;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("initialRotation", initialRotation, 0, 90);
		gui.addSlider("maxRotation", maxRotation, 0, 180);
		gui.addSlider("animationIndex", animationIndex, 0, 1);
		gui.addSlider("velocity", velocity, 0, .05);
		
		gui.addSlider("reactiveAngle", reactiveAngle, 0, 360);
		gui.addSlider("maxDistance", maxDistance, 0, 45);
	}
	
	void postGUI(){
		Scene::postGUI();
	}
	
	void start(){
		Scene::start();
	}
	
	void update(){
		Scene::update();
		
		// Animation sequence, animationIndex goes from 0 to 1
		// then 0 to 1 again (circle)
		animationIndex += direction*velocity;
		if (animationIndex > 1) {
			animationIndex = 0;
		}
		
		reactiveAngle = ofMap(animationIndex, 0, 1, 0, 360);
		float distance = 0;
		float interMotorAngle = 360/getMotorCount();
		for(int i=0; i<getMotorCount(); i++){
			distance = reactiveAngle - interMotorAngle*i;
			
			// Fix to get the distance right
			// both at the start and the end of the circle
			if(distance > 180){
				distance = distance-360;
			}else if(distance < -180){
				distance = distance+360;
			}
			
			// If a panel is close enough to the reactiveAngle it reacts to it
			if(abs(distance) < maxDistance){
				setMotorAngle(i, initialRotation + maxRotation*(distance/maxDistance));
			}else{
				setMotorAngle(i, initialRotation);
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
