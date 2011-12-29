#pragma once
/*
 *  ofxTimelineTest.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxTimeline.h"

class ofxTimelineTest : public ofxBaseObject {
	
public:
	
	ofxTimeline timeline;
	
	void setup(){
		name = "TimelineTest";
		
		ofxTimelineEvent& e = timeline.addEvent("testA", 2, this, &ofxTimelineTest::testMethodA);
		timeline.addEvent("testB", 4, this, &ofxTimelineTest::testMethodB);
		timeline.addEvent("testC", 6, this, &ofxTimelineTest::testMethodC);
		timeline.setDebug(true);
		timeline.maxTime = 16;
	}
	
	void update(){
	}
	
	void draw(){}
	
	void setupGUI(){
		timeline.setupGUI();
	}
	
	void testMethodA(ofEventArgs& e){
		ofLog(OF_LOG_VERBOSE, "testMethodA");
	}
	
	void testMethodB(ofEventArgs& e){
		ofLog(OF_LOG_VERBOSE, "testMethodB");
	}
	
	void testMethodC(ofEventArgs& e){
		ofLog(OF_LOG_VERBOSE, "testMethodC");
	}
	
	void testRelativeMethod(ofEventArgs& e){
		ofLog(OF_LOG_VERBOSE, "testRelative");
	}
};