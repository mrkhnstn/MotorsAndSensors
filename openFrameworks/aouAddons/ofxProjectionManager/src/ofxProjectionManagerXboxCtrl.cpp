/*
 *  ofxProjectionManagerXboxCtrl.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxProjectionManagerXboxCtrl.h"

ofxProjectionManagerXboxCtrl::ofxProjectionManagerXboxCtrl() : ofxBaseObject(){
	joystickUpdateDelay = 100;
	nextJoystickUpdate = ofGetElapsedTimeMillis() + joystickUpdateDelay;
	projectionManager = Singleton<ofxProjectionManager>::instance();
	enabled = false;
}

void ofxProjectionManagerXboxCtrl::setup(){
	
	xboxController = Singleton<ofxXboxController>::instance();
	
	joystickFactor = 5;
	buttonFactor = 1;
	
	ofAddListener(xboxController->start.onOn, this, &ofxProjectionManagerXboxCtrl::startButton);
	ofAddListener(xboxController->back.onOn, this, &ofxProjectionManagerXboxCtrl::backButton);
	ofAddListener(xboxController->home.onOn, this, &ofxProjectionManagerXboxCtrl::homeButton);
	
	/*
	 ofAddListener(xboxController->lb.onOn, this, &ofxXboxGUIControl::lbButton);
	 ofAddListener(xboxController->rb.onOn, this, &ofxXboxGUIControl::rbButton);
	 
	 ofAddListener(xboxController->dUp.onOn, this, &ofxXboxGUIControl::prevFocus);
	 ofAddListener(xboxController->dDown.onOn, this, &ofxXboxGUIControl::nextFocus);
	 ofAddListener(xboxController->a.onOn, this, &ofxXboxGUIControl::keyEnter);
	 
	 ofAddListener(xboxController->x.onOn, this, &ofxXboxGUIControl::keyLeft);
	 ofAddListener(xboxController->b.onOn, this, &ofxXboxGUIControl::keyRight);
	 */
}

void ofxProjectionManagerXboxCtrl::setupGUI(){
	gui.addSlider("joystick factor", joystickFactor, 0.1, 10);
	gui.addSlider("button factor", buttonFactor, 0.1, 10);
	gui.addSlider("button repeat start delay", ofxButtonObserver::repeatStartDelay, 1, 1000);
	gui.addSlider("button repeat delay", ofxButtonObserver::repeatDelay, 1, 1000);
	
}

void ofxProjectionManagerXboxCtrl::addButtonListeners(){
	ofAddListener(xboxController->dLeft.onOn, this, &ofxProjectionManagerXboxCtrl::dLeft);
	ofAddListener(xboxController->dRight.onOn, this, &ofxProjectionManagerXboxCtrl::dRight);
	ofAddListener(xboxController->dUp.onOn, this, &ofxProjectionManagerXboxCtrl::dUp);
	ofAddListener(xboxController->dDown.onOn, this, &ofxProjectionManagerXboxCtrl::dDown);
	
	ofAddListener(xboxController->x.onOn, this, &ofxProjectionManagerXboxCtrl::xButton);
	ofAddListener(xboxController->y.onOn, this, &ofxProjectionManagerXboxCtrl::yButton);
	ofAddListener(xboxController->a.onOn, this, &ofxProjectionManagerXboxCtrl::aButton);
	ofAddListener(xboxController->b.onOn, this, &ofxProjectionManagerXboxCtrl::bButton);
	
	ofAddListener(xboxController->lb.onOn, this, &ofxProjectionManagerXboxCtrl::lbButton);
	ofAddListener(xboxController->rb.onOn, this, &ofxProjectionManagerXboxCtrl::rbButton);
}

void ofxProjectionManagerXboxCtrl::removeButtonListeners(){
	ofRemoveListener(xboxController->dLeft.onOn, this, &ofxProjectionManagerXboxCtrl::dLeft);
	ofRemoveListener(xboxController->dRight.onOn, this, &ofxProjectionManagerXboxCtrl::dRight);
	ofRemoveListener(xboxController->dUp.onOn, this, &ofxProjectionManagerXboxCtrl::dUp);
	ofRemoveListener(xboxController->dDown.onOn, this, &ofxProjectionManagerXboxCtrl::dDown);
	
	ofRemoveListener(xboxController->x.onOn, this, &ofxProjectionManagerXboxCtrl::xButton);
	ofRemoveListener(xboxController->y.onOn, this, &ofxProjectionManagerXboxCtrl::yButton);
	ofRemoveListener(xboxController->a.onOn, this, &ofxProjectionManagerXboxCtrl::aButton);
	ofRemoveListener(xboxController->b.onOn, this, &ofxProjectionManagerXboxCtrl::bButton);
	
	ofRemoveListener(xboxController->lb.onOn, this, &ofxProjectionManagerXboxCtrl::lbButton);
	ofRemoveListener(xboxController->rb.onOn, this, &ofxProjectionManagerXboxCtrl::rbButton);
}

void ofxProjectionManagerXboxCtrl::update(){
	
	if (enabled) {
		
		if (ofGetElapsedTimeMillis() > nextJoystickUpdate) {
			nextJoystickUpdate = ofGetElapsedTimeMillis() + joystickUpdateDelay;
			ofxXboxController::PadUI& pad = xboxController->pad;
			
			if(pad.leftAnalogX < 0){
				projectionManager->leftLeft(-1*pad.leftAnalogX * joystickFactor,true);
			} else if (pad.leftAnalogX > 0) {
				projectionManager->leftRight(pad.leftAnalogX * joystickFactor,true);
			}
			
			if(pad.leftAnalogY < 0){
				projectionManager->leftUp(-1*pad.leftAnalogY * joystickFactor,true);
			} else if (pad.leftAnalogY > 0) {
				projectionManager->leftDown(pad.leftAnalogY * joystickFactor,true);
			}
			
			if(pad.rightAnalogX < 0){
				projectionManager->rightLeft(-1*pad.rightAnalogX * joystickFactor,true);
			} else if (pad.rightAnalogX > 0) {
				projectionManager->rightRight(pad.rightAnalogX * joystickFactor,true);
			}
			
			if(pad.rightAnalogY < 0){
				projectionManager->rightUp(-1*pad.rightAnalogY * joystickFactor,true);
			} else if (pad.rightAnalogY > 0) {
				projectionManager->rightDown(pad.rightAnalogY * joystickFactor,true);
			}
			
		}
		
	}
	
}

void ofxProjectionManagerXboxCtrl::enableCtrl(){
	if(!enabled){
		enabled = true;
		projectionManager->enableEditing(true);
		gui.hide();
		addButtonListeners();
	}
}

void ofxProjectionManagerXboxCtrl::disableCtrl(){
	if (enabled) {
		enabled = false;
		projectionManager->enableEditing(false); //< this will also save the grids
		removeButtonListeners();
	}
}

void ofxProjectionManagerXboxCtrl::homeButton(ofEventArgs& e){
	//disableCtrl();
}

void ofxProjectionManagerXboxCtrl::backButton(ofEventArgs& e){
	/*
	 if (!enabled) {
	 enabled = true;
	 projectionManager->enableEditing(true);
	 gui.hide();
	 addButtonListeners();
	 } else {
	 projectionManager->prevOutput();
	 }
	 */
}

void ofxProjectionManagerXboxCtrl::startButton(ofEventArgs& e){
	/*
	 if (!enabled) {
	 enabled = true;
	 projectionManager->enableEditing(true);
	 gui.hide();
	 addButtonListeners();
	 } else {
	 projectionManager->nextOutput();
	 }
	 */
}

void ofxProjectionManagerXboxCtrl::yButton(ofEventArgs& e){
	projectionManager->rightUp(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::aButton(ofEventArgs& e){
	projectionManager->rightDown(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::xButton(ofEventArgs& e){
	projectionManager->rightLeft(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::bButton(ofEventArgs& e){
	projectionManager->rightRight(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::dUp(ofEventArgs& e){
	projectionManager->leftUp(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::dDown(ofEventArgs& e){
	projectionManager->leftDown(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::dLeft(ofEventArgs& e){
	projectionManager->leftLeft(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::dRight(ofEventArgs& e){
	projectionManager->leftRight(buttonFactor);
}

void ofxProjectionManagerXboxCtrl::lbButton(ofEventArgs& e){
	projectionManager->previousMode();
}

void ofxProjectionManagerXboxCtrl::rbButton(ofEventArgs& e){
	projectionManager->nextMode();
}