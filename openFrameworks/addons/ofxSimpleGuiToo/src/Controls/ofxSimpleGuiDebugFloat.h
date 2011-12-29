#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxSimpleGuiDebugBase.h"


class ofxSimpleGuiDebugFloat : public ofxSimpleGuiDebugBase<float> {
	
public:
	
	int pointDigits;
	
	ofxSimpleGuiDebugFloat(string name, float &value, int pointDigits_=2) : ofxSimpleGuiDebugBase<float>(name, value) {
		controlType = "DebugFloat";
		pointDigits = pointDigits_;
	}
	
	virtual string getDebugString(){
		return ofToString(*value,pointDigits);
	}
};