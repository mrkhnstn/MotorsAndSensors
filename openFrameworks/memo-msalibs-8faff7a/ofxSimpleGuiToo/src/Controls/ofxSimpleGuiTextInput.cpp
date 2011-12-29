/*
 *  ofxSimpleGuiTextInput.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 22/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiTextInput.h"
#include "ofxSimpleGuiToo.h"

ofxSimpleGuiTextInput::ofxSimpleGuiTextInput(string name, string &value) : ofxSimpleGuiControl(name) {
	this->value = &value;	
	defaultValue = value;
	controlType = "TextInput";
	setup();
}

void ofxSimpleGuiTextInput::disableOtherKeyboardInput(){
	gui.disableKeyboardShortcuts();
}

void ofxSimpleGuiTextInput::enableOtherKeyboardInput(){
	gui.enableKeyboardShortcuts();
}

void ofxSimpleGuiTextInput::setup() {
	setSize(config->gridSize.x - config->padding.x, config->buttonHeight);
	keyboardEdit = false;
	keyboardInputString = "";
}

void ofxSimpleGuiTextInput::loadFromXML(ofxXmlSettings &XML) {
	if(includeInXml)
		setValue(XML.getValue(controlType + "_" + key + ":value", defaultValue));
}

void ofxSimpleGuiTextInput::saveToXML(ofxXmlSettings &XML) {
	if(includeInXml){
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		XML.addValue("value", getValue());
		XML.popTag();
	}
}	

string ofxSimpleGuiTextInput::getValue() {
	return (*value);
}

void ofxSimpleGuiTextInput::setValue(string s) {
	(*value) = s;
}

void ofxSimpleGuiTextInput::convertKeyboardInputToValue(){
	*value = keyboardInputString;
	keyboardEdit = false;
	enableOtherKeyboardInput();
}

void ofxSimpleGuiTextInput::onPress(int x, int y, int button) {
	keyboardEdit = !keyboardEdit;
	if (keyboardEdit) {
		disableOtherKeyboardInput();
		keyboardInputString = *value;
	} else {
		convertKeyboardInputToValue();
	}
}

void ofxSimpleGuiTextInput::disableOtherKeyboardInput();

void ofxSimpleGuiTextInput::enableOtherKeyboardInput();

void ofxSimpleGuiTextInput::onRelease(int x, int y, int button){
	ofNotifyEvent(onReleased, e, this);
}	

void ofxSimpleGuiTextInput::keyReleased(int key){
	ofLog(OF_LOG_VERBOSE, ofToString(key));
	if (keyboardEdit) {
		if (key == 13 || key == 3) { // on enter convert
			convertKeyboardInputToValue();
		} else if (key == 127) { // backspace
			if (keyboardInputString.size() > 0) {
				keyboardInputString.resize(keyboardInputString.size()-1, '0');
			}
			//ofLog(OF_LOG_VERBOSE, s);
		} else {
			keyboardInputString.append(1,key);
		}
	}
	
}

void ofxSimpleGuiTextInput::draw(float x, float y) {
	setPos(x, y);
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	ofEnableAlphaBlending();
	ofFill();
	setTextBGColor();
	ofRect(0, 0, width, height);
	
	if (keyboardEdit) {
		ofSetColor(255, 0, 0); //< set text to red while editing with keyboard. TODO: Make this more generic using something like setTextEditColor()
		ofDrawBitmapString(name + " []: " + keyboardInputString, 3, 15);
	} else {
		setTextColor();
		ofDrawBitmapString(name + " []: " + *value, 3, 15);
	}
	
	setTextColor();
	
	ofDisableAlphaBlending();
	
	glPopMatrix();
}
