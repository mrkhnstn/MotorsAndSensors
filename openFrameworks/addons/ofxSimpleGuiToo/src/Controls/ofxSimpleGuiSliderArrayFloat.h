#pragma once

#include "ofxSimpleGuiControl.h"

#include "ofxSimpleGuiSliderArrayBase.h"


class ofxSimpleGuiSliderArrayFloat : public ofxSimpleGuiSliderArrayBase<float> {
	
public:
	ofxSimpleGuiSliderArrayFloat(string name, float &value, float min, float max, float smoothing = 0) : ofxSimpleGuiSliderArrayBase<float>(name, value, min, max, smoothing) {
		controlType = "SliderArrayFloat";
	}
	
	virtual void receiveOscMessage(ofxOscMessage& m){
		if( m.getArgType(0) == OFXOSC_TYPE_FLOAT ){
			set(m.getArgAsFloat(0));
		}
	}
	
	virtual void createOscArgs(ofxOscMessage& m){
		m.addFloatArg(*value);
	}
	
	void setFromString(string& s){
		set(ofToFloat(s));
	}
	
	string getString(){
		return ofToString(*value, 2);
	}
};
