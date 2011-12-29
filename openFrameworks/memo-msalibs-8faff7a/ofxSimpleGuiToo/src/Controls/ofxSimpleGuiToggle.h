#pragma once

#include "ofxSimpleGuiControl.h"


class ofxSimpleGuiToggle : public ofxSimpleGuiControl {
	
public:
	bool	*value;
	bool	defaultValue;
	bool	momentary;
	
	ofEvent<ofEventArgs> onReleased;
	ofEvent<ofEventArgs> onPressed;
	ofEventArgs e;
	
	ofxSimpleGuiToggle(string name, bool &value);
	ofxSimpleGuiToggle& setMomentary(bool m);
	void setup();
	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);
	bool getValue();
	void setValue(bool b);
	void toggle();
	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void keyPressed( int key );
	void keyReleased( int key );
	void onKeyEnter();
	void onKeyRight();
	void onKeyLeft();
	void update();
	void draw(float x, float y);
};
