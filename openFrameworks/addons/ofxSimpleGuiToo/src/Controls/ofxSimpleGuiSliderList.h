/*
#pragma once

#include "ofxSimpleGuiControl.h"

#include "ofxSimpleGuiSliderBase.h"

class ofxSimpleGuiSliderList : public ofxSimpleGuiSliderBase<int> {
	
public:
	ofxSimpleGuiSliderList(string name, int value = 0) : ofxSimpleGuiSliderBase<int>(name, value,0,0,0) {
		controlType = "SliderList";
	}
	
	virtual void receiveOscMessage(ofxOscMessage& m){
		if( m.getArgType(0) == OFXOSC_TYPE_INT32 ){
			set(m.getArgAsInt32(0));
		}
	}
	
	virtual void createOscArgs(ofxOscMessage& m){
		m.addIntArg(*value);
	}

	
};
*/