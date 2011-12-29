#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxSimpleGuiDebugBase.h"


class ofxSimpleGuiDebugBool : public ofxSimpleGuiDebugBase<bool> {
	
public:
	
	ofxSimpleGuiDebugBool(string name, bool &value) : ofxSimpleGuiDebugBase<bool>(name, value) {
		controlType = "DebugBool";
	}
	
	virtual string getDebugString(){
		return *value ? "true" : "false";
	}
};