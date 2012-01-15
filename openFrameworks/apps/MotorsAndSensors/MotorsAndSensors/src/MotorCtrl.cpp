/*
 *  MotorCtrl.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MotorCtrl.h"

void MotorCtrl::setup(){
	
	servoMin = 600;
	servoMax = 2000;
	
	int bank = 0;
	int ch = 1;
	for (int i=0; i<Motor::numMotors; i++) {
		Motor* motor = new Motor();
		motor->index = i;
		motor->setup();
		motor->bank = bank;
		motor->ch = ch;
		motors.push_back(motor);
		
		ch++;
		if(ch > 18) {
			ch = 1;
			bank++;
		}
	}
	
	refTime = -1;
}

void MotorCtrl::setupGUI(){
	
	gui.page(1).addPageShortcut(gui.addPage("Motors"));
	
	gui.addTitle("servo limits");
	gui.addSlider("servoMin", servoMin, 500, 2500);
	gui.addSlider("servoMax", servoMax, 500, 2500);
	gui.addSlider("servoMaxSpeed", Motor::maxMotorSpeed,0.1,2);
	
	
	// gui pages for individual elements
	gui.addTitle("motors");
	for (int j=0; j<4; j++) {
		int min = j*18;
		int max = (j+1)*18;
		gui.page("Motors").addPageShortcut(gui.addPage("Motors_"+ofToString(min)+"-"+ofToString(max-1)));
		
		gui.addTitle("angleN");
		for(int i=min; i<max && i < motors.size(); i++)
		{
			gui.addSlider("angleN_"+ofToString(i), motors[i]->angleN, 0, 1);
		}
		
		gui.addTitle("tgtAngleN").setNewColumn(true);
		for(int i=min; i<max && i < motors.size(); i++)
		{
			gui.addSlider("tgtAngleN_"+ofToString(i), motors[i]->tgtAngleN, 0, 1);
		}
		
		/*
		 gui.addTitle("bank").setNewColumn(true);
		 for(int i=min; i<max && i < motors.size(); i++)
		 {
		 gui.addSlider("bank_"+ofToString(i), motors[i]->bank, 0, 3);
		 }
		 
		 gui.addTitle("channel").setNewColumn(true);
		 for(int i=min; i<max && i < motors.size(); i++)
		 {
		 gui.addSlider("channel_"+ofToString(i), motors[i]->ch, 1, 84);
		 }
		 
		 gui.addTitle("enabled").setNewColumn(true);
		 for(int i=min; i<max && i < motors.size(); i++)
		 {
		 gui.addToggle("enabled_"+ofToString(i), motors[i]->enabled);
		 }
		 */
		
	}
	/*
	 for (int i=0; i<motors.size(); i++) {
	 motors[i]->setupGUI();
	 }
	 */
	
	gui.setPage("Motors");
	gui.addTitle("drawing");
	
	gui.addToggle("draw", Motor::doDraw);
	gui.addToggle("draw3D", Motor::doDraw3D);
	gui.addToggle("draw2D", Motor::doDraw2D);
	gui.addToggle("drawLabels", Motor::doDrawLabels);
	
	gui.addColorPicker("backColorLow", &Motor::backColorLow.r, false, true);
	gui.addColorPicker("frontColorLow", &Motor::frontColorLow.r, false, true);
	gui.addColorPicker("backColorHigh", &Motor::backColorHigh.r, false, true);
	gui.addColorPicker("frontColorHigh", &Motor::frontColorHigh.r, false, true);
}

void MotorCtrl::postGUI(){
	for (int i=0; i<motors.size(); i++) {
		motors[i]->postGUI();
	}
}

void MotorCtrl::update(){
	
	// update max allowed speed
	Motor::maxAngleNSpeed = 1 / (Motor::maxMotorSpeed * 3);
	
	// update elapsed time used by motors
	if (refTime == -1) {
		refTime = ofGetElapsedTimef();
	} else {
		float newTime = ofGetElapsedTimef();
		Motor::elapsedTime = newTime - refTime;
		refTime = newTime;
	}
	
	for (int i=0; i<motors.size(); i++) {
		motors[i]->update();
	}
}

void MotorCtrl::draw(){
	for (int i=0; i<motors.size(); i++) {
		motors[i]->draw();
	}
}