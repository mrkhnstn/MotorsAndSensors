/*
 *  PlaybackView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "PlaybackView.h"
#include "SelfridgesApp.h"

void PlaybackView::setup(){
	
	// setup view //
	UIContainer::setup();
	name = "PlaybackView";
	
	// setup UI elements //
	clearButton.setup();
	clearButton.name = "clearButton";
	clearButton.label = "clear";
	clearButton.isInteractive = true;
	ofAddListener(clearButton.onPressed, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::clearRecording);
	addElement(clearButton);
	
	emailButton.setup();
	emailButton.name = "emailButton";
	emailButton.label = "email";
	emailButton.isInteractive = true;
	ofAddListener(emailButton.onPressed, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::switchToImageSelectionView);
	addElement(emailButton);
	
	videoOffButton.setup();
	videoOffButton.name = "videoOffButton";
	videoOffButton.label = "camera off";
	videoOffButton.isInteractive = true;
	ofAddListener(videoOffButton.onPressed, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::switchPlaybackToCameraOffView);
	addElement(videoOffButton);
	
	playbackIconSprite.setup();
	playbackIconSprite.name = "playbackIconSprite";
	playbackIconSprite.label = "playback icon";
	addElement(playbackIconSprite);
	
}

void PlaybackView::draw(){
	if(!enabled) return;	

	// increment playback frame
	Singleton<CameraManager>::instance()->stepPlayback();
	
	// draw playback frame
	ofPushMatrix();
	ofTranslate(0, Coords::height, 0);
	ofRotate(-90, 0, 0, 1);
 	//Singleton<CameraManager>::instance()->getPlaybackTexture().draw(0,0,Coords::height,Coords::width);
	
	Singleton<CameraManager>::instance()->drawPlayback(0,0,Coords::height,Coords::width);
	ofPopMatrix();
	
	// draw other UI elements
	UI:UIContainer::draw();
}