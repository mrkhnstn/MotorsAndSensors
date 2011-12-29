#pragma once

#include "ofxSimpleGuiControl.h"

#include "ofxSimpleGuiSliderArrayBase.h"


class ofxSimpleGuiSliderArrayInt : public ofxSimpleGuiSliderArrayBase<int> {
	
public:
	ofxSimpleGuiSliderArrayInt(string name, int &value, int min, int max, float smoothing = 0) : ofxSimpleGuiSliderArrayBase<int>(name, value, min, max,smoothing) {
		controlType = "SliderArrayInt";
	}
	
	virtual void receiveOscMessage(ofxOscMessage& m){
		if( m.getArgType(0) == OFXOSC_TYPE_INT32 ){
			set(m.getArgAsInt32(0));
		}
	}
	
	virtual void createOscArgs(ofxOscMessage& m){
		m.addFloatArg(*value);
	}
	
	void setFromString(string& s){
		set(ofToInt(s));
	}
	
	string getString(){
		return ofToString(*value);
	}
};
