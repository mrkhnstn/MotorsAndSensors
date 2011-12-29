#pragma once
/*
 *  RecordingView.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Blink.h"

class RecordingView : public UI::View {
public:
	
	UIElement recIconSprite;
	ofxTimeline timeline;
	
	void setup();	
	void enable();
	void disable();
	void draw();
	
};