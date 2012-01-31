#pragma once
/*
 *  SinWaveScene.h
 *		This class uses sine trig. function to create a multiple waves animation
 *
 *  Created by Miquel Lopez on 19/01/12.
 *
 */

#include "Scene.h"

class SineWaveScene : public Scene {
	
public:
	int direction;				// Two possible states: 1 (increasing) or -1 (decreasing)
	float animationAngle;		// Angle used as a reference for multiwave movement
	float sine;
	float phase;				// Phase applied to each motor in relation with the animationAngle
	float motorAngle;			// Angle for each motor. Calculated from animaTionAngle, sine, etc...
	float velocity;
	float minAngle, maxAngle;	// Motors rotation will range between minAngle and maxAngle
	int numWaves;
	
	float fade;					// For smooth fading in and out
	float fadeDuration;			// Duration of fading in and out
	float duration;				// duration of scene
	
	int counter, maxCounter;
	
	// List of variable presets
	bool setPreset1, setPreset2, setPreset3, setPreset4;
	
	static int uniqueId;
	int id;
	
	void setup(){
		Scene::setup();
		id = uniqueId++;
		name = "SineWaveScene_"+ofToString(id);
		counter = 0;
		maxCounter = 3;
		
		fade = 0;
		fadeDuration = 3;
		duration = 10;
	}
	
	void setupGUI(){
		Scene::setupGUI();
		gui.addSlider("sceneDuration", duration, 10, 300);
		gui.addSlider("fadeDuration", fadeDuration, 0, 10);
		gui.addDebug("fade", fade);
		
		gui.addTitle("Sine parameters");
		gui.addSlider("animationAngle", animationAngle, 0, 360);
		gui.addSlider("velocity", velocity, 0, 10);
		gui.addSlider("minAngle", minAngle, 0, 180);
		gui.addSlider("maxAngle", maxAngle, 0, 180);
		gui.addSlider("numWaves", numWaves, 1, 36);
		
		// Variable presets
		gui.addTitle("Variable presets").setNewColumn(true);
		gui.addButton("Preset 1", setPreset1);
		gui.addButton("Preset 2", setPreset2);
		gui.addButton("Preset 3", setPreset3);
		gui.addButton("Preset 4", setPreset4);
		
		gui.addTitle("Playlist variables").setNewColumn(true);
		//gui.addSlider("counter", counter, 0, 100);
		//gui.addSlider("maxCounter", maxCounter, 1, 100);
		
	}
	
	void postGUI(){
		Scene::postGUI();
		
		// Force settings (to be disabled at a later stage)
		animationAngle = 0;
		direction = 1;
		velocity = 1;
		
		minAngle = 0;
		maxAngle = 180;
		numWaves = 1;
		setPreset1 = false;
		setPreset2 = false;
		setPreset3 = false;
		setPreset4 = false;
	}
	
	void start(){
		Scene::start();
		
		counter = 0;
	}
	
	void update(){
		if(!enabled) return;
		
		Scene::update();
		
		// Adjust fade
		if (elapsedTime < fadeDuration) {
			fade = ofMap(elapsedTime, 0, fadeDuration, 0, 1, true);
		} else if (elapsedTime > duration - fadeDuration){
			fade = ofMap(elapsedTime, duration - fadeDuration, duration, 1, 0, true);
		} else {
			fade = 1;
		}
		
		// Detect presets
		if(setPreset1 == true)
			setPreset(1);
		if(setPreset2 == true)
			setPreset(2);
		if(setPreset3 == true)
			setPreset(3);
		if(setPreset4 == true)
			setPreset(4);
		
		animationAngle += direction*velocity;
		
		// Force animationAngle to remain between 0 and 360 degrees
		// (for readability purposes only)
		if (animationAngle > 360){
			animationAngle = animationAngle-360;
			
			/*
			// Scene playlist functionality
			counter++;
			if(counter >= maxCounter)
				stop();
			 */
		}
		
		
		if (animationAngle < 0)
			animationAngle = animationAngle+360;
		
		// Get the motor angles from the animationAngle
		for(int i=0; i<getMotorCount(); i++){
			phase = 5*(numWaves)*i;
			sine = sin((animationAngle + phase ) *DEG_TO_RAD);
			motorAngle = ofMap(sine, -1, 1, minAngle, maxAngle) * fade;
			setMotorAngle(i, motorAngle);
		}
		
		if(elapsedTime > duration + 3)
			stop();
		
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
				minAngle = 0;
				maxAngle = 30;
				numWaves = 3;
				velocity = 2.5;
				break;
			case 2:
				minAngle = 0;
				maxAngle = 90;
				numWaves = 1;
				break;
			case 3:
				minAngle = 0;
				maxAngle = 60;
				numWaves = 4;
				velocity = 5;
				break;
			case 4:
				minAngle = 0;
				maxAngle = 180;
				numWaves = 1;
				velocity = .5;
				break;
			default:
				break;
		}
	}
};
