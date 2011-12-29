#pragma once
/*
 *  BlobManager.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 19/04/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOpenCv.h"
#include "ofxVectorMath.h"

#define MAX_BLOB_COUNT_LIMIT 100
#define BLOB_PROCESSED 1
#define BLOB_GHOST 2
#define BLOB_NEW 3
#define BLOB_STATIC 4
#define BLOB_MOVEMENT 5

#define BlobManager_BORDER_TOP 0
#define BlobManager_BORDER_BOTTOM 1
#define BlobManager_BORDER_LEFT 2
#define BlobManager_BORDER_RIGHT 3

// hand //////////////////////////////////////////////

class Hand{
public:
	int x,y;
	int w,h;
	int x_world, y_world;
	int w_world, h_world;
	int closest_border;
	
	Hand();
	void update(ofxCvBlob* blob);
	void drawContour();
	ofxVec2f getCentroid(ofxCvBlob* blob, int base, int border);
private:
	
	ofxVec2f p,r1,r2,r3,r4,c1,c2,c3,c4;
	ofPoint vertices[500];
	int vertices_count;
};

// blob //////////////////////////////////////////////

class Blob{
	
public:
	
	int id;
	
	int x,y;
	int w,h;
	int x_world, y_world;
	int w_world, h_world;
	
	Hand hand;
	int ghost;
	int life;
	int state;
	
	//bool isHand;
	
	ofxCvBlob cvBlob;
	ofColor color;
	
	Blob();
	void set(ofxCvBlob* _cvBlob, int _id=-1, int _state=BLOB_NEW);
	void set(int _state);
	void set(Blob* newBlob, Blob* oldBlob, int _state);
	void drawContour();
	void drawBox();
};

// blob manager //////////////////////////////////////////////

class BlobManager{
public:
	Blob blobs[MAX_BLOB_COUNT_LIMIT];
	int blobsCount;
	int countN, countG, countA;
	
	BlobManager();
	void update();
	void add(ofxCvBlob* _cvBlob, bool _isHand = true);
	void drawDebug(int camW, int camH, int stageW, int stageH, int ghost, int life);
	void drawHands();
	void drawCounts();
	void drawHandsInfo();
	void drawHandsInfoList();
	void drawParameters();
	
private:
	Blob prev_blobs[MAX_BLOB_COUNT_LIMIT];
	Blob new_blobs[MAX_BLOB_COUNT_LIMIT];
	int prev_blobsCount;
	int new_blobsCount;
	int IDcounter;
};