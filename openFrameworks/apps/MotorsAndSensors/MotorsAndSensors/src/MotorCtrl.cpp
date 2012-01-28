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
	gui.addSlider("userOffDelayTime", Motor::userOffDelayTime,0,3);
	gui.addSlider("servoMaxSpeed", Motor::maxMotorSpeed,1,180);
	gui.addSlider("distanceToCentre", Motor::distanceToCentre, 270, 350);
	
	// gui pages for individual elements
	gui.addTitle("motors");
	for (int j=0; j<4; j++) {
		int min = j*18;
		int max = (j+1)*18;
		gui.page("Motors").addPageShortcut(gui.addPage("Motors_"+ofToString(min)+"-"+ofToString(max-1)));
		
		gui.addTitle("angle");
		for(int i=min; i<max && i < motors.size(); i++)
		{
			gui.addSlider("angle_"+ofToString(i), motors[i]->angle, 0, 1);
		}
		
		gui.addTitle("tgtAngle").setNewColumn(true);
		for(int i=min; i<max && i < motors.size(); i++)
		{
			gui.addSlider("tgtAngle_"+ofToString(i), motors[i]->tgtAngle, 0, 1);
		}
		
		gui.addTitle("proximityValue").setNewColumn(true);
		for(int i=min; i<max && i < motors.size(); i++)
		{
			gui.addDebug("proximityValue_"+ofToString(i), motors[i]->proximityValue);
		}
	}
	
	// add calibration pages
	gui.setPage("Motors");
	gui.addTitle("calibration");
	gui.page("Motors").addPageShortcut(gui.addPage("Calibrate_Pulse_Min"));
	string calibrationChoices[3] = {"none","pulse min", "pulse max"};
	gui.addComboBox("calibration mode", Motor::calibrationMode, 3, calibrationChoices);
	gui.addSlider("globalPulseMin", Motor::globalPulseMin, 550, 2350);
	gui.addButton("setAllToGlobal", this, &MotorCtrl::setAllToGlobalPulseMin);
	for (int i=0; i<motors.size(); i++) {
		gui.addSlider("motor_"+ofToString(i), motors[i]->pulseMin, 550, 2350);
	}
	
	gui.page("Motors").addPageShortcut(gui.addPage("Calibrate_Pulse_Max"));
	gui.addComboBox("calibration mode", Motor::calibrationMode, 3, calibrationChoices);
	gui.addSlider("globalPulseMax", Motor::globalPulseMax, 550, 2350);
	for (int i=0; i<motors.size(); i++) {
		gui.addSlider("motor_"+ofToString(i), motors[i]->pulseMin, 550, 2350);
	}
	gui.addButton("setAllToGlobal", this, &MotorCtrl::setAllToGlobalPulseMax);
	 
	// add drawing parameters to gui
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
	Motor::calibrationMode = 0;
	for (int i=0; i<motors.size(); i++) {
		motors[i]->postGUI();
	}
}

void MotorCtrl::update(){
	
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

void MotorCtrl::draw(){}

void MotorCtrl::draw3d(){
	for (int i=0; i<motors.size(); i++) {
		motors[i]->draw();
	}
}

void MotorCtrl::setAllToGlobalPulseMin(ofEventArgs& e){
	for(int i=0; i<motors.size(); i++)
		motors[i]->pulseMin = Motor::globalPulseMin;
}

void MotorCtrl::setAllToGlobalPulseMax(ofEventArgs& e){
	for(int i=0; i<motors.size(); i++)
		motors[i]->pulseMin = Motor::globalPulseMax;

}