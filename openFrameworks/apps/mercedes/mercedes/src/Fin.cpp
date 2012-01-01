/*
 *  Fin.cpp
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Fin.h"
#include "SensorCtrl.h"

float Fin::elapsedTime = 0;
float Fin::maxMotorSpeed = 0.3;
float Fin::maxAngleNSpeed = 1;

bool Fin::doDraw = true;		// global draw flag
bool Fin::doDraw3D = true;		// draw 3d box
bool Fin::doDraw2D = false;		// draw 2d on null plane
bool Fin::doDrawLabels = false; // draw ids of each fin

ofColor Fin::backColorLow;		// back side color of fin when NO user in proximity
ofColor Fin::frontColorLow;		// front side color of fin when NO user in proximity

ofColor Fin::backColorHigh;		// back side color of fin when user in proximity
ofColor Fin::frontColorHigh;	// front side color of fin when user in proximity

float Fin::sensorAngleRange = 10; 

void Fin::updateSensors(){
	sensors.clear();
	
	float minAngle = posAngle - 0.5 * sensorAngleRange;
	if(minAngle < 0)
		minAngle += 360;
	
	float maxAngle = posAngle + 0.5 * sensorAngleRange;
	if(minAngle > 360)
		minAngle -= 360;
	
	Singleton<SensorCtrl>::instance()->getSensorsBetweenAngles(sensors,minAngle,maxAngle);
}

void Fin::updateUserProximity(){
	_userInProximity = false;
	for(int i=0; i<sensors.size(); i++)
		if (sensors[i]->userInProximity()) {
			_userInProximity = true;
			break;
		}
}