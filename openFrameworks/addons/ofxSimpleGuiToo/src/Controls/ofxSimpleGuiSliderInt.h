#pragma once

#include "ofxSimpleGuiControl.h"

#include "ofxSimpleGuiSliderBase.h"

class ofxSimpleGuiSliderInt : public ofxSimpleGuiSliderBase<int> {
	
public:
	ofxSimpleGuiSliderInt(string name, int &value, int min, int max, float smoothing = 0) : ofxSimpleGuiSliderBase<int>(name, value, min, max, smoothing) {
		controlType = "SliderInt";
	}

	virtual void receiveOscMessage(ofxOscMessage& m){
		if( m.getArgType(0) == OFXOSC_TYPE_INT32 ){
			set(m.getArgAsInt32(0));
		}
	}

	virtual void createOscArgs(ofxOscMessage& m){
		m.addIntArg(*value);
	}

	void setFromString(string& s){
		set(ofToInt(s));
	}
	
	string getString(){
		return ofToString(*value);
	}
	
	
};
