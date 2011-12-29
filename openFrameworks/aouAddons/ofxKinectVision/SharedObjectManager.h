#pragma once
/*
 *  SharedObjectManager.h
 *  TestBed_Mark2
 *
 *  Created by TAQA_CH_0 on 10/02/2011.
 *  Copyright 2011 AllofUs. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxSharedObject.h"
#include "KinectShare.h"
#include "GlobalVariables.h"
#include "ofxCameraStitcher.h"

#define CAMERA_STITCHER_WIDTH	960
#define CAMERA_STITCHER_HEIGHT	256

/**
 - initializes shared memory object on construction
 - shared memory initialized in client mode
 - this object should only be accessed via a singleton
 */
class SharedObjectManager : public ofxBaseObject {
	
public:
	
	GlobalVariables*		computerVision;
	ofxSharedObject			<KinectShare> sharedObject;
	ofxCameraStitcher		cameraStitcher;
	ofImage					testImage[3];
	ofImage					compositeImage;
	bool					doDrawCameraInputs;
	//bool					doDrawCompositeImage;
	//bool					doDrawCameraStitcher;
	//ofColor					imageColor;
	bool					isComputerVision; //< true if computer vision application
	
	SharedObjectManager();
	
	void setup();
	
	void setupGUI();
	
	void postGUI();
	
	void drawFbos();
	
	void update();
	
	void draw();
};
