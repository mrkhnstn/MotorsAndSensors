#pragma once

#include "ofxSimpleGuiControl.h"

template <typename Type> 
class ofxSimpleGuiDebugBase : public ofxSimpleGuiControl {
public:
	
	Type		*value;
	float		barwidth;
	float		pct;
	
	ofxSimpleGuiDebugBase(string name, Type &value);
	void setup();	
	Type getValue();
	void set(Type f);
	void draw(float x, float y);
	virtual string getDebugString();	
};

// ofxSimpleGuiDebugBool /////////////////////////////////////////////

class ofxSimpleGuiDebugBool : public ofxSimpleGuiDebugBase<bool> {
	
public:
	
	ofxSimpleGuiDebugBool(string name, bool &value);
	virtual string getDebugString();
	
};

// ofxSimpleGuiDebugFloat /////////////////////////////////////////////

class ofxSimpleGuiDebugFloat : public ofxSimpleGuiDebugBase<float> {
	
public:
	
	int pointDigits;
	
	ofxSimpleGuiDebugFloat(string name, float &value, int pointDigits_=2);	
	virtual string getDebugString();
};

// ofxSimpleGuiDebugInt /////////////////////////////////////////////

class ofxSimpleGuiDebugInt : public ofxSimpleGuiDebugBase<int> {
	
public:
	
	ofxSimpleGuiDebugInt(string name, int &value);	
	virtual string getDebugString();};

// ofxSimpleGuiDebugString /////////////////////////////////////////////

class ofxSimpleGuiDebugString : public ofxSimpleGuiDebugBase<string> {
	
public:
	
	ofxSimpleGuiDebugString(string name, string &value);	
	virtual string getDebugString();
};