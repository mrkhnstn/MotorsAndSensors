#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxSimpleGuiSlider.h"
#include "ofxSimpleGuiTitle.h"


class ofxSimpleGuiColorPicker : public ofxSimpleGuiControl {
public:
	
	float		*value;
	float		defaultValues[4];
	float		min, max;
	
	float		barwidth[4];
	float		pct[4];
	
	bool		displayInteger; //< if true then displays values in the gui between 0 and 255
	
	ofxSimpleGuiColorPicker(string name, float *value, float max = 1, bool displayInteger = false);
	void setup();
	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);
	float getValue(int i);
	void setValue(float f, int i);
	void updateSlider();	
	void onPress(int x, int y, int button);
	void onDragOver(int x, int y, int button);
	void onDragOutside(int x, int y, int button);
	void update();
	void draw(float x, float y);
	
};

// COLOR PICKER 2 ///////////////////////////////////////////////

class ofxSimpleGuiColorSlider : public ofxSimpleGuiSliderBase<float> {

public:
	
	bool displayInteger;
	ofColor color;
	
	ofxSimpleGuiColorSlider(string name, float &value, float min, float max, bool displayInteger = true);	
	void setFromString(string& s);
	string getString();
	void draw(float x, float y);
	
};

//

class ofxSimpleGuiColorTitle : public ofxSimpleGuiTitle {
	
public:
	float *value;
	bool isInteger;
	
	ofxSimpleGuiColorTitle(string name, float height);
	void draw(float x, float y);
};

//

class ofxSimpleGuiColorPicker2 : public ofxSimpleGuiControl {
public:
	
	float		*value;
	float		defaultValues[4];
	float		min, max;
	
	bool		displayInteger; //< if true then displays values in the gui between 0 and 255

	ofxSimpleGuiColorTitle* title;
	ofxSimpleGuiColorSlider* sliders[4];
	int focus;
	
	ofxSimpleGuiColorPicker2(string name, float *value, float max = 1, bool displayInteger = false);
	~ofxSimpleGuiColorPicker2();
	void setup();
	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);
	float getValue(int i);
	void setValue(float f, int i);	
	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void onDragOver(int x, int y, int button);
	void onDragOutside(int x, int y, int button);
	void onJoystick(float x, float y, int id);
	void onKeyRight();
	void onKeyLeft();
	void update();
	void draw(float x, float y);
	
	void _mouseMoved(ofMouseEventArgs &e);
	void _mousePressed(ofMouseEventArgs &e);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);
	void _keyPressed(ofKeyEventArgs &e);
	void _keyReleased(ofKeyEventArgs &e);

	/** returns true if control focused an control internally */
	bool focusNextControl();
	
	/** returns true if control focused an control internally */
	bool focusPrevControl();
	
	void setFocus(bool newFocus);

};

//

