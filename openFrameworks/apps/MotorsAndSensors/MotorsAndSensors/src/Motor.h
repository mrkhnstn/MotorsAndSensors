#pragma once
/*
 *  Motor.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class Sensor;

class Motor : public ofxBaseObject {
	
public:
	
	static float elapsedTime;
	static float maxMotorSpeed;
	static float maxAngleNSpeed;
	
	int		index;			// index within motor array
	float	angleN;			// normalized angle	of motor rotation
	float	tgtAngleN;
	float	posAngle;		// position angle calculated out of index and numOfMotors
	ofxVec2f	pos;		// position of motor in relation centre of cylinder
	int		bank;			// SD84 bank
	int		ch;				// SD84 channel
	
	static bool doDraw;		// global draw flag
	static bool doDraw3D;	// draw 3d box
	static bool doDraw2D;	// draw 2d on null plane
	static bool doDrawLabels; // draw ids of each motor
	
	static bool doStaticInit;
	static ofTrueTypeFont font;
	
	static ofColor backColorLow;	// back side color of motor when NO user in proximity
	static ofColor frontColorLow;	// front side color of motor when NO user in proximity
	
	static ofColor backColorHigh;	// back side color of motor when user in proximity
	static ofColor frontColorHigh;	// front side color of motor when user in proximity
	
	static float distanceToCentre;
	static float panelWidth;
	static int numMotors;
	
	static float sensorAngleRange; 
	// demotores which sensors shall be included for user proximity detection
	// minRange = posAngle - 0.5 * sensorAngleRange
	// maxRange = posAngle + 0.5 * sensorAngleRange
	
	vector<Sensor*> sensors; 
	// all sensors that shall be checked for user proximity detection
	// gets populated / refreshed using updateSensors() 
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	
	void setAngle(float angle);
	float getAngle();
	ofxVec2f getPos();
	float getPosAngle();
	
	bool userInProximity();	// simple binary way of figuring out whether a user stands in front of this motor
	
protected:
	
	void updateSensors(); 	// populates / refreshes the list of sensors to be used for user proximity detection
	void setBackColor();
	void setFrontColor();
	void updateUserProximity();
	void setUserInProximity(bool b);
	float getAngleN();
	float getTgtAngleN();
	void setTgtAngleN(float f);
	void setAngleN(float f);
	
	bool _userInProximity;
	
};
