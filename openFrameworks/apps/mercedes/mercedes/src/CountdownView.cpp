/*
 *  CountdownView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CountdownView.h"
#include "SelfridgesApp.h"
#include "CameraManager.h"

void CountdownView::setup(){
	
	// setup view //
	
	UIContainer::setup();
	name = "CountdownView";
	
	// setup UI elements //

	instructionSprite.setup();
	instructionSprite.name = "instructionSprite";
	instructionSprite.label = "Please step back and get into position";
	addElement(instructionSprite);
	
	progressMsgSprite.setup();
	progressMsgSprite.name = "progressMsgSprite";
	progressMsgSprite.label = "";
	addElement(progressMsgSprite);
	
	//timeline.addEvent("three", 1, this, &CountdownView::three);
	timeline.addEvent("two", 1, this, &CountdownView::two);
	timeline.addEvent("one", 2, this, &CountdownView::one);	
	timeline.addEvent("countdownFinished", 3, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::startRecording);
}

void CountdownView::enable(){
	UIContainer::enable();
	progressMsgSprite.label = "3";
	timeline.reset();
	timeline.start();
}

void CountdownView::disable(){
	timeline.stop();
	UIContainer::disable();
}

void CountdownView::draw(){
	if(!enabled) return;
	
	// draw live camera
	ofPushMatrix();
	ofTranslate(0, Coords::height, 0);
	ofRotate(-90, 0, 0, 1);
 	Singleton<CameraManager>::instance()->getLiveTexture().draw(0,0,Coords::height,Coords::width);
	ofPopMatrix();	
	
	// draw other ui elements
	UIContainer::draw();
}

void CountdownView::three(ofEventArgs& e){
	progressMsgSprite.label = "3";
}

void CountdownView::two(ofEventArgs& e){
	progressMsgSprite.label = "2";
}

void CountdownView::one(ofEventArgs& e){
	progressMsgSprite.label = "1";
}