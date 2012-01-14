/*
 *  RecordingView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "RecordingView.h"
#include "SelfridgesApp.h"
#include "CameraManager.h"

void RecordingView::setup(){
	
	// setup view //
	UIContainer::setup();
	name = "RecordingView";
	
	// setup UI elements //
	recIconSprite.setup();
	recIconSprite.name = "recIconSprite";
	recIconSprite.label = "recording";
	addElement(recIconSprite);
	
	// setup timeline events
	ofAddListener(Singleton<CameraManager>::instance()->onRecordingFinished, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::stopRecording);

}

void RecordingView::enable(){
	UI::UIContainer::enable();
	Singleton<CameraManager>::instance()->startRecording();
	timeline.reset();
	timeline.start();
}

void RecordingView::disable(){
	timeline.stop();
	UI::UIContainer::disable();
}

void RecordingView::draw(){
	if(!enabled) return;
	
	
	float currentFrame = (float)Singleton<CameraManager>::instance()->getRecordFrame();
	float totalFrames = (float)Singleton<CameraManager>::instance()->getNumFrames();
	float recPercentage =  currentFrame / totalFrames * 100;
	
	recIconSprite.label = "recording " + ofToString(recPercentage,0) + "%";
	
	// draw live camera
	ofPushMatrix();
	ofTranslate(0, Coords::height, 0);
	ofRotate(-90, 0, 0, 1);
 	Singleton<CameraManager>::instance()->getLiveTexture().draw(0,0,Coords::height,Coords::width);
	ofPopMatrix();
	
	
	UI::UIContainer::draw();
}