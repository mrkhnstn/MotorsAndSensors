#pragma once
/*
 *  TweenHelper.h
 *  TestBed_Mark
 *
 *  Created by TAQA_CH_0 on 28/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxTween.h"
#include "ofxBaseObject.h"

class TweenHelper {

public:
	
	float* value;
	float valueIn, valueOut;
	float timeIn, timeOut;
	int easingTypeId;
	int easingId;
	bool alwaysApply; //< if true then also apply outside of timeIn and timeOut
	
	static ofxEasing* easingObject[11];
	static string easingObjectName[11];
	static ofxTween::ofxEasingType easingType[3];
	static string easingTypeNames[3];
	
	void setup(string name, float& value, float minValue, float maxValue, float minTime, float maxTime, bool alwaysApply = false){
		easingId = 6; //linear by default
		easingTypeId = 2; //ease in out by default 
		this->alwaysApply = alwaysApply;
		this->value = &value;
		gui.addTitle(name);
		gui.addDebug("value", *this->value);
		gui.addSlider("time in", timeIn, minTime, maxTime);
		gui.addSlider("time out", timeOut, minTime, maxTime);
		gui.addSlider("value in", valueIn, minValue, maxValue);
		gui.addSlider("value out", valueOut, minValue, maxValue);
		gui.addToggle("always apply", alwaysApply);
		gui.addComboBox("ease object", easingId, 11, easingObjectName);
		gui.addComboBox("ease type", easingTypeId, 3, easingTypeNames);		
		
	}
	
	void update(float time){
		if (alwaysApply) {
			*value = ofxTween::map(time, timeIn, timeOut, valueIn, valueOut, true, *easingObject[easingId], easingType[easingTypeId]);
		} else {
			if (time >= timeIn && time <= timeOut) {
				*value = ofxTween::map(time, timeIn, timeOut, valueIn, valueOut, false, *easingObject[easingId], easingType[easingTypeId]);				
			}
		}
	}
	
};
