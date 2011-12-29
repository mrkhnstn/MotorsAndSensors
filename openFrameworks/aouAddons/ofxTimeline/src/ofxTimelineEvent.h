#pragma once
/*
 *  ofxTimelineEvent.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

class ofxTimelineEvent {
	
public:
	
	ofEvent<ofEventArgs> onTime;
	string name;
	bool enabled;
	
	ofxTimelineEvent(string name = "", float time = 0){
		this->time = time;
		this->name = name;
		enabled = true;
	}
	
	template <class ListenerClass>
	void addListener(ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
		ofAddListener(onTime, listener,method);
	}
	
	void dispatchEvent(){
		if (enabled) {
			ofNotifyEvent(onTime, eventArgs);
		}
	}
	
	virtual float setTime(float newTime){
		if (time != newTime) {
			time = newTime;
			//TODO: changed!!
		}
	}
	
	virtual float getTime(){
		return time;
	}
	
	virtual void setupGUI(float minTime, float maxTime){
		gui.addSlider(this->name, time, minTime, maxTime); 
	}
	
protected:
	
	float time;	
	ofEventArgs eventArgs;
	
};