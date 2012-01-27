#pragma once

/*
 *  SensorCtrl.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Sensor.h"
#include "ofxOscManager.h"
#include "AverageF.h"

#define SENSOR_BANKS_NUM	4
#define SENSORS_PER_BANK	9
#define RAYS_PER_SENSOR		5
#define SENSORS_NUM			SENSOR_BANKS_NUM * SENSORS_PER_BANK
#define RAYS_PER_BANK		SENSORS_PER_BANK * RAYS_PER_SENSOR
#define TOTAL_RAYS			SENSOR_BANKS_NUM * SENSORS_PER_BANK * RAYS_PER_SENSOR

#define RAY_GAP 1.0
#define MOTOR_GAP 3.0 * RAY_GAP
#define SENSOR_OFFSET 0.5 * MOTOR_GAP
#define SENSOR_GAP 2.0 * MOTOR_GAP
#define RAY_OFFSET SENSOR_OFFSET - 2.0 * RAY_GAP

#define SENSOR_MAX_DISTANCE	300
#define SENSOR_OSC_TAG		"/sensor"

class SensorCtrl : public ofxBaseObject, ofxOscHandler {

public:

	int rawValues[TOTAL_RAYS];			// raw 10-bit analog reading from sensors
	AverageF averageValues[TOTAL_RAYS];
	float bgSubtract[TOTAL_RAYS];		// dynamically adapter bg values
	float adaptedValues[TOTAL_RAYS];	// bg subtracted values
	float distanceValues[TOTAL_RAYS];	// distance reading of each sensor in cm converted from rawValues
	float hitThreshold[TOTAL_RAYS];		// if the distance value of a ray is larger then its threshold then the ray is considered to be hitting something in the first instance
	float hitScore[TOTAL_RAYS];			// the hitscore per sensor is calculated out of how steadily the distance values are larger then the threshold and also by the amount that they are larger then the threshold
	bool  hit[TOTAL_RAYS];				// if true then sensor is considered to be hit (final)
	bool rayEnabled[TOTAL_RAYS];		// if false then disable ray
	float distanceLUT[1024];			// look up table used to convert rawValues into distanceValues;
	float rayPos[TOTAL_RAYS];				// position of sensors
	
	bool doAveraging;
	bool doLUT;
	float minDistance;
	float maxDistance;
	float distancePow;
	float analogMin;
	float analogMax;
	
	float bgAdaptFactor;
	float _bgAdaptFactor;
	float adaptLerpStrength;
	float adjLerpStrength;
	float globalHitThreshold; //TODO: calculate each hit threshold out of maximum and minimum values of each sensor
	float globalHitScoreThreshold;
	float maxHitScore;
	float hitScoreIncFactor;
	float hitScoreDecFactor;
	
	int numOfRaysHit;
	int numOfRaysHitThresh;
	int numOfAdjacentRaysHit;
	int numOfAdjacentRaysHitThresh;
	int numOfRaysHitWindowSize;
	int* numOfRaysHitWindow;
	int numOfRaysHitIndex;
	int numOfRaysHitInWindow;
	int numOfRaysHitInWindowThresh;
						   
	vector<Sensor*> sensors;
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	void draw3d();

	void updateLUT();
	void processSensorReadings();
	void handleOscMessage(ofxOscMessage& message);
	void getSensorsBetweenAngles(vector<Sensor*>& tempSensors, float startAngle, float endAngle);
	
	void enableAllSensors(ofEventArgs& e);
	void disableAllSensors(ofEventArgs& e);
	
	bool _userInProximity;
	bool userInProximity(); // returns true if a user is in proximity 
};