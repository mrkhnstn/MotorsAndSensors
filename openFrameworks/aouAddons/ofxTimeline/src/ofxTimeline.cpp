/*
 *  ofxTimeline.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxTimeline.h"

ofxTimeline::ofxTimeline(){
	
	timeElapsed = 0;
	refTime = ofGetElapsedTimef();
	currentEvent = events.end();
	_started = false;
	_debug = false;
	name = "Timeline";
	maxTime = 255;
}


ofxTimelineEvent& ofxTimeline::addEvent(string name, float time){
	return addEvent(* (new ofxTimelineEvent(name,time)));
}

ofxTimelineEvent& ofxTimeline::addEvent(ofxTimelineEvent& event){
	events.push_back(&event);
	timelineChanged();
	return event;
}

void ofxTimeline::timelineChanged(ofEventArgs& e){
	timelineChanged();
}

void ofxTimeline::timelineChanged(){
	sortEvents();
	resetCurrentEvent();
}

void ofxTimeline::start(){
	if (!_started) {
		log("start");
		refTime = ofGetElapsedTimef();
		ofAddListener(ofEvents.update, (ofxMSAInteractiveObject*)this, &ofxMSAInteractiveObject::_update);
		_started = true;
	}
}

void ofxTimeline::stop(){
	if(_started){
		log("stop");
		ofRemoveListener(ofEvents.update, (ofxMSAInteractiveObject*)this, &ofxMSAInteractiveObject::_update);
		_started = false;
	}
}

void ofxTimeline::reset(ofEventArgs& e){
	reset();
}

/**
 resets the clock
 */
void ofxTimeline::reset(){
	log("reset");
	timeElapsed = 0;
	refTime = ofGetElapsedTimef();
	resetCurrentEvent();
}

void ofxTimeline::update(){
	float currentTime = ofGetElapsedTimef();
	float deltaTime = currentTime - refTime;
	refTime = currentTime;
	timeElapsed += deltaTime;
	
	if (currentEvent != events.end()) {
		ofxTimelineEvent* tempEvent = *currentEvent;
		
		if (timeElapsed >= tempEvent->getTime()) {
			log("dispatched: " + tempEvent->name);
			incrementCurrentEvent();
			tempEvent->dispatchEvent();
		}
	}
}

void ofxTimeline::setupGUI(){
	ofxSimpleGuiPage& page = gui.addPage(name);
	gui.addDebug("timeElapsed", timeElapsed);
	gui.addButton("start", (ofxBaseObject*)this, &ofxBaseObject::_start); 
	gui.addButton("stop", (ofxBaseObject*)this, &ofxBaseObject::_stop);
	gui.addButton("reset", this, &ofxTimeline::reset);
	gui.addButton("timelineChanged", this, &ofxTimeline::timelineChanged);
	gui.addTitle("Events\nPress on [timelineChanged]\nafter changes to event times",50);
	for (vector<ofxTimelineEvent*>::iterator it = events.begin(); it != events.end(); ++it) {
		(*it)->setupGUI(0,maxTime);
	}
	page.loadFromXML();
	timelineChanged();
}

void ofxTimeline::log(string logText){
	if(_debug)
		ofLog(OF_LOG_VERBOSE, name + ": " + logText);
}

void ofxTimeline::setDebug(bool newValue){
	_debug = newValue;	
}

bool ofxTimeline::getDebug(){
	return _debug;
}

bool ofxTimeline::isRunning(){
	return _started;
}

void ofxTimeline::resetCurrentEvent(){
	bool found = false;
	currentEvent = events.begin();
	while(currentEvent != events.end() && !found){
		ofxTimelineEvent* tempEvent = *currentEvent;
		if (tempEvent->getTime() >= timeElapsed) {
			found = true;
		} else {
			currentEvent++;
		}
	}
}



void ofxTimeline::sortEvents(){
	sort(events.begin(),events.end(),timelineEventCompare);
}

void ofxTimeline::incrementCurrentEvent(){
	if (currentEvent != events.end()) {
		currentEvent++;
		if (currentEvent == events.end()) {
			stop();
		}
	}
}