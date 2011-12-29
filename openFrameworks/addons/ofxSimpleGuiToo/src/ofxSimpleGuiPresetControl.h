#pragma once
/*
 *  ofxSimpleGuiPresetControl.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 07/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <set>

class ofxSimpleGuiPresetControl {
	
public:
	
	/**
	 interpolates between slot 1 and slot 2
	 */
	virtual void restorePreset(int slotId1, int slotId2, float f){
	}
	
	// restores slot using interpolation
	virtual void restorePreset(float f){
	}
	
	virtual void restorePreset(int i){
	}
	
	virtual void storePreset(int i){
	}
	
};

class ofxSimpleGuiPresetList : public ofxSimpleGuiPresetControl{

public:
	
	set<ofxSimpleGuiPresetControl*> presets;
	
	virtual void restorePreset(int slotId1, int slotId2, float f){
		for (set<ofxSimpleGuiPresetControl*>::iterator it = presets.begin(); it != presets.end(); ++it) {
			ofxSimpleGuiPresetControl* preset = *it;
			preset->restorePreset(slotId1,slotId2,f);
		}
	}
	
	virtual void restorePreset(float f){
		for (set<ofxSimpleGuiPresetControl*>::iterator it = presets.begin(); it != presets.end(); ++it) {
			ofxSimpleGuiPresetControl* preset = *it;
			preset->restorePreset(f);
		}
	}
	
	virtual void restorePreset(int i){
		for (set<ofxSimpleGuiPresetControl*>::iterator it = presets.begin(); it != presets.end(); ++it) {
			ofxSimpleGuiPresetControl* preset = *it;
			preset->restorePreset(i);
		}
	}
	
	virtual void storePreset(int i){
		for (set<ofxSimpleGuiPresetControl*>::iterator it = presets.begin(); it != presets.end(); ++it) {
			ofxSimpleGuiPresetControl* preset = *it;
			preset->storePreset(i);
		}
	}
	
	virtual void addPreset(ofxSimpleGuiPresetControl& preset){
		presets.insert(&preset);
	}
	
	virtual void removePreset(ofxSimpleGuiPresetControl& preset){
		set<ofxSimpleGuiPresetControl*>::iterator it = presets.find(&preset);
		if(it != presets.end())
			presets.erase(it);
	}
};