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
	
	static float maxMotorSpeed; // in degrees per second
	bool doLimitSpeed;
	//static float maxAngleNSpeed;
	static float userOffDelayTime; // time after no user detection to consider user still in proximity (to eliminate flickering)
	
	static int globalPulseMin;
	static int globalPulseMax;
	
	static int calibrationMode; // 0: none, 1: pulse min, 2: pulse max
	int pulseMin;			// pulse for setting panels to front (0 degrees)
	int pulseMax;			// pulse for setting panels to back (180 degrees)
	int pulse;				// pulse corresponding to angle
	
	int		index;			// index within motor array
	//float	angleN;			// normalized angle	of motor rotation
	//float	tgtAngleN;
	
	float angle;			// actual angle of motor in degrees from 0 to 180
	float tgtAngle;			// targeted angle of motor in degrees from 0 to 180
	
	float	posAngle;		// position angle calculated out of index and numOfMotors
	ofxVec2f	pos;		// position of motor in relation centre of cylinder
	ofxVec2f	cartPos;	// position of motor converted to cartesian system used for cv
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
	
	int proximityValue; // if a user is detected then this value will be incremented otherwise it will be decremented
	static int maxProximityValue;
	static int proximityOnThreshold; // if proximity value of sensor is larger then proximityOnThreshold then a user is in proximity
	bool proximityUpdated;
	
	float relativePos; // position in relation to rays;
	vector<int> rayIds;
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	
	void setAngle(float angle);
	void setTgtAngle(float angle);
	float getAngle();
	ofxVec2f getPos();
	float getPosAngle();
	
	bool userInProximity();	// simple binary way of figuring out whether a user stands in front of this motor
	void incrementProximityValue(int i=1);
	void decrementProximityValue(int i=1);
	void userInProximityOn(); // switch on user in proximity
	void userInProximityOff(); // switch off user in proximity
	float userInProximityOnTime;
	float userInproximityOffTime;
	
protected:
	
	void updateSensors(); 	// populates / refreshes the list of sensors to be used for user proximity detection
	void setBackColor();
	void setFrontColor();
	void updateUserProximity();
	/*
	float getAngleN();
	float getTgtAngleN();
	void setTgtAngleN(float f);
	void setAngleN(float f);
	*/
	
	bool _userInProximity;
	
};
