#pragma once
/*
 *  SharedMemoryTestBed.h
 *  ofxKinect
 *
 *  Created by Mark Hauenstein on 02/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxSharedObject.h"
#include "ofxKinect.h"
#include "KinectTestShare.h"

//////////////////////////////////////////////////////////////////////////////////

class KinectSharedMemoryTestBed : public ofxBaseObject {
	
public:
	
	int id; //if 0 then master otherwise client
	
	bool master; //< if true then this is the instance allocating the shared memory
	string sharedObjectFilename;
	
	ofxSharedObject<KinectTestShare> sharedObject;
	
	ofxKinect kinect;
	ofImage image;
	
	KinectSharedMemoryTestBed(){
		id = 0;
		master = false;
		sharedObjectFilename = "/tmp/SharedMemoryTest";
	}
	
	void setup(){
		master = (id == 0);
		
		//setup shared memory
		sharedObject.attach(sharedObjectFilename, master);
		
		if (master) {
			kinect.init();
			kinect.setVerbose(true);
			kinect.open();
		} else {
			image.allocate(KINECT_WIDTH, KINECT_HEIGHT, OF_IMAGE_GRAYSCALE);
		}
		
	}
	
	void setupGUI(){
		gui.addFPSCounter();
		gui.addDebug("instance id", id);
		gui.addDebug("is master", master);
		gui.addSlider("test float",sharedObject.getObject().floatValue, 0, 100);		
	}
	
	void update(){
		if (master) {
			kinect.update();
			
			//sharedObject.lockWrite();
			// copy pixels from kinect into shared memory
			memcpy(sharedObject.getObject().pixels,kinect.getDepthPixels(),KINECT_WIDTH*KINECT_HEIGHT*KINECT_CHANNELS);
			//sharedObject.unlock();
			
		} else {
			// copy pixels from shared memory into image to draw
			image.setFromPixels(sharedObject.getObject().pixels, KINECT_WIDTH, KINECT_HEIGHT, OF_IMAGE_GRAYSCALE, true);
			image.update();
		}
	}
	
	void draw(){
		if(master){
			//videoGrabber.draw(0, 0);
			kinect.getDepthTextureReference().draw(0, 0);
		} else {
			image.draw(0, 0);
		}
	}
	
};

