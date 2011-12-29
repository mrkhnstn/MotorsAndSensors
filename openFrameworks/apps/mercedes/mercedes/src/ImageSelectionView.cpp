/*
 *  ImageSelectionView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 14/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageSelectionView.h"
#include "SelfridgesApp.h"

void ImageSelectionView::setup(){
	
	// setup view
	
	UIContainer::setup();
	name = "ImageSelectionView";
	isInteractive = true; // because of image selection
	
	// setup UI elements //

	cancelButton.setup();
	cancelButton.name = "cancelButton";
	cancelButton.label = "cancel";
	cancelButton.isInteractive = true;
	ofAddListener(cancelButton.onPressed, Singleton<SelfridgesApp>::instance(), &SelfridgesApp::cancelImageSelection);
	addElement(cancelButton);
	
	helpTextSprite.setup();
	helpTextSprite.name = "helpTextSprite";
	helpTextSprite.label = "Please select an image";
	addElement(helpTextSprite);
	
	// setup grid //
	
	cols = 3;
	rows = 3;
	imgW = Coords::width / cols;
	imgH = Coords::height / rows;
	imgCount = cols * rows;
	
	float frame = 0;
	float frameStep = (Singleton<CameraManager>::instance()->getNumFrames() - 1.0) / (imgCount - 1.0);
	int i=0;
	for (int row=0; row<rows; row++) {
		for(int col=0; col<cols; col++){
			gridFrames[i] = (int)round(frame);
			frame += frameStep;
			i++;
		}
	}
	
}

void ImageSelectionView::draw(){
	if(!enabled) return;
	
	// draw selection grid 
	int i=0;
	for (int row=0; row<rows; row++) {
		for(int col=0; col<cols; col++){
			ofTexture& tex = Singleton<CameraManager>::instance()->getFrame(gridFrames[i]);
			ofPushMatrix();
			ofTranslate(col*imgW, (row+1)*imgH, 0);
			ofRotate(-90, 0, 0, 1);
			tex.draw(0,0,imgH,imgW);
			ofPopMatrix();
			i++;
		}
	}

	UIContainer::draw();
}

void ImageSelectionView::onPress(int x, int y, int button){
	if (cancelButton.isMouseOver()) return; // prevent image selection when pressing cancel button
	
	// calculate selected cell in grid from mouse x and mouse y
	// x and y swapped due to screen rotation!
	int colSelected = floor(x/ imgW);
	int rowSelected = floor(y / imgH);
	int frameSelected = ofClamp(rowSelected * cols + colSelected,0,imgCount-1);

	//ofLog(OF_LOG_VERBOSE, "image selected: " + ofToString(frameSelected));
	Singleton<SelfridgesApp>::instance()->selectImageToEmail(gridFrames[frameSelected]);
}