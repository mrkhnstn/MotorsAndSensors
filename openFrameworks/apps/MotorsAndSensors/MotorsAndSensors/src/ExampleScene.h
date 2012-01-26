#pragma once
/*
 *  ExampleScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 15/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
this class explains how to create your own scene
 */

class ExampleScene : public Scene { //< every custom scene needs to inherit from Scene
public:

	float	manualAngle; 
	
	// animation fields
	float	angleVel;
	float	dir;
	
	int		angleMode; 
	// 0: set all panels to manual angle
	// 1: set all panels to front face
	// 2: open all panels
	// 3: set all panels to back face
	// 4: animate panels
	
	int		motorSelected;
	float	selectedMotorAngle;
	bool	highlightSelectedMotor;
	
	float	duration;
	float	stopTime;
	float	deltaTime;
	float	oldTime;
	
	/////////////////////////////////////////////////////////////////////////////////
	// FUNCTION CALL ORDER
	
	// the functions of the scene are called automatically from SceneCtrl

	// ONCE ON STARTUP
	// constructor()
	// setup();		<< best to initialize everything here
	// setupGUI();	<< add all gui related code
	// postGUI();	<< gets called after all settings are loaded. useful if you want to overwrite settings before

	// ONCE ON STARTING SCENE
	// start();		<< gets called on activation (usually by SceneCtrl when this scene is set to be the current scene)
	
	// ON LOOP IF ACTIVATED
	// update();	<< add anything to do with moving the motors into this function
	// draw();		<< use this to draw help objects related to this scene
	
	// ONCE ON STOPPING SCENE
	// stop();		<< this function should be called from within the scene when the scene is over. this will notify SceneCtrl to select and play the next scene
	
	/////////////////////////////////////////////////////////////////////////////////
	
	// intialize all you objects required for this scene here
	void setup(){
		Scene::setup();			//<< MANDATORY!! usually for each function that you override call the parent function first
		name = "ExampleScene";	//<< MANDATORY!! always give a unique name to a scene. 
		
		// initialize fields of this scene
		manualAngle = 0;
		angleVel = 0;
		dir = 1;
		angleMode = 0;
		motorSelected = 0;
		selectedMotorAngle = 0;
		highlightSelectedMotor = true;
		duration = 10;
		stopTime = ofGetElapsedTimef() + duration;
		oldTime = ofGetElapsedTimef();
		deltaTime = 0;
		
	}
	
	// hook up relevant fields into the GUI within this funciton
	void setupGUI(){
		Scene::setupGUI(); //<< MANDATORY!! adds a GUI page with the name of this scene
		
		gui.addSlider("angle", manualAngle, 0, 180);									// add a float slider
		gui.addSlider("velocity", angleVel, 0, 180);
		string angleModeChoices[5] = {"angle","front","open","back","animate"};
		gui.addComboBox("angleMode", angleMode, 5, angleModeChoices);			// add combobox to set how 
		
		gui.addTitle("motor");													// add a title bar to structure the gui page
		gui.addSlider("motorSelected", motorSelected, 0, getMotorCount() - 1);	// add an int slider
		gui.addDebug("selectedMotorAngle", selectedMotorAngle);					// this displays the selectedMotorAngle fields but it cannot be changed via the guis
		gui.addToggle("highlightSelectedMotor", highlightSelectedMotor);		//	add a toggle to highlight selected motor
	
		
	}
	
	// add any code that shall be executed after settings have been loaded from xml and before the frame loop starts
	void postGUI(){
		Scene::postGUI(); //<< MANDATORY!!
		
		manualAngle = 0;
		//^ as angle is hooked up into the GUI on startup angle is set to a value read from the settings xml
		//  if you want to force angle to be a certain value on startup independent from the settings xml then you can do this here
	}
	
	// add any code that shall be executed before a scene starts playing
	void start(){
		Scene::start(); //<< MANDATORY!!
		
		oldTime = ofGetElapsedTimef();
		stopTime = ofGetElapsedTimef() + duration; //< we'll set the time to stop the scene here
	}
	
	// add any code that shall be executed on the frame loop if this scene is active
	void update(){
		Scene::update(); //<< MANDATORY!!
		
		// calculate time elapsed from last frame (used for animation)
		float newTime = ofGetElapsedTimef();
		deltaTime = newTime - oldTime;
		oldTime = newTime;
		
		// set the angle of all motors depending on the angleMode set via the GUI
		switch (angleMode) {
			case 0:	// set angles of all motors according to the manualAngle slider
				for(int i=0; i<getMotorCount(); i++)
					setMotorAngle(i, manualAngle);
				break;
			case 1: // set all motors to display front panel
				for(int i=0; i<getMotorCount(); i++)
					panelFront(i);
				break;
			case 2: // open all panels
				for(int i=0; i<getMotorCount(); i++)
					panelOpen(i);
				break;
			case 3: // set all motors to display back panel
				for(int i=0; i<getMotorCount(); i++)
					panelBack(i);
				break;
			case 4: // animate panels back and forth
				manualAngle += dir * angleVel * deltaTime;
				//
				if (dir > 0 && manualAngle > 180) {
					manualAngle = 180;
					dir *= -1;
				} else if (dir < 0 && manualAngle < 0) {
					manualAngle = 0;
					dir *= -1;
				}
				for(int i=0; i<getMotorCount(); i++)
					setMotorAngle(i, manualAngle);
				break;
			default:
				break;
		}
		
		// update selectedMotorAngle field
		selectedMotorAngle = getMotorAngle(motorSelected); //<< see debug field in GUI
		
		// stop the scene if the scene duration has elapsed >> see start()
		if (ofGetElapsedTimef() > stopTime) {
			stop();
		}
	}
	
	// add anything that shall be drawn onto screen to understand how this scene is working
	void draw(){
		Scene::draw(); //<< MANDATORY!!
		
		if(highlightSelectedMotor){
			// draw a yellow circle around selected motor
			ofPushStyle();
			ofSetColor(255, 255, 0);
			ofNoFill();
			ofSetLineWidth(1);
			ofxVec2f motorPos = getMotorPos(motorSelected);
			ofCircle(motorPos.x, motorPos.y, 10);
			ofPopStyle();
		}	
		
	}
	
	// add any code to reset the scene
	// this function usually gets called by SceneCtrl before it starts another scene 
	void stop(){
		Scene::stop(); //<< MANDATORY!!
	}

};