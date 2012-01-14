/*
 *  SendConfirmationView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 17/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SendConfirmationView.h"
#include "SelfridgesApp.h"

void SendConfirmationView::setup(){
	// setup view
	UIContainer::setup();
	name = "SendConfirmationView";

	// setup progress message sprite
	progressMsgSprite.setup();
	progressMsgSprite.name = "progressMsgSprite";
	addElement(progressMsgSprite);
	
	// setup timeline events
	timeline.addEvent("displayConfirmation", 2, this, &SendConfirmationView::displayConfirmation);
	timeline.addEvent("switchBackToPlaybackView", 5, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::switchBackToPlaybackView);
}

void SendConfirmationView::enable(){
	UIContainer::enable();
	progressMsgSprite.label = "Sending...";
	timeline.reset();
	timeline.start();
}

void SendConfirmationView::disable(){
	timeline.stop();
	UIContainer::disable();
}

void SendConfirmationView::displayConfirmation(ofEventArgs& e){
	progressMsgSprite.label = "Your email was sent.";
}