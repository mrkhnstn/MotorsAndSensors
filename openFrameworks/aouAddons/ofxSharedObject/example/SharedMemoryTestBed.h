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
#include "TestShare.h"

//////////////////////////////////////////////////////////////////////////////////

class SharedMemoryTestBed : public ofxBaseObject {
	
public:
	
	int id; //if 0 then master otherwise client
	
	bool master; //< if true then this is the instance allocating the shared memory
	string sharedObjectFilename;
	
	ofxSharedObject<TestShare> sharedObject;

	ofVideoGrabber videoGrabber;
	ofImage image;
	
	SharedMemoryTestBed(){
		id = 0;
		master = false;
		sharedObjectFilename = "/tmp/SharedMemoryTest";
	}
	
	void setup(){
		master = (id == 0);
		
		//setup shared memory
		sharedObject.attach(sharedObjectFilename, master);
		
		if (master) {
			videoGrabber.initGrabber(IMG_WIDTH, IMG_HEIGHT, true);
		} else {
			image.allocate(IMG_WIDTH, IMG_HEIGHT, OF_IMAGE_COLOR);
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
			videoGrabber.update();
			
			//sharedObject.lockWrite();
			// copy pixels from videoGrabber into shared memory
			memcpy(sharedObject.getObject().pixels,videoGrabber.getPixels(),IMG_WIDTH*IMG_HEIGHT*IMG_CHANNELS);
			//sharedObject.unlock();
						
		} else {
			// copy pixels from shared memory into image to draw
			image.setFromPixels(sharedObject.getObject().pixels, IMG_WIDTH, IMG_HEIGHT, OF_IMAGE_COLOR, true);
			image.update();
		}
	}
	
	void draw(){
		if(master){
			videoGrabber.draw(0, 0);
		} else {
			image.draw(0, 0);
		}
	}

};

