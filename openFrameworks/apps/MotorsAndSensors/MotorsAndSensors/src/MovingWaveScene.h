#pragma once
/*
 *  MovingWaveScene.h
 *		This class creates a wave effect animation.
 *
 *  Created by Miquel Lopez on 17/01/12.
 *
 */

#include "Scene.h"

class MovingWaveScene : public Scene {
	
public:
	int direction;			// Two possible states: 1 (increasing) or -1 (decreasing)
	float animationIndex;	// Ranges from 0 to 1, then 0 to 1 again
	float initialRotation;	// Rotation when position is 0
	float maxRotation;		// Maximum rotation (+/-), usually added to initialRotation
	float velocity;
	int counter, maxCounter;
	
	float reactiveAngle;
	float maxDistance;
	
	void setup(){
		Scene::setup();
		name = "MovingWaveScene";
		
		counter = 0;
		maxCounter = 3;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("initialRotation", initialRotation, 0, 90);
		gui.addSlider("maxRotation", maxRotation, 0, 180);
		gui.addSlider("animationIndex", animationIndex, 0, 1);
		gui.addSlider("velocity", velocity, 0, .005);
		
		gui.addSlider("reactiveAngle", reactiveAngle, 0, 360);
		gui.addSlider("maxDistance", maxDistance, 0, 45);
		
		gui.addTitle("Playlist variables").setNewColumn(true);
		gui.addSlider("counter", counter, 0, 100);
		gui.addSlider("maxCounter", maxCounter, 1, 100);
	}
	
	void postGUI(){
		Scene::postGUI();
		
		// Force settings (to be disabled at a later stage)
		initialRotation = 0;
		maxRotation = 75;
		animationIndex = 0;
		velocity = .002;
		direction = 1;
		
		reactiveAngle = 0;
		maxDistance = 30;
	}
	
	void start(){
		Scene::start();
		
		counter = 0;
	}
	
	void update(){
		Scene::update();
		
		// Animation sequence, animationIndex goes from 0 to 1
		// then 0 to 1 again (circle)
		animationIndex += direction*velocity;
		if (animationIndex > 1) {
			animationIndex = 0;
			
			
			// Scene playlist functionality
			// Every time we complete de circle we've completed a cycle (no. cycles = counter)
			// maxCounter defines how many cycles we'll use before showing the next animation
			counter++;
			if(counter >= maxCounter)
				stop();
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
