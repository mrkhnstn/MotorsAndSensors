#pragma once
/*
 *  ofxXboxController.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 22/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxOscManager.h"
#include "Singleton.h"
#include "ofxValueObserverT.h"
#include "ofxButtonObserver.h"

class ofxXboxController : public ofxBaseObject, public ofxOscHandler {

public:
	
	ofxOscManager	oscManager;
	
	// Values for frequently updating analog inputs
	float					timeElapsed;
	float					updateFrequency;
	
	struct PadUI {
		
		bool back;
		bool start;
		bool home;
		
		int lt;
		int rt;
		
		bool lb;
		bool rb;
		
		bool dUp;
		bool dDown;
		bool dLeft;
		bool dRight;
		
		bool x;
		bool y;
		bool a;
		bool b;
		
		float	leftAnalogX;
		float	leftAnalogY;
		float	rightAnalogX;
		float	rightAnalogY;
		
	} pad;
	
	ofxButtonObserver start;
	ofxButtonObserver back;
	ofxButtonObserver home;
	
	ofxButtonObserver lb;
	ofxButtonObserver rb;
	
	ofxButtonObserver dUp;
	ofxButtonObserver dDown;
	ofxButtonObserver dLeft;
	ofxButtonObserver dRight;

	ofxButtonObserver x;
	ofxButtonObserver y;
	ofxButtonObserver a;
	ofxButtonObserver b;
	
	void setup(){
		// setup OSC
		//TODO: parameterize port
		//TODO: should I use Singleton OSC?
		oscManager.setup(7777, true);
		oscManager.registerInterest(*this, "/pad");
		
		home.setup(pad.home);
		start.setup(pad.start);
		back.setup(pad.back);
		lb.setup(pad.lb);
		rb.setup(pad.rb);
		
		dUp.setup(pad.dUp);
		dDown.setup(pad.dDown);
		dLeft.setup(pad.dLeft);
		dRight.setup(pad.dRight);
		
		x.setup(pad.x);
		y.setup(pad.y);
		a.setup(pad.a);
		b.setup(pad.b);
	}
	
	void update(){
		home.update();
		start.update();
		back.update();
		lb.update();
		rb.update();
		
		dUp.update();
		dDown.update();
		dLeft.update();
		dRight.update();
		
		x.update();
		y.update();
		a.update();
		b.update();
	}
	
	void setupGUI(){
		
		gui.page(1).addPageShortcut(gui.addPage("Xbox Controller"));

		gui.addTitle("ABXY buttons");
		gui.addToggle("A", pad.a);
		gui.addToggle("B", pad.b);
		gui.addToggle("X", pad.x);
		gui.addToggle("Y", pad.y);
		
		gui.addTitle("centre buttons");
		gui.addToggle("back", pad.back);
		gui.addToggle("start", pad.start);
		gui.addToggle("home", pad.home);
		
		gui.addTitle("top buttons");
		gui.addSlider("LT", pad.lt, 0, 255);
		gui.addSlider("RT", pad.rt, 0, 255);
		gui.addToggle("LB", pad.lb);
		gui.addToggle("RB", pad.rb);
		
		gui.addTitle("d pad");
		gui.addToggle("up",pad.dUp);
		gui.addToggle("down",pad.dDown);
		gui.addToggle("left",pad.dLeft);
		gui.addToggle("right",pad.dRight);
		
		gui.addTitle("left joystick");
		gui.addSlider("x", pad.leftAnalogX, -1, 1);
		gui.addSlider("y", pad.leftAnalogY, -1, 1);
		
		gui.addTitle("right joystick");
		gui.addSlider("x", pad.rightAnalogX, -1, 1);
		gui.addSlider("y", pad.rightAnalogY, -1, 1);
		
	}
	
	void postGUI(){
		pad.a = false;
		pad.b = false;
		pad.x = false;
		pad.y = false;
		
		pad.back = false;
		pad.start = false;
		pad.home = false;
		pad.lt = 0;
		pad.rt = 0;
		pad.lb = false;
		pad.rb = false;
		pad.dUp = false;
		pad.dDown = false;
		pad.dLeft = false;
		pad.dRight = false;
		pad.leftAnalogX = 0;
		pad.leftAnalogY = 0;
		pad.rightAnalogX = 0;
		pad.rightAnalogY = 0;
	}
	
	// override handleOscMessage
	void handleOscMessage(ofxOscMessage& message){
		string address = message.getAddress();
		if (address == "/pad") { //< cannot use switch heres
			int element = castOscNumberToInt(message,0); //<castOscNumberToInt is inherited from handler
			float value = castOscNumberToFloat(message,1); //<castOscNumberToFloat is inherited from handler
			//ofLog(OF_LOG_VERBOSE, "received input from element " + ofToString(element) + " with value " + ofToString(value));
			handleInput(element, value);
		}
	}
	
	/**
	 element is the device element ID and corresponds to a button or pad
	 value is the value of the specified device element
	 */
	void handleInput(int element, float value){
		
		switch (element) {
				
				// Misc //////////////////////////////////////////////////
			case 14: // start
				//pad.start = value;
				start.set(value);
				break;
				
			case 15: // back
				back.set(value);
				//pad.back = value;
				break;
				
				// Dpad //////////////////////////////////////////////////
			case 10:
				// dpad up
				//pad.dUp = value;
				dUp.set(value);
				break;
			case 11:
				// dpad down
				//pad.dDown = value;
				dDown.set(value);
				break;
			case 12:
				// dpad left
				//pad.dLeft = value;
				dLeft.set(value);
				break;
			case 13:
				// dpad right
				//pad.dRight = value;
				dRight.set(value);
				break;
			case 20:
				//pad.home = value;
				home.set(value);
				break;
				
			
				// XBYA Buttons //////////////////////////////////////////
			case 22:
				// B
				//pad.b = value;
				b.set(value);
				break;
			case 23:
				// X
				//pad.x = value;
				x.set(value);
				break;
			case 24:
				// Y
				//pad.y = value;
				y.set(value);
				break;
			case 21:
				// A
				//pad.a = value;
				a.set(value);
				break;
				
				
				// Shoulders /////////////////////////////////////////////
			case 18:
				// left front shoulder
				//pad.lb = value;
				lb.set(value);
				break;
			case 19:
				// right front shoulder
				//pad.rb = value;
				rb.set(value);
				break;
			case 25:
				// left back shoulder
				pad.lt = value;
				break;
			case 26:
				// right back shoulder
				pad.rt = value;
				break;
				
				
				// Left Analogue /////////////////////////////////////////
			case 27:
				// left analogue horizontal
				pad.leftAnalogX = value;
				break;
			case 28:
				// left analogue vertical
				pad.leftAnalogY = value;
				break;
				
				
				// Right Analogue /////////////////////////////////////////
			case 29:
				// right analogue horizontal
				pad.rightAnalogX = value;
				break;
			case 30:
				// right analogue vertical
				pad.rightAnalogY = value;
				break;
				
			default:
				break;
			
		}
	}
	
};

// ofxXboxGUIControl ///////////////////////////////////////////////////////////////////////

class ofxXboxGUIControl : public ofxBaseObject {

public:
	
	ofxXboxController* xboxController;
	
	int joystickUpdateDelay;
	int nextJoystickUpdate;
	
	int nextTopButtonUpdate;
	
	ofxXboxGUIControl() : ofxBaseObject(){
		joystickUpdateDelay = 100;
		nextJoystickUpdate = ofGetElapsedTimeMillis() + joystickUpdateDelay;
		nextTopButtonUpdate = ofGetElapsedTimeMillis() + joystickUpdateDelay;
	}
	
	void setup(){
		xboxController = Singleton<ofxXboxController>::instance();
		ofAddListener(xboxController->home.onOn, this, &ofxXboxGUIControl::toggleGUI);
		ofAddListener(xboxController->lb.onOn, this, &ofxXboxGUIControl::lbButton);
		ofAddListener(xboxController->rb.onOn, this, &ofxXboxGUIControl::rbButton);
		
		//ofAddListener(xboxController->dUp.onOn, this, &ofxXboxGUIControl::prevFocus);
		//ofAddListener(xboxController->dDown.onOn, this, &ofxXboxGUIControl::nextFocus);
		//ofAddListener(xboxController->a.onOn, this, &ofxXboxGUIControl::keyEnter);
		
		ofAddListener(xboxController->y.onOn, this, &ofxXboxGUIControl::keyUp);
		ofAddListener(xboxController->a.onOn, this, &ofxXboxGUIControl::keyDown);
		ofAddListener(xboxController->x.onOn, this, &ofxXboxGUIControl::keyLeft);
		ofAddListener(xboxController->b.onOn, this, &ofxXboxGUIControl::keyRight);
		
		//ofAddListener(xboxController->dUp.onOn, this, &ofxXboxGUIControl::keyUp);
		//ofAddListener(xboxController->dDown.onOn, this, &ofxXboxGUIControl::keyDown);
		//ofAddListener(xboxController->dLeft.onOn, this, &ofxXboxGUIControl::keyLeft);
		//ofAddListener(xboxController->dRight.onOn, this, &ofxXboxGUIControl::keyRight);
		
		ofAddListener(xboxController->dUp.onOn, &gui, &ofxSimpleGuiToo::incrementScroll);
		ofAddListener(xboxController->dDown.onOn, &gui, &ofxSimpleGuiToo::decrementScroll);
		
		ofAddListener(xboxController->dLeft.onOn, &gui, &ofxSimpleGuiToo::closeAllTitles);
		ofAddListener(xboxController->dRight.onOn, &gui, &ofxSimpleGuiToo::openAllTitles);
		
	}
	
	void update(){
		
		if (gui.isOn()) {
			if (ofGetElapsedTimeMillis() > nextJoystickUpdate) {
				nextJoystickUpdate = ofGetElapsedTimeMillis() + joystickUpdateDelay;
				//gui.currentPage().onJoystick(xboxController->pad.rightAnalogX, xboxController->pad.rightAnalogY, 0);
				float moveFactor = 10;
				float sizeFactor = 10;
				gui.moveBy(xboxController->pad.leftAnalogX * moveFactor, xboxController->pad.leftAnalogY * moveFactor);
				gui.resizeBy(xboxController->pad.rightAnalogX * sizeFactor, xboxController->pad.rightAnalogY * sizeFactor);
			}
			
			if (ofGetElapsedTimeMillis() > nextTopButtonUpdate) {
				nextTopButtonUpdate = ofGetElapsedTimeMillis() + joystickUpdateDelay;
				gui.currentPage().onJoystick(xboxController->pad.rt / 255. - xboxController->pad.lt / 255., 0, 0);
			}
		}
		
	}
	
	void toggleGUI(ofEventArgs& e){
		gui.toggleDraw();
		if (gui.isOn()) {
			gui.setPage(1);
		} else {
			gui.saveToXML();
		}
	 }
	
	void rbButton(ofEventArgs& e){
		if(gui.isOn()){
			if(xboxController->pad.lb)
				gui.setPage(1);
			else
				gui.nextPage();
		}
	}

	void lbButton(ofEventArgs& e){
		if(gui.isOn()){
			if(xboxController->pad.rb)
				gui.setPage(1);
			else
				gui.prevPage();
		}
	}
	
	void nextFocus(ofEventArgs& e){
		if(gui.isOn())
			gui.focusNext();
	}
	
	void prevFocus(ofEventArgs& e){
		if(gui.isOn())
			gui.focusPrev();
	}
	
	void keyLeft(ofEventArgs& e){
		if(gui.isOn())
			gui.currentPage().onKeyLeft();
	}
	
	void keyRight(ofEventArgs& e){
		if(gui.isOn())
			gui.currentPage().onKeyRight();
	}
	
	void keyUp(ofEventArgs& e){
		if(gui.isOn())
			gui.currentPage().onKeyUp();
	}
	
	void keyDown(ofEventArgs& e){
		if(gui.isOn())
			gui.currentPage().onKeyDown();
	}
	
	void keyEnter(ofEventArgs& e){
		if(gui.isOn())
			gui.currentPage().onKeyEnter();
	}
	
};