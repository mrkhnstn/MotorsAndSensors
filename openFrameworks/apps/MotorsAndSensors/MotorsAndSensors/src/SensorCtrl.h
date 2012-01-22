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

#define SENSOR_MAX_DISTANCE	300
#define SENSOR_OSC_TAG		"/sensor"

class SensorCtrl : public ofxBaseObject, ofxOscHandler {

public:

	int rawValues[TOTAL_RAYS];			// raw 10-bit analog reading from sensors
	AverageF averageValues[TOTAL_RAYS];
	float distanceValues[TOTAL_RAYS];	// distance reading of each sensor in cm converted from rawValues
	float distanceLUT[1024];			// look up table used to convert rawValues into distanceValues;
	
	vector<Sensor*> sensors;
	
	void setup();
	void setupGUI();
	void postGUI();
	void update();
	void draw();
	void draw3d();

	void updateLUT();
	void updateDistanceValues();
	void handleOscMessage(ofxOscMessage& message);
	void getSensorsBetweenAngles(vector<Sensor*>& tempSensors, float startAngle, float endAngle);
};