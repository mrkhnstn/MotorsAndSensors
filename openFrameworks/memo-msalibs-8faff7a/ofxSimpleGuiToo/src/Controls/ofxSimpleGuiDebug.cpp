/*
 *  ofxSimpleGuiDebugBase.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 30/04/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "ofxSimpleGuiDebug.h"

template <typename Type>
ofxSimpleGuiDebugBase<Type>::ofxSimpleGuiDebugBase(string name, Type &value) : ofxSimpleGuiControl(name) {
	this->value = &value;
	controlType = "DebugBase";
	setup();
}

template <typename Type>
void ofxSimpleGuiDebugBase<Type>::setup() {
	setSize(config->gridSize.x - config->padding.x, config->sliderHeight + config->sliderTextHeight);
}

template <typename Type>
void ofxSimpleGuiDebugBase<Type>::draw(float x, float y) {
	
	enabled = true;
	
	//update postion of gui object
	setPos(x, y);
	
	ofEnableAlphaBlending();
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
	
	//setEmptyColor();
	//ofRect(0, 0, width, config->sliderHeight);
	
	
	//setFullColor();
	//ofRect(0, 0, barwidth, config->sliderHeight);
	/*
	 setTextBGColor();
	 ofRect(0, config->sliderHeight, width, config->sliderTextHeight);
	 */
	
	setTextBGColor();
	ofRect(0, 0, width, height);
	
	setTextColor();
	string s = name + ": " + getDebugString();
	ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	ofDisableAlphaBlending();
	glPopMatrix();
}

template <typename Type>
string ofxSimpleGuiDebugBase<Type>::getDebugString(){
	return "";
}

template <typename Type>
Type ofxSimpleGuiDebugBase<Type>::getValue() {
	return (*value);
}

template <typename Type>
void ofxSimpleGuiDebugBase<Type>::set(Type f) {
	(*value) = f;
}

//

template class ofxSimpleGuiDebugBase<int>;
template class ofxSimpleGuiDebugBase<float>;
template class ofxSimpleGuiDebugBase<bool>;
template class ofxSimpleGuiDebugBase<string>;

//

ofxSimpleGuiDebugBool::ofxSimpleGuiDebugBool(string name, bool &value) : ofxSimpleGuiDebugBase<bool>(name, value) {
	controlType = "DebugBool";
}

string ofxSimpleGuiDebugBool::getDebugString(){
	return *value ? "true" : "false";
}

//

ofxSimpleGuiDebugFloat::ofxSimpleGuiDebugFloat(string name, float &value, int pointDigits_) : ofxSimpleGuiDebugBase<float>(name, value) {
	controlType = "DebugFloat";
	pointDigits = pointDigits_;
}

string ofxSimpleGuiDebugFloat::getDebugString(){
	return ofToString(*value,pointDigits);
}

//

ofxSimpleGuiDebugInt::ofxSimpleGuiDebugInt(string name, int &value) : ofxSimpleGuiDebugBase<int>(name, value) {
	controlType = "DebugInt";
}

string ofxSimpleGuiDebugInt::getDebugString(){
	return ofToString(*value);
}

//

ofxSimpleGuiDebugString::ofxSimpleGuiDebugString(string name, string &value) : ofxSimpleGuiDebugBase<string>(name, value) {
	controlType = "DebugString";
}

string ofxSimpleGuiDebugString::getDebugString(){
	return *value;
}