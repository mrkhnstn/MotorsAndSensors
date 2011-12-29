#pragma once

#include "ofxSimpleGuiControl.h"



template <typename Type> class ofxSimpleGuiDebugBase : public ofxSimpleGuiControl {
public:
	
	Type		*value;
	
	float		barwidth;
	float		pct;
	
	float		lerpSpeed;
	Type		targetValue;
	Type		oldValue;
	
	//--------------------------------------------------------------------- construct
	ofxSimpleGuiDebugBase(string name, Type &value) : ofxSimpleGuiControl(name) {
		this->value = &value;
		controlType = "DebugBase";
		setup();
	}
	
	void setup() {
		setSize(config->gridSize.x - config->padding.x, config->sliderHeight + config->sliderTextHeight);
		//pct		 = ofMap((*value), min, max, 0.0, width);
		//barwidth = pct;
	}
	
	void loadFromXML(ofxXmlSettings &XML) {
		//set((Type)XML.getValue("controls:" + controlType + "_" + key + ":value", 0.0f));
	}
	
	void saveToXML(ofxXmlSettings &XML) {
		/*
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		XML.addValue("value", getValue());
		XML.popTag();
		*/
	}
	
	
	
	Type getValue() {
		return (*value);
	}
	
	
	void set(Type f) {
		(*value) = f;
	}
		
	void onPress(int x, int y, int button) {
	}
	
	void onDragOver(int x, int y, int button) {
	}
	
	void onDragOutside(int x, int y, int button) {
	}
	
	void onKeyRight() {
	}
	
	void onKeyLeft() {
	}
	
	
	//--------------------------------------------------------------------- update
	void update() {
	}
	
	//--------------------------------------------------------------------- draw
	void draw(float x, float y) {
		
		enabled = true;
		
		//update postion of gui object
		setPos(x, y);
		
		ofEnableAlphaBlending();
		glPushMatrix();
		glTranslatef(x, y, 0);
		ofFill();
		
		setEmptyColor();
		ofRect(0, 0, width, config->sliderHeight);
		
		
		//setFullColor();
		//ofRect(0, 0, barwidth, config->sliderHeight);
		
		setTextBGColor();
		ofRect(0, config->sliderHeight, width, config->sliderTextHeight);
		
		setTextColor();
		string s = name + ": " + getDebugString();
		ofDrawBitmapString(s, 3, config->sliderHeight + 14);
		ofDisableAlphaBlending();
		glPopMatrix();
	}
	
	virtual string getDebugString(){
		return "";
	}
	
};
