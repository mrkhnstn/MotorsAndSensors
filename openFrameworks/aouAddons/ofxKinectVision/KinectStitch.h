#pragma once
/*
 *  KinectCapture.h
 *  TestBed_SharedMemory
 *
 *  Created by Mark Hauenstein on 03/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxSharedObject.h"
#include "KinectShare.h"
#include "ofxCvGrayscaleImage.h"

/**
 captures depth images from one kinect and copies them to shared memory
 */
class KinectStitch : public ofxBaseObject {
public:
	
	ofxSharedObject<KinectShare> sharedObject;
	ofxCvGrayscaleImage image; 
	
	KinectStitch(){
		//
	}
	
	void setup(){
		image.allocate(KINECT_COUNT*KINECT_WIDTH, KINECT_HEIGHT);
		
		sharedObject.attach(KINECT_SHARE_PATH, true); // setting up as master share
	}
	
	void setupGUI(){
		gui.addTitle("kinect stitch");
	}
	
	void update(){
		for (int i=0; i<KINECT_COUNT; i++) {
			image.setROI(i*KINECT_WIDTH,0,KINECT_WIDTH,KINECT_HEIGHT );
			image.setRoiFromPixels(sharedObject.getObject().pixels[i], KINECT_WIDTH, KINECT_HEIGHT);
		}
	}
	
	void draw(){
		image.draw(0,0,ofGetWidth(),ofGetHeight());
	}
	
};
