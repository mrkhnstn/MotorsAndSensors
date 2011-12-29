#pragma once
/*
 *  CameraManager.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 17/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxFBOTexture.h"

#define NUM_FRAMES 90 // number of frames that can be recorded

class CameraManager : public ofxBaseObject {
public:
	
	ofEvent<ofEventArgs> onRecordingFinished;
	ofEventArgs e;
	
	void setup(){
		camWidth = 640;
		camHeight = 480;
		
		recordFrame = 0;
		playFrame = 0;
		playSubFrame = 0;
		playSubFramesPerFrame = 10;
		playBlendRate = 0;
		isRecording = false;
		
		vidGrabber.setVerbose(true);
		vidGrabber.initGrabber(camWidth,camHeight);
		
		for(int i=0; i<NUM_FRAMES; i++)
			frames[i].allocate(camWidth,camHeight);
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("CameraManager"));
		gui.addDebug("recordFrame", recordFrame);
		gui.addDebug("playFrame",playFrame);
		gui.addDebug("playSubFrame", playSubFrame);
		gui.addSlider("playSubFramesPerFrame", playSubFramesPerFrame, 1, 30);
	}
	
	void update(){
		vidGrabber.grabFrame();
		
		if(isRecording){
			// store a new frame in RAM
			if (vidGrabber.isFrameNew()){
				frames[recordFrame].begin();
				//frames[recordFrame].loadData(vidGrabber.getPixels(), camWidth,camHeight, GL_RGB);
				vidGrabber.draw(0, 0);
				frames[recordFrame].end();
				recordFrame++;
			}
			// check whether recording is done
			if (recordFrame >= NUM_FRAMES) {
				isRecording = false;
				ofNotifyEvent(onRecordingFinished, e, this); // notify listeners that recording is finished
			}
		}
		
	}
	
	ofTexture& getLiveTexture(){
		return vidGrabber.getTextureReference();
	}
	
	ofTexture& getPlaybackTexture(){
		return frames[playFrame];
	}
	
	void drawLiveCamera(){
		vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight()); // draw camera feed full screen
	}
	
	void drawPlayback(){
		frames[playFrame].draw(0, 0, ofGetWidth(), ofGetHeight()); // draw playback frame from RAM
	}
	
	void drawPlayback(float x, float y, float width, float height){
		
		int nextFrame = playFrame + 1;
		if(nextFrame == getNumFrames())
			nextFrame = 0;
		
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, 255);
		frames[playFrame].draw(x,y,width,height); // draw playback frame from RAM
		
		ofSetColor(255, 255, 255, playBlendRate * 255);
		frames[nextFrame].draw(x,y,width,height); // draw playback frame from RAM
		
		ofPopStyle();
	}
	
	void stepPlayback(){
		// increment / loop playback frame
		playSubFrame++;
		if (playSubFrame >= playSubFramesPerFrame) {
			playSubFrame = 0;
			playFrame++;
			if(playFrame >= NUM_FRAMES)
				playFrame = 1;
		}
		playBlendRate = playSubFrame / (float)playSubFramesPerFrame;
	}

	void startRecording(){
		recordFrame = 0;
		playFrame = 0;
		playSubFrame = 1;
		isRecording = true;
	}
	
	// returns the frame a
	ofTexture& getFrame(int i){
		if (i>=0 && i<NUM_FRAMES) {
			return frames[i];
		} else {
			return frames[0]; // return first frame if requested frame is out of bounds
		}
	}
	
	int getRecordFrame(){
		return recordFrame;
	}
	
	// returns the number of frames in the image buffer
	int getNumFrames(){
		return NUM_FRAMES;
	}

	
private:
	
	ofVideoGrabber 		vidGrabber;
	int 				camWidth;
	int 				camHeight;
	
	ofxFBOTexture		frames[NUM_FRAMES];		// buffer used to temporarily store captured images in RAM
	int					recordFrame;			
	int					playFrame;
	int					playSubFrame;
	int					playSubFramesPerFrame;	
	float				playBlendRate;
	bool				isRecording;			// true if recording
	
};
