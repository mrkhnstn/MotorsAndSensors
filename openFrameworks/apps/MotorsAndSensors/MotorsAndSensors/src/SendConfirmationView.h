#pragma once
/*
 *  SendConfirmationView.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 17/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class SendConfirmationView : public UI::View {
public:
	
	UIElement progressMsgSprite;
	ofxTimeline timeline;
	
	void setup();
	void enable();	
	void disable();	
	void displayConfirmation(ofEventArgs& e);
	
};
