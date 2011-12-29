#pragma once

#include "ofxSimpleGuiControl.h"

#include "ofxSimpleGuiSliderBase.h"


class ofxSimpleGuiSliderFloat : public ofxSimpleGuiSliderBase<float> {
	
public:
	ofxSimpleGuiSliderFloat(string name, float &value, float min, float max, float smoothing = 0) : ofxSimpleGuiSliderBase<float>(name, value, min, max, smoothing) {
		controlType = "SliderFloat";
	}
	
	void receiveOscMessage(ofxOscMessage& m){
		if( m.getArgType(0) == OFXOSC_TYPE_FLOAT ){
			set(m.getArgAsFloat(0));
		}
	}
	
	void createOscArgs(ofxOscMessage& m){
		m.addFloatArg(*value);
	}
	
	void setFromString(string& s){
		set(ofToFloat(s));
	}
	
	string getString(){
		return ofToString(*value, 2);
	}
};
