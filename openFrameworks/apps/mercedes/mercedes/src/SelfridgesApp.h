#pragma once
/*
 *  SelfridgesApp.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 10/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "IdleModeView.h"
#include "CameraOffView.h"
#include "CountdownView.h"
#include "RecordingView.h"
#include "PlaybackView.h"
#include "ImageSelectionView.h"
#include "EmailView.h"
#include "SendConfirmationView.h"

#include "CameraManager.h"
#include "EmailManager.h"

class SelfridgesApp : public ofxBaseObject {
public:
	
	//static SelfridgesApp& singleton();
	
	int displayMode; // 0: dev, 1: rotate (non scale), 2: rotate (scaled)
	
	IdleModeView idleModeView;
	CameraOffView cameraOffView;
	CountdownView countdownView;
	RecordingView recordingView;
	PlaybackView playbackView;
	ImageSelectionView imageSelectionView;
	EmailView emailView;
	SendConfirmationView sendConfirmationView;
	
	vector<UI::UIContainer*> views; // contains all views
	
	UI::UIContainer* cameraOffReturnView; // view to return to when switching on camera in camera off view
	
	// SETUP //////////////////////////////////////////////
	
	void setup(){
		Singleton<CameraManager>::instance()->setup();
		
		displayMode = 0;
		
		views.push_back(&idleModeView);
		views.push_back(&cameraOffView);
		views.push_back(&countdownView);
		views.push_back(&recordingView);
		views.push_back(&playbackView);
		views.push_back(&imageSelectionView);
		views.push_back(&emailView);
		views.push_back(&sendConfirmationView);
		
		for(int i=0; i<views.size(); i++)
			views[i]->setup();
		
		// start off with idle mode
		idleModeView.enable();
	}
	
	// GUI //////////////////////////////////////////////
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("SelfridgesApp"));
		string displayModeChoices[2] = {"dev","rotated CW"};
		gui.addComboBox("displayMode", displayMode, 2, displayModeChoices);
		
		gui.addTitle("UIElements");
		gui.addToggle("doDrawNames", UI::UIElement::doDrawNames);
		gui.addToggle("doDrawOutlines", UI::UIElement::doDrawOutlines);
		gui.addToggle("doDrawCrosses", UI::UIElement::doDrawCrosses);
		
		Singleton<CameraManager>::instance()->setupGUI();
		
		for(int i=0; i<views.size(); i++)
			views[i]->setupGUI();
	}
	
	void postGUI(){
		for(int i=0; i<views.size(); i++)
			views[i]->postGUI();
	}
	
	// UPDATE //////////////////////////////////////////////
	
	void update(){
		Singleton<CameraManager>::instance()->update();
		
		for(int i=0; i<views.size(); i++)
			views[i]->update();
	}
	
	// DRAW //////////////////////////////////////////////
	
	void draw(){
		
		float scale = 1;
		float translateX = 0;
		float translateY = 0;
		float rotation = 0;
		
		switch(displayMode){
			case 0: // dev
				scale = ofGetHeight() / Coords::height;
				translateX = ofGetWidth() * 0.5 - Coords::width * 0.5 * scale;
				rotation = 0;
				Coords::x = translateX;
				Coords::y = translateY;
				Coords::vx.x = 1/scale;
				Coords::vx.y = 0;
				Coords::vy.x = 0;
				Coords::vy.y = 1/scale;
				break;
			/*
			case 1: // installation (rotated, not scaled)
				scale = 1;
				translateX = ofGetWidth();
				translateY = 0;
				rotation = 90;
				Coords::x = translateX;
				Coords::y = translateY;
				Coords::vx.x = 0;
				Coords::vx.y = -1/scale;
				Coords::vy.x = 1/scale;
				Coords::vy.y = 0;
				break;
			*/
			case 1: // rotated
				scale = ofGetWidth() / Coords::height;
				translateX = ofGetWidth();
				translateY = ofGetHeight() * 0.5 - Coords::width * 0.5 * scale;
				rotation = 90;
				Coords::x = translateX;
				Coords::y = translateY;
				Coords::vx.x = 0;
				Coords::vx.y = -1/scale;
				Coords::vy.x = 1/scale;
				Coords::vy.y = 0;
				break;

				
		}
		
		ofPushMatrix();
		ofTranslate(translateX, translateY, 0);
		ofRotate(rotation, 0, 0, 1);
		ofScale(scale, scale, 0);
		for(int i=0; i<views.size(); i++)
			views[i]->draw();
		
		ofPopMatrix();
	}
	
	// UI HANDLERS //////////////////////////////////////////////
	
	void switchPlaybackToCameraOffView(ofEventArgs& e){
		//TODO: switch off camera
		cameraOffReturnView = &playbackView;
		playbackView.disable();
		cameraOffView.enable();
	}
	
	void switchIdleModeToCameraOffView(ofEventArgs& e){
		//TODO: switch off camera
		cameraOffReturnView = &idleModeView;
		idleModeView.disable();
		cameraOffView.enable();
	}
	
	void returnFromCameraOffView(ofEventArgs& e){
		//TODO: switch on camera
		cameraOffView.disable();
		cameraOffReturnView->enable();
	}
	
	void switchToCountdownView(ofEventArgs& e){
		idleModeView.disable();
		countdownView.enable();
	}
	
	void switchToImageSelectionView(ofEventArgs& e){
		playbackView.disable();
		imageSelectionView.enable();
	}
	
	void selectImageToEmail(int i)
	{
		emailView.frameSelected = i;
		imageSelectionView.disable();
		emailView.enable();
	}
	
	// not in use
	void switchToEmailView(ofEventArgs& e){
		imageSelectionView.disable();
		emailView.enable();
	}
	
	void startRecording(ofEventArgs& e){
		countdownView.disable();
		recordingView.enable();
	}
	
	void stopRecording(ofEventArgs& e){
		recordingView.disable();
		playbackView.enable();
	}
	
	void clearRecording(ofEventArgs& e){
		playbackView.disable();
		idleModeView.enable();
	}
	
	void cancelImageSelection(ofEventArgs& e){
		imageSelectionView.disable();
		playbackView.enable();
	}
	
	void sendEmail(string emailAddress, int frame){
		emailView.disable();
		sendConfirmationView.enable();
	}
	
	// not in use
	void sendEmail(ofEventArgs& e){
		emailView.disable();
		sendConfirmationView.enable();
	}
	
	void switchBackToImageSelection(ofEventArgs& e){
		emailView.disable();
		imageSelectionView.enable();
	}
	
	void switchBackToPlaybackView(ofEventArgs& e){
		sendConfirmationView.disable();
		playbackView.enable();
	}
};
