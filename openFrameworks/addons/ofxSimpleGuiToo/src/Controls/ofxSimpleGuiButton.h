#pragma once

#include "ofxSimpleGuiControl.h"


class ofxSimpleGuiButton : public ofxSimpleGuiControl {
public:
	ofEvent<ofEventArgs> onReleased;
	ofEvent<ofEventArgs> onPressed;
	ofEventArgs e;
	
	bool*			value;
	
	bool			beToggle;
	bool			beenPressed;
	
	bool			selfValue;
	
	ofxSimpleGuiButton(string name, bool &value);
	ofxSimpleGuiButton(string name);
	void setup();
	void loadFromXML(ofxXmlSettings &XML);	
	void saveToXML(ofxXmlSettings &XML);
	void keyPressed( int key );
	bool getValue();
	void set(bool b);
	void toggle();
	void setToggleMode(bool b);
	virtual void onPress(int x, int y, int button);
	virtual void onRelease(int x, int y, int button);
	virtual void draw(float x, float y);
	
	virtual void receiveOscMessage(ofxOscMessage& m){
		set(true);
	}
};
