#pragma once
/*
 *  ofxProjectionManagerXboxCtrl.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 23/03/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxXboxController.h"
#include "Singleton.h"
#include "ofxProjectionManager.h"


class ofxProjectionManagerXboxCtrl : public ofxBaseObject {

public:
	
	ofxXboxController* xboxController;
	ofxProjectionManager* projectionManager;
	
	int		joystickUpdateDelay;
	int		nextJoystickUpdate;
	float	joystickFactor;
	float	buttonFactor;
	
	ofxProjectionManagerXboxCtrl();
	
	void setup();
	
	void setupGUI();
	
	void addButtonListeners();
	
	void removeButtonListeners();
	
	void update();
	
	void enableCtrl();
	
	void disableCtrl();
	
	void homeButton(ofEventArgs& e);
		
	void backButton(ofEventArgs& e);
	
	void startButton(ofEventArgs& e);
	
	void yButton(ofEventArgs& e);
	
	void aButton(ofEventArgs& e);
	
	void xButton(ofEventArgs& e);
	
	void bButton(ofEventArgs& e);
	
	void dUp(ofEventArgs& e);
	
	void dDown(ofEventArgs& e);
	
	void dLeft(ofEventArgs& e);
	
	void dRight(ofEventArgs& e);
	
	void lbButton(ofEventArgs& e);

	void rbButton(ofEventArgs& e);
	
};