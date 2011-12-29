/*
 ------------------------------------------------------------------------------------------------
 ofxKinectVision.h | ofxKinectVision | 26/01/2011
 
 created by MATT JAKOB - www.mattjakob.com
 Copyright 2011. All rights reserved.
 ------------------------------------------------------------------------------------------------
 */
#pragma once

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxBaseObject.h"
#include "ofxCvBlob.h"
#include "ofxVec2f.h"
#include "ofxFBOTexture.h"
#include "GlobalVariables.h"
#include "Singleton.h"
#include "Space.h"
//#include "ofxProxyImage.h"
//#include "ofxStitcher.h"
#include "ofxOpticalFlow.h"
#include "ofxSharedObject.h"
#include "SharedObjectManager.h"
#include "BlobManager.h"

/*
#define CONTROLPANEL_WIDTH 1440
#define CONTROLPANEL_HEIGHT 768
#define MODE_LIVE 1
#define MODE_REC 2
#define MODE_PLAYBACK 3
*/

#define FILTER_INPUT 0
#define FILTER_RANGE 1
#define FILTER_SUBTRACT_STATIC 2
#define FILTER_SUBTRACT_DYNAMIC 3
#define	FILTER_SMOOTH 4
#define FILTER_DILATE 5
#define FILTER_ERODE 6
#define FILTER_THRESHOLD 7
#define FILTER_HIGHPASS 8
#define FILTER_AMPLIFY 9
#define FILTER_COUNT 10

// kinect vision //////////////////////////////////////////////

class KinectVision{

private:	
	
	ofxCvShortImage floatImg; //< used for adaptive or dynamic subtraction
	
	//Mask
	ofxFBOTexture surface_fbo;
	ofxCvGrayscaleImage innermaskImg;
	ofxCvGrayscaleImage outermaskImg;
		
public:
	
	static GlobalVariables* global; //< also being used by objects in BlobManager
	
	//Optical flow
	ofxOpticalFlow opflow;
		
	SharedObjectManager* sharedObjectManager;
	ofxSharedObject<KinectShare>* sharedObject;	
	int camW, camH;
	
	BlobManager innerManager_;
	BlobManager* innerManager;
	BlobManager outerManager;
	
	//Image buffers
	//ofxCvColorImage colorImg;
	ofxCvGrayscaleImage cvImg;
	ofxCvGrayscaleImage cvImg_prev; //< used for optical flow
	ofxCvGrayscaleImage tmp2Img;
	
	ofxCvGrayscaleImage grayImg; //< input preview image
	ofxCvGrayscaleImage innerImg;
	ofxCvGrayscaleImage outerImg;
	ofxCvGrayscaleImage bg_staticImg;
	ofxCvGrayscaleImage bg_dynamicImg;
	
	ofxCvGrayscaleImage rangeImg;
	ofxCvGrayscaleImage subtractSImg;
	ofxCvGrayscaleImage subtractDImg;
	ofxCvGrayscaleImage smoothImg;
	ofxCvGrayscaleImage dilateImg;
	ofxCvGrayscaleImage erodeImg;
	ofxCvGrayscaleImage tresholdImg;
	ofxCvGrayscaleImage highpassImg;
	ofxCvGrayscaleImage amplifyImg;
	
	//Contour finder
	ofxCvContourFinder innerFinder, outerFinder;
	
	KinectVision();
	~KinectVision();
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void doSettingsUpdate();
	void doAllocation(int _camW, int _camH);
	void doFrame();
	void doFilters();
	void doFilter(int filter_name);
	
	void clearBackground();
	void snapBackground();
	
	void doContours();
	void doOpticalFlow();
	void doShareData();
	void draw();
	void exit();
	
	void updateAnchors(AnchorManager* anchorManager);
	
	void takeSnapShot(int filter_name);
	void takeBackgroundSnapshot(ofEventArgs& e);
	void takeBackgroundSnapshot();
	
};
