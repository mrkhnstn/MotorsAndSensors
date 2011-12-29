#pragma once
/*
 *  ofxSimpleGuiTextInput.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 22/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiControl.h"

class ofxSimpleGuiTextInput : public ofxSimpleGuiControl {

public:
	
	string *value;
	string defaultValue;
	
	bool		keyboardEdit;
	string		keyboardInputString;
	
	ofEvent<ofEventArgs> onReleased;
	ofEvent<ofEventArgs> onPressed;
	ofEventArgs e;	
	
	ofxSimpleGuiTextInput(string name, string &value);
	
	void setup();
	
	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);	
	
	string getValue();
	void setValue(string s);
	
	virtual void convertKeyboardInputToValue();
	
	void onPress(int x, int y, int button);		
	void onRelease(int x, int y, int button);	
	
	void disableOtherKeyboardInput();
	void enableOtherKeyboardInput();
	
	void keyReleased(int key);
	
	void draw(float x, float y);
	
};