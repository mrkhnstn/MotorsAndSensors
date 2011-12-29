#pragma once
/*
 *  ofxCameraStitcher.h
 *  TestBed_Mark
 *
 *  Created by TAQA_CH_2 on 11/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxProjectionManager.h"
#include <algorithm>
//#include <utility>

class CameraIdPair {
public:
	int id;
	ofxProjectionManager* projectionManager;
};

class ofxCameraStitcher : public ofxBaseObject {

public:
	
	VectorMap<string, ofxProjectionManager*> cameras;
	VectorMap<string, ofTexture*> cameraTextures;
	vector<CameraIdPair> sortedCameras;
	
	ofxFBOTexture fbo; //< main fbo to draw cameras into
	ofxFBOTexture fbo2;
	unsigned char * pixels;
	
	//unsigned char testPixels[960 * 256];
	
	void setup(int fboWidth, int fboHeight);
	
	void addCamera(string cameraName, ofTexture& cameraTexture, string xmlFolderpath = "");
	
	void setupGUI();
	
	void postGUI();
	
	void update();
	
	void drawFbos();
	
	void draw();
	
	void draw(float x, float y, float width, float height);
	
	struct sortCameraClass {
		bool operator() (CameraIdPair a,CameraIdPair b) { return (a.projectionManager->drawLayer < b.projectionManager->drawLayer);}
	} sortCamera;
	
};