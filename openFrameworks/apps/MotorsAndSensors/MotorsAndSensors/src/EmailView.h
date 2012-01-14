#pragma once
/*
 *  EmailView.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class EmailView : public UI::View {
public:
	
	UIElement sendButton;
	UIElement backButton;
	UIElement nextImageButton;
	UIElement prevImageButton;
	UIElement selectedImageSprite;
	UIElement emailFieldSprite;
	UIElement keyboardSprite;
	UIElement callToActionSprite;
	
	int frameSelected;	// buffer index of selected frame
	int frameStep;		// number of frames to increment or decrease on pressing next or previous buttons
	
	void setup();
	void draw();
	
	void nextImage(ofEventArgs& e);
	void prevImage(ofEventArgs& e);
	void sendEmail(ofEventArgs& e);
	
};

