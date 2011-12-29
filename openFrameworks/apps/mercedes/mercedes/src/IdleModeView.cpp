/*
 *  IdleModeView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "IdleModeView.h"
#include "SelfridgesApp.h"
#include "CameraManager.h"

void IdleModeView::setup(){
	
	// setup view //
	UIContainer::setup();
	name = "IdleModeView";
	
	// setup UI elements //
	recordButton.setup();
	recordButton.name = "recordButton";
	recordButton.label = "record";
	recordButton.isInteractive = true;
	recordButton.verbose = false;	
	ofAddListener(recordButton.onPressed, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::switchToCountdownView);
	addElement(recordButton);
	
	videoOffButton.setup();
	videoOffButton.name = "videoOffButton";
	videoOffButton.label = "camera off";
	videoOffButton.isInteractive = true;
	ofAddListener(videoOffButton.onPressed, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::switchIdleModeToCameraOffView);
	addElement(videoOffButton);
	
	instructionSprite.setup();
	instructionSprite.name = "instructionSprite";
	instructionSprite.label = "Capture and email a picture of yourself";
	addElement(instructionSprite);
	
}

void IdleModeView::draw(){
	if(!enabled) return;
	
	// draw live camera
	ofPushMatrix();
	ofTranslate(0, Coords::height, 0);
	ofRotate(-90, 0, 0, 1);
 	Singleton<CameraManager>::instance()->getLiveTexture().draw(0,0,Coords::height,Coords::width);
	ofPopMatrix();
	
	// draw other ui elements
	UI::UIContainer::draw();
}