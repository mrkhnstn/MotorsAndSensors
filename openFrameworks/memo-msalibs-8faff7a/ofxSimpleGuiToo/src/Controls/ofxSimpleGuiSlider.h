#pragma once

#include "ofxSimpleGuiControl.h"



template <typename Type> class ofxSimpleGuiSliderBase : public ofxSimpleGuiControl {
public:

	Type		*value;
	Type		defaultValue;
	Type		min, max;

	float		barwidth;
	float		pct;

	float		lerpSpeed;
	Type		targetValue;
	Type		oldValue;
	Type		increment;
	
	bool		keyboardEdit;
	string		keyboardInputString;

	ofEvent<ofEventArgs> onReleased;
	ofEvent<ofEventArgs> onPressed;
	ofEventArgs e;	
	
	ofxSimpleGuiSliderBase(string name, Type &value, Type min, Type max);

	void setup();

	void loadFromXML(ofxXmlSettings &XML);
	
	void setSmoothing(float smoothing);
	
	virtual void setIncrement(Type increment);
	
	void saveToXML(ofxXmlSettings &XML);

	Type getValue();

	void setValue(Type f);
	
	void setTargetValue(Type f);
	
	virtual void setFromString(string& s);
	
	virtual string getString();
	
	virtual void convertKeyboardInputToValue();

	virtual void increase(float incrementFactor = 0.001);

	virtual void decrease(float incrementFactor = 0.001);

	void updateSlider();

	void onPress(int x, int y, int button);

	void onRelease(int x, int y, int button);
	
	void onDragOver(int x, int y, int button);

	void onDragOutside(int x, int y, int button);

	// use x to increment and decrement
	// assuming that x is normalized between -1 and 1
	void onJoystick(float x, float y, int id);

	void onKeyRight();

	void onKeyLeft();
	
	void onKeyUp();
	
	void onKeyDown();
	
	void keyReleased(int key);

	void update();

	virtual void draw(float x, float y);

};

// ofxSimpleGuiSliderFloat ///////////////////////////////////////////

class ofxSimpleGuiSliderFloat : public ofxSimpleGuiSliderBase<float> {
	
public:
	ofxSimpleGuiSliderFloat(string name, float &value, float min, float max);	
	void setFromString(string& s);
	string getString();
};

// ofxSimpleGuiSliderInt ///////////////////////////////////////////

class ofxSimpleGuiSliderInt : public ofxSimpleGuiSliderBase<int> {
	
public:
	
	ofxSimpleGuiSliderInt(string name, int &value, int min, int max);	
	void setFromString(string& s);
	string getString();
	void increase(float incrementFactor);
	void decrease(float incrementFactor);
};

//