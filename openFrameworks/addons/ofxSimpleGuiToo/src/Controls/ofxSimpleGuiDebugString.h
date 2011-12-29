#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxSimpleGuiDebugBase.h"


class ofxSimpleGuiDebugString : public ofxSimpleGuiDebugBase<string> {
	
public:
	
	ofxSimpleGuiDebugString(string name, string &value) : ofxSimpleGuiDebugBase<string>(name, value) {
		controlType = "DebugString";
	}
	
	virtual string getDebugString(){
		return *value;
	}
};