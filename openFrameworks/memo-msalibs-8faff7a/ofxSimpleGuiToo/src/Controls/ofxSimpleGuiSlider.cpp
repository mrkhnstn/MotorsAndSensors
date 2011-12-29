/*
 *  ofxSimpleGuiSlider.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 30/04/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiSlider.h"

template <typename Type>
ofxSimpleGuiSliderBase<Type>::ofxSimpleGuiSliderBase(string name, Type &value, Type min, Type max) : ofxSimpleGuiControl(name) {
	this->value = &value;
	defaultValue = value;
	this->min	= min;
	this->max	= max;
	
	targetValue	= value;
	oldValue	= targetValue;
	controlType = "SliderBase";
	
	setIncrement(0);
	setSmoothing(0);
	
	setup();
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::setup() {
	setSize(config->gridSize.x - config->padding.x, config->sliderHeight + config->sliderTextHeight);
	pct		 = ofMap((*value), min, max, 0.0, width);
	barwidth = pct;
	
	keyboardEdit = false;
	keyboardInputString = "";
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::loadFromXML(ofxXmlSettings &XML) {
	if(includeInXml)
		setValue((Type)XML.getValue(controlType + "_" + key + ":value", defaultValue));
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::setSmoothing(float smoothing) {
	lerpSpeed	= 1.0f - smoothing * 0.9;		// so smoothing :1 still results in some motion!
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::setIncrement(Type increment) {
	this->increment = increment;
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::saveToXML(ofxXmlSettings &XML) {
	if(includeInXml){
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		XML.addValue("value", getValue());
		XML.popTag();
	}
}

template <typename Type>
Type ofxSimpleGuiSliderBase<Type>::getValue() {
	return (*value);
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::setValue(Type f) {
	setTargetValue(f);
	oldValue = *value =  targetValue;
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::setTargetValue(Type f) {
	targetValue = ofClamp(f, min, max);
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::setFromString(string& s){}

template <typename Type>
string ofxSimpleGuiSliderBase<Type>::getString(){
	return "";
}	

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::convertKeyboardInputToValue(){
	setFromString(keyboardInputString);
	//keyboardInputString = "";
	keyboardEdit = false;
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::increase(float incrementFactor) {
	//if(increment == 0) setIncrement((max - min) * incrementFactor);
	//		oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
	setIncrement((max - min) * incrementFactor);
	setTargetValue(*value + increment);
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::decrease(float incrementFactor) {
	//if(increment == 0) setIncrement((max - min) * incrementFactor);
	//		oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
	setIncrement((max - min) * incrementFactor);
	setTargetValue(*value - increment);
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::updateSlider() {
	if(!enabled) return;
	
	if(pct > width) {
		pct = width;
	}
	else {
		pct = getMouseX() - x;
		float temp = ofMap(pct, 0.0, width, min, max, true);
		
		targetValue = (Type)temp;
		oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
	}
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onPress(int x, int y, int button) {
	if (button == 0 && !keyboardEdit) {
		ofNotifyEvent(onPressed, e, this);
		updateSlider();
	} else { // on right mouse click toggle keyboard input // if (button == 2) {
		keyboardEdit = !keyboardEdit;
		if (keyboardEdit) {
			keyboardInputString = getString();
		} else {
			convertKeyboardInputToValue();
		}
	}
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onRelease(int x, int y, int button){
	if (button == 0 && !keyboardEdit) {
		ofNotifyEvent(onReleased, e, this);
	}
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onDragOver(int x, int y, int button) {
	if(isMouseDown() && !keyboardEdit && button == 0)
		updateSlider();
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onDragOutside(int x, int y, int button) {
	if(isMouseDown() && !keyboardEdit && button == 0)
		updateSlider();
}

// use x to increment and decrement
// assuming that x is normalized between -1 and 1
template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onJoystick(float x, float y, int id){
	if(!keyboardEdit){
		if (x > 0) {
			increase(ofMap(x, 0, 1, 0, 0.1, true));
		} else if (x < 0){
			decrease(ofMap(x, 0, -1, 0, 0.1, true));
		}
	}
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onKeyRight() {
	if(!keyboardEdit)
		if (isShiftKeyDown()) {
			increase(0.01);
		} else {
			increase(0.001);
		}
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onKeyLeft() {
	if(!keyboardEdit)
		if (isShiftKeyDown()) {
			decrease(0.01);
		} else {
			decrease(0.001);
		}
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onKeyUp() {
	/*
	 if(!keyboardEdit)
	 if (isShiftKeyDown()) {
	 decrease(0.01);
	 } else {
	 decrease(0.001);
	 }
	 */
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::onKeyDown() {
	/*
	 if(!keyboardEdit)
	 if (isShiftKeyDown()) {
	 increase(0.01);
	 } else {
	 increase(0.001);
	 }
	 */
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::keyReleased(int key){
	//ofLog(OF_LOG_VERBOSE, ofToString(key));
	if (keyboardEdit) {
		if (key >= '0' && key <= '9') {
			keyboardInputString.append(1,key);
			//ofLog(OF_LOG_VERBOSE, s);
		} else if (key == '.') {
			keyboardInputString.append(1,key);
		} else if (key == '-' && keyboardInputString.size() == 0) {
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

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::update() {
	if(!enabled) return;
	
	if(oldValue != *value) {					// if value has changed programmatically by something else
		oldValue = targetValue = *value;			// save the value in target and oldvalue
	} else {									// otherwise lerp
		*value += (Type)((targetValue - *value) * lerpSpeed);
		oldValue = *value;							// and save oldvalue
	}
	
	if(lock) {
		updateSlider();
	}
	
	//		enabled = false;
	
}

template <typename Type>
void ofxSimpleGuiSliderBase<Type>::draw(float x, float y) {
	
	//		enabled = true;
	
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
	
	//string s = name + ": " + ofToString((*value));
	//ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	
	
	if (keyboardEdit) {
		ofSetColor(255, 0, 0); //< set text to red while editing with keyboard. TODO: Make this more generic using something like setTextEditColor()
		string s = name + ": " + keyboardInputString;
		ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	} else {
		setTextColor();
		string s = name + ": " + ofToString((*value));
		ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	}
	
	ofDisableAlphaBlending();
	glPopMatrix();
}

//

template class ofxSimpleGuiSliderBase<float>;
template class ofxSimpleGuiSliderBase<int>;

//

ofxSimpleGuiSliderFloat::ofxSimpleGuiSliderFloat(string name, float &value, float min, float max) : ofxSimpleGuiSliderBase<float>(name, value, min, max) {
	controlType = "SliderFloat";
}

void ofxSimpleGuiSliderFloat::setFromString(string& s){
	setValue(ofToFloat(s));
}

string ofxSimpleGuiSliderFloat::getString(){
	return ofToString(*value, 2);
}

//

ofxSimpleGuiSliderInt::ofxSimpleGuiSliderInt(string name, int &value, int min, int max) : ofxSimpleGuiSliderBase<int>(name, value, min, max) {
	controlType = "SliderInt";
	setIncrement(1);
}

void ofxSimpleGuiSliderInt::setFromString(string& s){
	setValue(ofToInt(s));
}

string ofxSimpleGuiSliderInt::getString(){
	return ofToString(*value);
}


void ofxSimpleGuiSliderInt::increase(float incrementFactor) {
	setIncrement((max - min) * incrementFactor);
	if(increment < 1)
		increment = 1;
	setTargetValue(*value + increment);
}

void ofxSimpleGuiSliderInt::decrease(float incrementFactor) {
	setIncrement((max - min) * incrementFactor);
	if(increment < 1)
		increment = 1;
	setTargetValue(*value - increment);
}

//