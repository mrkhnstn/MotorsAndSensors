#pragma once
/*
 *  ofxTimeline.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxTimelineEvent.h"

class ofxTimeline : public ofxBaseObject{
	
public:

	float maxTime; //< at the moment only used to set range of event time sliders 
	
	ofxTimeline();
	
	template <class ListenerClass>
	ofxTimelineEvent& addEvent(string name, float time, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
		ofxTimelineEvent& event = addEvent(name, time);
		event.addListener<ListenerClass>(listener,method);
		return event;
	}
	
	ofxTimelineEvent& addEvent(string name, float time);	
	ofxTimelineEvent& addEvent(ofxTimelineEvent& event);	
	
	/**
	 needs to be called if times of events are changed
	 */
	void timelineChanged(ofEventArgs& e);	
	void timelineChanged();
	
	void start();
	void stop();
	
	/**
	 resets the clock to 0
	 */
	void reset(ofEventArgs& e);	
	void reset();
	
	void update();
	
	void setupGUI();
	
	void log(string logText);
	
	void setDebug(bool newValue);
	bool getDebug();
	
	bool isRunning();
	
protected:
	
	/**
	 sets the current event iterator to the first one whose time is larger then timeElapsed
	 */
	void resetCurrentEvent();
	
	struct TimelineEventCompare{	
		bool operator()(ofxTimelineEvent* i,ofxTimelineEvent* j) 
		{
			return (i->getTime() < j->getTime()); 
		}
	} timelineEventCompare;
	
	void sortEvents();	
	void incrementCurrentEvent();	
	
	float timeElapsed;
	float refTime;
	vector<ofxTimelineEvent*> events;
	vector<ofxTimelineEvent*>::iterator currentEvent;
	bool _started;
	bool _debug;
	
};
