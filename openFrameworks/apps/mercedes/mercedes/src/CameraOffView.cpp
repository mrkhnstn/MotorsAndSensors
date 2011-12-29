/*
 *  CameraOffView.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 11/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CameraOffView.h"
#include "SelfridgesApp.h"

void CameraOffView::setup(){
	
	// setup view //
	UIContainer::setup();
	name = "CameraOffView";

	// setup UI elements //
	cameraOnButton.setup();
	cameraOnButton.name = "cameraOnButton";
	cameraOnButton.label = "camera on";
	cameraOnButton.isInteractive = true;
	ofAddListener(cameraOnButton.onPressed,Singleton<SelfridgesApp>::instance(), &SelfridgesApp::returnFromCameraOffView);
	addElement(cameraOnButton);
	
	cameraOffSprite.setup();
	cameraOffSprite.name = "cameraOffSprite";
	cameraOffSprite.label = "camera off icon";
	addElement(cameraOffSprite);
}