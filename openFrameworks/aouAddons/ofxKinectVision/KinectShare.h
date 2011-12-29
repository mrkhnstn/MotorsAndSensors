#pragma once
/*
 *  KinectShare.h
 *  TestBed_SharedMemory
 *
 *  Created by Mark Hauenstein on 03/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#define KINECT_WIDTH		640
#define KINECT_HEIGHT		480
#define KINECT_CHANNELS		1
#define KINECT_COUNT		3 // number of kinects
#define KINECT_SHARE_PATH	"/tmp/MultiKinect"


#define KINECT_MAX_BLOB_COUNT 100
#define KINECT_MAX_CORNERS 500

#include "GlobalVariables.h"
#include "ofxSimpleGuiToo.h"
#include "BlobManager.h"

class Anchor {
public:
	int x,y;
	int w,h;
	int x_world, y_world;
	int w_world, h_world;
	int life;
	int state;
	int ghost;
	int id;
	int closestBorder;
	
	Anchor();
};

class AnchorManager {
public:
	Anchor inner[KINECT_MAX_BLOB_COUNT];
	int innerCount;
	
	Anchor outer[KINECT_MAX_BLOB_COUNT];
	int outerCount;
	
	Anchor opflow[KINECT_MAX_CORNERS];
	int opflowCount;
	
	AnchorManager();
};

class CompositeImage {
public:
	
	int width;
	int height;
	unsigned char pixels[1920 * 480];
	
	CompositeImage(){
		width = 1920;
		height = 480;
	}
};

class KinectSettings {
public:
	float	nearClipping;
	float	farClipping;
	float	cameraAngle;
	
	KinectSettings(){
		nearClipping = 768;
		farClipping = nearClipping + 255;
		cameraAngle = 0;
	}
};

/**
 used to share data capture from kinects
 */
class KinectShare {
	
public:
	
	unsigned char		pixels[KINECT_COUNT][KINECT_WIDTH*KINECT_HEIGHT*KINECT_CHANNELS];
	KinectSettings		kinectSettings[KINECT_COUNT];
	AnchorManager		anchorManager;
	CompositeImage		compositeImage;
	CompositeImage		inputImage;
	GlobalVariables		computerVision;
	//bool				stitchInComputerVision; //< if true then camera stitching happens in computer vision client, otherwise in render client
	//BlobManager		innerManager;
	
	void setupKinectSettingsGUI(){
		gui.page(1).addPageShortcut(gui.addPage("Kinect Cameras"));
		for (int i=0; i<KINECT_COUNT; i++) {
			gui.addTitle("Camera "+ofToString(i));
			KinectSettings& setting = kinectSettings[i];
			gui.addSlider("angle",setting.cameraAngle,-45,45);
			gui.addSlider("near clipping",setting.nearClipping,0,2048);
			gui.addSlider("far clipping",setting.farClipping,0,2048);
		}
	}
	
	void addPreviewImageCombobox(){
		string sharedImageTitles[11] = {
			"none",
			"input",
			"range",
			"subtract static",
			"subtract dynamic",
			"smooth",
			"dilate",
			"erode",
			"threshold",
			"highpass",
			"amplify"
		};
		gui.addComboBox("preview image", computerVision.shareImageId, 11, sharedImageTitles);		
	}
	
	void setupGlobalVariablesGUI(){ 
		
		ofxSimpleGuiPage& page_camera = gui.addPage("Kinect Vision");
		gui.page(1).addPageShortcut(page_camera);
		
		gui.addToggle("reload settings", computerVision.reloadSettings);
		gui.addTitle("camera stitcher");
		gui.addToggle("stitch in cv", computerVision.stitchInComputerVision);
		gui.addToggle("draw camera stitcher", computerVision.doDrawCameraStitcher);
		gui.addColorPicker("camera stitcher color", &computerVision.cameraStitcherColor.r, false, true);
		gui.addTitle("preview image");
		addPreviewImageCombobox();
		gui.addColorPicker("preview image color", &computerVision.previewImageColor.r, false, true);
		
		//gui.addTitle("Calibration");
		//gui.addToggle("Calibration mode", computerVision.calibrationMode);
		//gui.addSlider("Shift x", computerVision.shiftX, 0, ofGetWidth());
		//gui.addSlider("Shift y", computerVision.shiftY, 0, ofGetHeight());
		//gui.addSlider("Scale r", computerVision.scale, 0.0, 3.0);
		
		//gui.addTitle("HUD");//.setNewColumn(true);
		//gui.addToggle("Frame rate", computerVision.hud_framerate);
		//gui.addToggle("Blobs count", computerVision.hud_blobsCount);	
		//gui.addToggle("Parameters", computerVision.hud_parameters);
				
		//gui.addToggle("People detection", computerVision.hud_people);
		//gui.addToggle("Bounding boxes", computerVision.hud_blobsBoxes);
		
		
		ofxSimpleGuiPage& page_imgproc1 = gui.addPage("KV Processing");
		gui.page(1).addPageShortcut(page_imgproc1);
		
		//gui.addComboBox("share image", computerVision.shareImageId, 11, sharedImageTitles);
		addPreviewImageCombobox();
		
		gui.addTitle("Colour pass");
		gui.addSlider("c_min", computerVision.colourMin, 0, 255);
		gui.addSlider("c_max", computerVision.colourMax, 0, 255);
		
		gui.addTitle("Static subtract");
		gui.addSlider("Snapshot frame", computerVision.snapshotFrame, -1, 1000);
		gui.addDebug("Current frame", computerVision.currentSnapshotFrame);
		gui.addToggle("Take snapshot", computerVision.snap_background);
		gui.addButton("Clear snapshot", computerVision.reset_background);
		
		gui.addTitle("Smoothing");
		gui.addSlider("s_value",computerVision.smooth,0,100);
		
		gui.addTitle("Highpass");
		gui.addSlider("h_blur", computerVision.highpassBlur, 0, 100);
		gui.addSlider("h_noise", computerVision.highpassNoise, 0, 100);
		
		gui.addTitle("Amplify");
		gui.addSlider("a_value", computerVision.amplify, 0, 10);
		
		gui.addTitle("Treshold");
		gui.addSlider("t_value", computerVision.treshold, 0, 255);
		
		gui.addTitle("Dynamic subtract");	
		gui.addToggle("Dynamic learning", computerVision.dynamic_learning);
		gui.addSlider("Learning speed", computerVision.learning_speed, 1, 1000);
		
		gui.addTitle("Erode");
		gui.addSlider("e_value", computerVision.erode,0,100);
		
		gui.addTitle("Dilate");
		gui.addSlider("d_value", computerVision.dilate, 0, 100);
				
		// contour page ///
		
		ofxSimpleGuiPage& page_contour = gui.addPage("KV Contour Finder");
		gui.page(1).addPageShortcut(page_contour);
		
		//gui.addTitle("Hand blobs settings");
		gui.addToggle("enabled", computerVision.doInnerContours);

		gui.addTitle("Contour Settings");
		gui.addToggle("draw contours", computerVision.hud_blobsContours);
		gui.addSlider("min area", computerVision.innerMin,0,10000);
		gui.addSlider("max area", computerVision.innerMax,0,50000);
		gui.addSlider("max count", computerVision.innerMaxCount,1,100);
		gui.addSlider("hand points interval", computerVision.handPointsInterval,1,16);
		gui.addSlider("max hand points", computerVision.maxHandPoints, 1, 256);
		
		/*
		gui.addTitle("People blobs settings");
		gui.addToggle("enabled", computerVision.doOuterContours);
		gui.addSlider("p min area", computerVision.outerMin,0,10000);
		gui.addSlider("p max area", computerVision.outerMax,0,50000);
		gui.addSlider("p max count", computerVision.outerMaxCount,1,100);
		*/
		
		gui.addTitle("Blob Settings");
		
		gui.addToggle("draw", computerVision.hud_hands);
		gui.addToggle("draw info", computerVision.hud_handsInfo);
		//gui.addToggle("Parameters", computerVision.hud_parameters);
		//gui.addToggle("draw ext info", computerVision.hud_handsInfoExt);
		//computerVision.hud_handsInfoList;
		
		//gui.addToggle("draw search radius", computerVision.hud_handsSearchRadius);
		gui.addSlider("min life time", computerVision.minLife, 0, 100);
		gui.addSlider("max ghost time", computerVision.maxGhost, 0, 100);
		gui.addSlider("max search radius", computerVision.maxSearchRad, 0, 100);
		gui.addSlider("min movement", computerVision.minMovement, 0, 100);
		gui.addSlider("hand length", computerVision.handLength, 0, 300);
		
		/*
		gui.addTitle("Optical Flow");
		gui.addToggle("Activate Optical Flow", computerVision.doOpFlow);
		gui.addToggle("Normalise flow", computerVision.normalise);
		gui.addSlider("Min flow", computerVision.minFlow, 0, 200);
		gui.addSlider("Max flow", computerVision.maxFlow, 0, 200);
		*/
		
		gui.addTitle("Table surface");//.setNewColumn(true);	
		gui.addToggle("draw", computerVision.hud_tablesurface);
		gui.addSlider("TL_x",(computerVision.table).corners[0].x,0,640*4);
		gui.addSlider("TL_y",(computerVision.table).corners[0].y,0,480*4);
		gui.addSlider("BL_x",(computerVision.table).corners[3].x,0,640*4);
		gui.addSlider("BL_y",(computerVision.table).corners[3].y,0,480*4);
		gui.addSlider("TR_x",(computerVision.table).corners[1].x,0,640*4);
		gui.addSlider("TR_y",(computerVision.table).corners[1].y,0,480*4);
		gui.addSlider("BR_x",(computerVision.table).corners[2].x,0,640*4);
		gui.addSlider("Br_y",(computerVision.table).corners[2].y,0,480*4);
		
		gui.addPage("KV Debug Colors");
		//gui.page(1).addPageShortcut(page_contour);
		gui.addColorPicker("blob new", &computerVision.blobNewColor.r, false, true);
		gui.addColorPicker("blob active", &computerVision.blobActiveColor.r, false, true);
		gui.addColorPicker("blob ghost", &computerVision.blobGhostColor.r, false, true);
		gui.addColorPicker("contour", &computerVision.blobContourColor.r, false, true);
	}
};

