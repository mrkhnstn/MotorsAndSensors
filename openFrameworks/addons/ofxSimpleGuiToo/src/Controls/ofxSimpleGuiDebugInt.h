#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxSimpleGuiDebugBase.h"


class ofxSimpleGuiDebugInt : public ofxSimpleGuiDebugBase<int> {
	
public:
	
	ofxSimpleGuiDebugInt(string name, int &value) : ofxSimpleGuiDebugBase<int>(name, value) {
		controlType = "DebugInt";
	}
	
	virtual string getDebugString(){
		return ofToString(*value);
	}
};