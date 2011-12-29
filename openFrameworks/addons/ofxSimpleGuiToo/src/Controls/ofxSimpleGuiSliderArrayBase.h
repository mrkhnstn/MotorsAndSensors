/*
 *  ofxSimpleGuiSliderArrayBase.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 06/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxSimpleGuiPresetControl.h"



template <typename Type> class ofxSimpleGuiSliderArrayBase : public ofxSimpleGuiControl, public ofxSimpleGuiPresetControl {
public:
	
	Type		*value;
	Type		min, max;
	
	float		barwidth;
	float		pct;
	
	float		lerpSpeed;
	Type		targetValue;
	Type		oldValue;
	
	bool		keyboardEdit;
	string		keyboardInputString;
	
	vector<Type> slot;
	
	//--------------------------------------------------------------------- construct
	ofxSimpleGuiSliderArrayBase(string name, Type &value, Type min, Type max, float smoothing = 0) : ofxSimpleGuiControl(name) {
		this->value = &value;
		this->min	= min;
		this->max	= max;
		
		lerpSpeed	= 1.0f - smoothing * 0.99;		// so smoothing :1 still results in some motion!
		targetValue	= value;
		oldValue	= targetValue;
		controlType = "SliderBase";
		
		keyboardEdit = false;
		keyboardInputString = "";
		
		setup();
	}
	
	void setup() {
		setSize(config->gridSize.x - config->padding.x, config->sliderHeight + config->sliderTextHeight);
		pct		 = ofMap((*value), min, max, 0.0, width);
		barwidth = pct;
	}
	
	void loadFromXML(ofxXmlSettings &XML) {
		set((Type)XML.getValue("controls:" + controlType + "_" + key + ":value", 0.0f));
		
		
		XML.pushTag("controls");
		XML.pushTag(controlType + "_" + key);
		int n = XML.getNumTags("slot");
		for (int i=0; i<n; i++) {
			storeSlot(i,(Type)XML.getValue("slot",0.0,i));
		}
		XML.popTag();
		XML.popTag();
		
	}
	
	void saveToXML(ofxXmlSettings &XML) {
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		XML.addValue("value", getValue());
		for (int i=0; i<slot.size(); i++) {
			XML.addValue("slot", getSlot(i));
		}
		XML.popTag();  
	}
	
	Type getValue() {
		return (*value);
	}
	
	Type getSlot(int i){
		return i < slot.size() ? slot[i] : (*value);
	}
	
	Type getSlot(float f){
		if(slot.size() == 0){
			return (*value);
		} else {
			float clamped = ofClamp(f, 0, slot.size()-1);
			if (clamped == 0) {
				return slot[0];
			} else if (clamped == slot.size()-1) {
				return slot[slot.size()-1]; 
			} else {
				int smallerSlot = floorf(clamped);
				int largerSlot = smallerSlot + 1;
				float delta = clamped - smallerSlot;
				return (Type)ofMap(clamped, smallerSlot, largerSlot, getSlot(smallerSlot), getSlot(largerSlot));
			}
		}
	}
	
	virtual void restorePreset(int slotId1, int slotId2, float f){
		return set(ofMap(f, 0, 1, getSlot(slotId1), getSlot(slotId2)));
	}
	
	virtual void restorePreset(float f){
		set(getSlot(f));
	}
	
	virtual void restorePreset(int i){
		if(i>=0 && i<slot.size()){
			set(slot[i]);
		}
	}
	
	virtual void storePreset(int i){
		storeSlot(i, getValue());
	}
	
	void storeSlot(int i, Type value_){
		if (i < slot.size()) {
			slot[i] = value_;
		} else {
			int toAdd = i-slot.size()+1;
			for(int j=0; j<toAdd; j++){
				slot.push_back(value_);
			}
		}
	}
	
	void set(Type f) {
		if(f < min) f = min;
		else if(f > max) f = max;
		(*value) = f;
		sendOsc();
	}
	
	void add(float addValue = .05f) {
		pct += addValue;
		float temp = ofMap(pct, 0.0, width, min, max);
		
		//VALUE CLAMP
		if(temp >= max)			temp = max;
		else if(temp <= min)	temp = min;
		
		targetValue = (Type)temp;
		oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
	}
	
	void sub(float subValue = .05f) {
		pct -= subValue;
		float temp = ofMap(pct, 0.0, width, min, max);
		
		//VALUE CLAMP
		if(temp >= max)			temp = max;
		else if(temp <= min)	temp = min;
		
		targetValue = (Type)temp;
		oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
	}
	
	void keyPressed(int key){
		if (isMouseOver() & !keyboardEdit) {
			switch (key) {
				case OF_KEY_LEFT:
					if(checkShiftKey()){
						sub(0.5f);
					} else {
						sub(0.05f);
					}
					break;
				case OF_KEY_RIGHT:
					if(checkShiftKey()){
						add(0.5f);
					} else {
						add(0.05f);
					}
					break;
				default:
					break;
			}
		}
	}
	
	void keyReleased(int key){
		//ofLog(OF_LOG_VERBOSE, ofToString(key));
		if (keyboardEdit) {
			if (key >= '0' && key <= '9') {
				keyboardInputString.append(1,key);
				//ofLog(OF_LOG_VERBOSE, s);
			} else if (key == '.') {
				keyboardInputString.append(1,key);
			} else if (key == 13 || key == 3) { // on enter convert
				convertKeyboardInputToValue();
			} else if (key == 127) { // backspace
				if (keyboardInputString.size() > 0) {
					keyboardInputString.resize(keyboardInputString.size()-1, '0');
				}
				//ofLog(OF_LOG_VERBOSE, s);
			} else {
				//ofLog(OF_LOG_VERBOSE, ofToString(key));
			}
		}
		
	}
	
	virtual void convertKeyboardInputToValue(){
		setFromString(keyboardInputString);
		//keyboardInputString = "";
		keyboardEdit = false;
	}
	
	virtual void setFromString(string& s){}
	
	virtual string getString(){
		return "";
	}
	
	void disable(){
		keyboardEdit = false;
	}
	
	
	void updateSlider() {
		if(!enabled) return;
		
		if(pct > width) {
			pct = width;
		}
		else {
			pct = getMouseX() - x;
			float temp = ofMap(pct, 0.0, (float)width, min, max);
			
			//VALUE CLAMP
			if(temp >= max)			temp = max;
			else if(temp <= min)	temp = min;
			
			targetValue = (Type)temp;
			oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
		}
	}
	
	void onPress(int x, int y, int button) {
		//ofLog(OF_LOG_VERBOSE, "onPress:"+ofToString(button));
		if (button == 0 && !keyboardEdit) {
			updateSlider();
		} else { // on right mouse click toggle keyboard input // if (button == 2) {
			keyboardEdit = !keyboardEdit;
			if (keyboardEdit) {
				keyboardInputString = getString();
			} else {
				convertKeyboardInputToValue();
			}
		}
		//dragging = true;
	}
	
	void onDragOver(int x, int y, int button) {
		
		if(isMouseDown() && !keyboardEdit && button == 0){
			//ofLog(OF_LOG_VERBOSE, "onDragOver:"+name);
			updateSlider();
		}
	}
	
	void onDragOutside(int x, int y, int button) {
		if (isMouseDown() && !keyboardEdit && button == 0) {
			//ofLog(OF_LOG_VERBOSE, "onDragOutside:"+name);
			updateSlider();
		}
	}
	
	
	
	void onKeyRight() {
		add();
	}
	
	void onKeyLeft() {
		sub();
	}
	
	
	//--------------------------------------------------------------------- update
	void update() {
		if(!enabled) return;
		
		if(lock) {
			updateSlider();
		}
		
		enabled = false;
		
	}
	
	//--------------------------------------------------------------------- draw
	void draw(float x, float y) {
		
		enabled = true;
		
		if(oldValue != *value) {					// if value has changed programmatically by something else
			oldValue = targetValue = *value;			// save the value in target and oldvalue
		} else {									// otherwise lerp
			*value += (Type)((targetValue - *value) * lerpSpeed);
			oldValue = *value;							// and save oldvalue
		}
		
		//update postion of gui object
		setPos(x, y);
		
		//VALUE CLAMP
		barwidth = ofMap((*value), min, max, 0.0, (float)width);
		if(barwidth > width) barwidth = width;
		else if(barwidth < 0) barwidth = 0;
		
		ofEnableAlphaBlending();
		glPushMatrix();
		glTranslatef(x, y, 0);
		ofFill();
		
		setEmptyColor();
		ofRect(0, 0, width, config->sliderHeight);
		
		
		setFullColor();
		ofRect(0, 0, barwidth, config->sliderHeight);
		
		setTextBGColor();
		ofRect(0, config->sliderHeight, width, config->sliderTextHeight);
		
		/*
		setTextColor();
		string s = "[P] " + name + ": " + ofToString((*value));
		ofDrawBitmapString(s, 3, config->sliderHeight + 14);
		*/
		
		
		if (keyboardEdit) {
			glColor4f(1,0,0,1); // set to red if keyboard edit
			string s = "[P] " + name + ": " + keyboardInputString;
			ofDrawBitmapString(s, 3, config->sliderHeight + 14);
		} else {
			setTextColor();
			string s = "[P] " + name + ": " + ofToString((*value));
			ofDrawBitmapString(s, 3, config->sliderHeight + 14);
		}
		
		ofDisableAlphaBlending();
		glPopMatrix();
	}
	
	
	
};
