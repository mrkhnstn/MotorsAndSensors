/*
 *  EmailView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "EmailView.h"
#include "SelfridgesApp.h"

void EmailView::setup(){
	
	// setup view //
	UIContainer::setup();
	name = "EmailView";

	frameSelected = 0;
	frameStep = 10;
	
	// setup UI elements //
	
	sendButton.setup();
	sendButton.name = "sendButton";
	sendButton.label = "send";
	sendButton.isInteractive = true;
	ofAddListener(sendButton.onPressed, this, &EmailView::sendEmail);
	addElement(sendButton);
	
	backButton.setup();
	backButton.name = "backButton";
	backButton.label = "back";
	backButton.isInteractive = true;
	ofAddListener(backButton.onPressed, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::switchBackToImageSelection);
	addElement(backButton);
	
	nextImageButton.setup();
	nextImageButton.name = "nextImageButton";
	nextImageButton.label = "next";
	nextImageButton.isInteractive = true;
	ofAddListener(nextImageButton.onPressed, this, &EmailView::nextImage);
	addElement(nextImageButton);
	
	prevImageButton.setup();
	prevImageButton.name = "prevImageButton";
	prevImageButton.label = "previous";
	prevImageButton.isInteractive = true;
	ofAddListener(prevImageButton.onPressed, this, &EmailView::prevImage);
	addElement(prevImageButton);	
	
	callToActionSprite.setup();
	callToActionSprite.name = "callToActionSprite";
	callToActionSprite.label = "Please enter your email address";
	addElement(callToActionSprite);
	
	selectedImageSprite.setup();
	selectedImageSprite.name = "selectedImageSprite";
	addElement(selectedImageSprite);
	
	emailFieldSprite.setup();
	emailFieldSprite.name = "emailFieldSprite";
	emailFieldSprite.label = "name@email.com";
	addElement(emailFieldSprite);
	
	keyboardSprite.setup();
	keyboardSprite.name = "keyboardSprite";
	keyboardSprite.label = "keyboard";
	addElement(keyboardSprite);
}

void EmailView::draw(){
	if(!enabled) return;

	// draw selected image
	ofPushMatrix();
	ofTranslate(selectedImageSprite.x, selectedImageSprite.y+selectedImageSprite.height, 0);
	ofRotate(-90, 0, 0, 1);
 	Singleton<CameraManager>::instance()->getFrame(frameSelected).draw(0,0,selectedImageSprite.height,selectedImageSprite.width);
	ofPopMatrix();
	
	UIContainer::draw();
}

void EmailView::nextImage(ofEventArgs& e){
	frameSelected += frameStep;
	while(frameSelected >= Singleton<CameraManager>::instance()->getNumFrames())
		frameSelected -= Singleton<CameraManager>::instance()->getNumFrames();
}

void EmailView::prevImage(ofEventArgs& e){
	frameSelected -= frameStep;
	while(frameSelected < 0)
		frameSelected += Singleton<CameraManager>::instance()->getNumFrames();
}

void EmailView::sendEmail(ofEventArgs& e){
	Singleton<SelfridgesApp>::instance()->sendEmail("mail@mail.com",frameSelected);
}