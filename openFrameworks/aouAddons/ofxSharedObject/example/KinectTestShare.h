#pragma once
/*
 *  KinectShare.h
 *  TestBed_SharedMemory
 *
 *  Created by Mark Hauenstein on 03/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#define KINECT_WIDTH 640
#define KINECT_HEIGHT 480
#define KINECT_CHANNELS 1

class KinectTestShare {
public:
	unsigned char pixels[KINECT_WIDTH*KINECT_HEIGHT*KINECT_CHANNELS];
	float floatValue;
};