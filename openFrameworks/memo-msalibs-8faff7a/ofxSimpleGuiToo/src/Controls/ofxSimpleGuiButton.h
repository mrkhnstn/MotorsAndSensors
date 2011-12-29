#pragma once

#include "ofxSimpleGuiValueControl.h"


class ofxSimpleGuiButton : public ofxSimpleGuiValueControl<bool> {
public:
	bool			beToggle;
	bool			beenPressed;
	
	ofEvent<ofEventArgs> onReleased;
	ofEvent<ofEventArgs> onPressed;
	ofEventArgs e;
	bool selfValue;
	
	ofxSimpleGuiButton(string name);	
	ofxSimpleGuiButton(string name, bool &value);	
	void setup();
	
	void loadFromXML(ofxXmlSettings &XML);	
	void saveToXML(ofxXmlSettings &XML);
	void keyPressed( int key );

	bool getValue();
	void setValue(bool b);
	void toggle();
	
	void setToggleMode(bool b);
	
	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void onKeyEnter();
	void onKeyRight();	
	void onKeyLeft();
	void draw(float x, float y);
	
};
