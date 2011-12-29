#pragma once

#include "ofxSimpleGuiControl.h"


class ofxSimpleGuiToggle : public ofxSimpleGuiControl {
	
public:
	bool	*value;
	bool	momentary;
	
	ofxSimpleGuiToggle(string name, bool &value);
	ofxSimpleGuiToggle& setMomentary(bool m);
	void setup();
	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);
	bool getValue();
	void set(bool b);
	void toggle();
	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void keyPressed( int key );
	void keyReleased( int key );
	void onKeyEnter();
	void update();
	void draw(float x, float y);
	
	virtual void receiveOscMessage(ofxOscMessage& m){
		if( m.getArgType(0) == OFXOSC_TYPE_INT32 ){
			set(m.getArgAsInt32(0) == 1);
		}
	}
	
	virtual void createOscArgs(ofxOscMessage& m){
		m.addIntArg(*value ? 1 : 0);
	}
};
