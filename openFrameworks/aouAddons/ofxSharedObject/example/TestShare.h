#pragma once
/*
 *  TestShare.h
 *  TestBed_SharedMemory
 *
 *  Created by Mark Hauenstein on 03/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define IMG_CHANNELS 3

class TestShare {
public:
	unsigned char pixels[IMG_WIDTH*IMG_HEIGHT*IMG_CHANNELS];
	float floatValue;
};