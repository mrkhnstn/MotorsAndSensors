/*
 *  KinectShare.cpp
 *  TestBed_SharedMemory
 *
 *  Created by Mark Hauenstein on 03/02/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "KinectShare.h"

Anchor::Anchor()
{
	x = x_world = y = y_world = h = w = h_world = w_world = id = life = 0;
}

AnchorManager::AnchorManager()
{
	innerCount = outerCount = opflowCount = 0;
	//inner.reserve(MAX_BLOB_COUNT_LIMIT);
	//outer.reserve(MAX_BLOB_COUNT_LIMIT);
	//opflow.reserve(MAX_CORNERS);
}
