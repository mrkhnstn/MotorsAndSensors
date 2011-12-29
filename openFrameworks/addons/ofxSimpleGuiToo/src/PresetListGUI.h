#pragma once
/*
 *  PresetListGUI.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 10/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiToo.h"
#include "ofEvents.h"

#define AUTO_RESTORE_NONE 			0
#define AUTO_RESTORE_INT 			1
#define AUTO_RESTORE_FLOAT			2 // interpolated
#define AUTO_RESTORE_FLOAT_ANIMATED	3 // interpolated and animated

class PresetListGUI : public ofxSimpleGuiPresetList {

public: 
	
	bool doStore;
	int restoreMode;
	int currentPresetInt;
	float currentPresetFloat;
	float targetPresetFloat;
	float presetVel;
	string restoreModeNames[4];// {"None","Int","Float","Animated"};
	string lastMsg;
	bool drawEnabled;
	
	PresetListGUI() : ofxSimpleGuiPresetList() {
		doStore = true;
		drawEnabled = false;
		restoreMode = AUTO_RESTORE_NONE;
		currentPresetInt = 0;
		currentPresetFloat = currentPresetInt;
		targetPresetFloat = currentPresetInt;
		presetVel = 0.05;
		restoreModeNames[0] = "None";
		restoreModeNames[1] = "Int";
		restoreModeNames[2] = "Float";
		restoreModeNames[3] = "Animated";
		lastMsg = "";
	}
	
	void setupGUI(string name=""){
		gui.addSlider(name+"Preset", currentPresetInt, 0, 9);
		gui.addSlider(name+"Interpolated", currentPresetFloat, 0, 9);
		gui.addSlider(name+"InterpolatedTarget", targetPresetFloat, 0, 9);
		gui.addSlider(name+"InterpolationSpeed", presetVel, 0, 0.1);
		gui.addSlider(name+"Restore Mode", restoreMode, AUTO_RESTORE_NONE, AUTO_RESTORE_FLOAT_ANIMATED);
		gui.addButton(name+"Store", doStore);
		gui.addToggle(name+"Draw Enabled", drawEnabled);
	}
	
	void draw(){
		if (drawEnabled) {
			ofSetColor(128, 128, 128);
			ofDrawBitmapString("Mode: " + restoreModeNames[restoreMode] + " | current: " + ofToString(currentPresetInt) + " | " + lastMsg, 10, 10);
			ofSetColor(255, 255, 255);
		}
	}
	
	void enable(){
	    ofAddListener(ofEvents.update, this, &PresetListGUI::update);
	}
	
	void disable(){
	    ofRemoveListener(ofEvents.update, this, &PresetListGUI::update);
	}
	
	void enableKeyboard(){
		ofAddListener(ofEvents.keyPressed, this, &PresetListGUI::keyPressed);
	}
	
	void disableKeyboard(){
		ofRemoveListener(ofEvents.keyPressed, this, &PresetListGUI::keyPressed);
	}
	
	void restore(int i){
		currentPresetInt = i;
		targetPresetFloat = currentPresetInt;
		if (restoreMode == AUTO_RESTORE_NONE) {
			restorePreset(currentPresetInt);
			currentPresetFloat = currentPresetInt;
		} else if (restoreMode == AUTO_RESTORE_INT) {
			restorePreset(currentPresetInt);
			currentPresetFloat = currentPresetInt;
		} else if (restoreMode == AUTO_RESTORE_FLOAT) {
			restorePreset(currentPresetInt);
			currentPresetFloat = currentPresetInt;
		} else if (restoreMode == AUTO_RESTORE_FLOAT_ANIMATED) {
			
		}
		lastMsg = "restored " + ofToString(currentPresetInt);
		//ofLog(OF_LOG_VERBOSE,"preset restore: "+ofToString(currentPresetInt));
	}
	
	void store(int i){
		currentPresetInt = i;
		targetPresetFloat = currentPresetInt;
		currentPresetFloat = currentPresetInt;
		storePreset(currentPresetInt);
		lastMsg = "stored " + ofToString(currentPresetInt);
		//ofLog(OF_LOG_VERBOSE,"preset store: "+ofToString(currentPresetInt));
	}
	
	void keyPressed(ofKeyEventArgs& args){

		if(args.key>='0' && args.key<='9') {
			if (checkControlKey()) { // store using CTRL + 0..9
				store(args.key - '0');
			} else if (checkAltKey()){ // restore using ALT + 0..9
				restore(args.key - '0');
			}
		} else {
			switch(args.key) {
					/*
				case '-':
					restoreMode--;
					if (restoreMode < 0) {
						restoreMode = AUTO_RESTORE_FLOAT_ANIMATED;
					}
					break;
					 */
				case '=':
					restoreMode++;
					if (restoreMode > AUTO_RESTORE_FLOAT_ANIMATED) {
						restoreMode = 0;
					}
					break;
			}
		}
	}
	
	bool checkShiftKey(){
		return (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
	};
	bool checkControlKey(){
		return (glutGetModifiers() & GLUT_ACTIVE_CTRL);
	};
	bool checkAltKey(){
		return (glutGetModifiers() & GLUT_ACTIVE_ALT);
	};
	
	/*
	void update(ofEventArgs& args){
		update();
	}
	*/
		
	
	
	void update(ofEventArgs& args){
		if (doStore) {
			storePreset(currentPresetInt);
			doStore = false;	
		}
		
		if (restoreMode == AUTO_RESTORE_INT) {
			restorePreset(currentPresetInt);
			currentPresetFloat = currentPresetInt;
		} else if (restoreMode == AUTO_RESTORE_FLOAT) {
			currentPresetInt = roundf(currentPresetFloat);
			restorePreset(currentPresetFloat);
		} else if (restoreMode == AUTO_RESTORE_FLOAT_ANIMATED) {
			if (targetPresetFloat > currentPresetFloat) {
				currentPresetFloat += presetVel;
				if (currentPresetFloat > targetPresetFloat) {
					currentPresetFloat = targetPresetFloat;
				}
			} else 	if (targetPresetFloat < currentPresetFloat) {
				currentPresetFloat -= presetVel;
				if (currentPresetFloat < targetPresetFloat) {
					currentPresetFloat = targetPresetFloat;
				}
			}
			currentPresetInt = roundf(currentPresetFloat);
			restorePreset(currentPresetFloat);
		}
	}
	
};
