#pragma once
/*
 *  CountdownView.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "Includes.h"
#include "Countdown.h"

class CountdownView : public UI::View {
public:
	
	UIElement instructionSprite;
	Countdown progressMsgSprite;
	ofxTimeline timeline;
	
	void setup();
	void enable();
	void disable();
	void draw();
	
	void three(ofEventArgs& e);
	void two(ofEventArgs& e);
	void one(ofEventArgs& e);
};
