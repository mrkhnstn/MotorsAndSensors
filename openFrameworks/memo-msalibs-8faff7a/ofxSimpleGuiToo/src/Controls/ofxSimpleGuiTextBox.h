#pragma once
/*
 *  ofxSimpleGuiTextBox.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 06/05/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiControl.h"
#include "ofxTextSuite.h"
#include "ofxFBOTexture.h"

class ofxSimpleGuiTextBox : public ofxSimpleGuiControl {
	
public:
	
	bool*					value;
	bool					beToggle;
	bool					beenPressed;
	static ofTrueTypeFont	font;
	ofxTextBlock			textBlock;
	float					expandedHeight;
	float					closedHeight;
	ofxFBOTexture*			fbo;
	
	ofxSimpleGuiTextBox(string name, float height);
	void setup();
	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);
	bool getValue();
	void setValue(bool b);
	void toggle();
	void setToggleMode(bool b);
	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void onKeyEnter();
	void onKeyRight();	
	void onKeyLeft();
	virtual void draw(float x, float y);
	
};

