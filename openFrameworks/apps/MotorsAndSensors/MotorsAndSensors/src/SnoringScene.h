#pragma once
/*
 *  SnoringScene.h
 *
 *  Created by Miquel Lopez on 20/01/12.
 *
 */

#include "Scene.h"

class SnoringScene : public Scene {
	
public:
	int direction;						// Two possible states: 1 (increasing) or -1 (decreasing)
	float animationIndex;				// Ranges from -1 to 1
	float minRotation, maxRotation;		// Minimum and maximum rotation
	float velocity, acceleration;		// Animations velocity and acceleration
	float angle;
	float oldTime, waitTime, maxWaitTime;
	bool snoringMode;
	
	// Defines the type of animation
	int animationMode;
	static const int OPENING = 0;
	static const int CLOSING = 1;
	static const int WAITING = 2;
	
	void setup(){
		Scene::setup();
		name = "SnoringScene";
		
		oldTime = 0;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("minRotation", minRotation, 0, 180);
		gui.addSlider("maxRotation", maxRotation, 0, 180);
		gui.addSlider("velocity", velocity, 0, .1);
		gui.addSlider("acceleration", acceleration, .1, 10);
		gui.addSlider("animationIndex", animationIndex, 0, 1);
		gui.addSlider("angle", angle, 0, 180);
		gui.addSlider("maxWaitTime", maxWaitTime, 0, 2);
		gui.addToggle("snoringMode", snoringMode);
	}
	
	void postGUI(){
		Scene::postGUI();
		
		animationIndex = 0;
		minRotation = 0;
		maxRotation = 115;
		velocity = .01;
		acceleration = 1;
		angle = 90;
		direction = 1;
		maxWaitTime = .8;
		snoringMode = true;
		
		animationMode = OPENING;
	}
	
	void start(){
		Scene::start();
	}
	
	void update(){
		Scene::update();
		
		bool snoring = false;
		float animationIndexSnore;
		switch (animationMode) {
			case OPENING:
				direction = 1;
				velocity = (animationIndex/65)+.001;
				
				// Snoring or proper sleeping?
				if(snoringMode == true){
					// Snoring vibration (on top)
					if(animationIndex >= .66 && animationIndex < .7)
						snoring = true;
					if(animationIndex >= .88 && animationIndex < .93)
						snoring = true;
					if(animationIndex >= .94 && animationIndex < .99)
						snoring = true;
					
					// Alternative index when snoring
					animationIndex += direction*(velocity);
					animationIndexSnore = animationIndex-direction*(velocity*7);
					if(snoring == true)
						angle = ofMap(animationIndexSnore, 0, 1, minRotation, maxRotation);
					else
						angle = ofMap(animationIndex, 0, 1, minRotation, maxRotation);
					
				}else{
					animationIndex += direction*(velocity);
					angle = ofMap(animationIndex, 0, 1, minRotation, maxRotation);
				}
				
				if(animationIndex >= 1){
					animationMode = CLOSING;
				}
				break;
			case CLOSING:
				direction = -1;
				velocity = (animationIndex/20)-.0025;
				if(velocity<.003)
					velocity = .003;
				
				animationIndex += direction*(velocity);
				angle = ofMap(animationIndex, 0, 1, minRotation, maxRotation);
				
				if(animationIndex <= 0){
					oldTime = ofGetElapsedTimef();
					animationMode = WAITING;
				}
				break;
			case WAITING:
				waitTime = ofGetElapsedTimef()-oldTime;
				if(waitTime >= maxWaitTime){
					animationMode = OPENING;
				}
				break;
			default:
				break;
		}
		
		
		for(int i=0; i<getMotorCount(); i++){
			setMotorAngle(i, angle);
		}
	}
	
	void draw(){
		Scene::draw();
	}
	
	void stop(){
		Scene::stop();
	}
	
};