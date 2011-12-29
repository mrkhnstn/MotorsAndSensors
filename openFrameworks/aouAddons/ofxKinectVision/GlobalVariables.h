/*
------------------------------------------------------------------------------------------------
   GlobalVariables.h | ofxKinectVision | 27/01/2011
 
   created by MATT JAKOB - www.mattjakob.com
   Copyright 2011. All rights reserved.
------------------------------------------------------------------------------------------------
*/
#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"
#include "Space.h"

class GlobalVariables
{
public:
	
	int snapshotFrame; // oF frame after startup on which to take snapshot
	int currentSnapshotFrame;
	
	int frameRate;
	
	bool speedMode;
	
	//Spaces
	int camW,camH, stageW,stageH;
	CoordSpace camera, projector, table;
	
	//Camera settings
	int  kAngle;	
	bool kLive;
	bool kRecord;
	bool kPositioning;
	//HUD
	bool hud_background;
	bool hud_dynamicBG;
	bool hud_feed;
	bool hud_blobsCount;
	bool hud_framerate;
	bool hud_parameters;
	bool hud_tablesurface;
	bool hud_hands;
	bool hud_handsInfo;
	bool hud_people;
	bool hud_blobsBoxes;
	bool hud_blobsContours;
	bool hud_handsInfoExt;
	bool hud_handsInfoList;
	bool hud_handsSearchRadius;
	
	//CV parameters
	bool dynamic_learning;
	bool snap_background;
	bool reset_background;
	int learning_speed;
	int colourMin, colourMax;
	int treshold;
	int smooth;
	int erode;
	int dilate;
	int highpassBlur;
	int highpassNoise;
	float amplify;
	int innerMin, innerMax;
	int innerMaxCount;
	int outerMin, outerMax;
	int outerMaxCount;
	int minLife;
	int maxGhost;
	int maxSearchRad;
	int minMovement;
	int handLength;
	bool doInnerContours;
	bool doOuterContours;
	
	//Optical flow parameters
	bool doOpFlow;
	bool normalise;
	float minFlow;
	float maxFlow;
	
	int proxyMode1,proxyMode2,proxyMode3;
	
	bool calibrationMode;
	int shiftX, shiftY;
	float scale;
	
	bool reloadSettings;
	
	int shareImageId;
	bool stitchInComputerVision;
	bool doDrawCameraStitcher;
	ofColor previewImageColor;
	ofColor	cameraStitcherColor;
	
	int handPointsInterval;		// number of segments to jump in hand detection
	int maxHandPoints;	// max number of hand segments to use in hand detection 

	ofColor blobNewColor;
	ofColor blobActiveColor;
	ofColor blobGhostColor;
	ofColor blobContourColor;
	
};