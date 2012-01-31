#pragma once
/*
 *  Scene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "MotorCtrl.h"
#include "SensorCtrl.h"

class Scene : public ofxBaseObject {
	
public:
	
	MotorCtrl*	motorCtrl;	// points to MotorCtrl singleton
	SensorCtrl* sensorCtrl; // points to SensorCtrl singleton
	float elapsedTime;
	float startTime;
	
	Scene();
	virtual void setup();	
	virtual void setupGUI();	
	virtual void postGUI();
	virtual void update();
	virtual void draw();
	
	virtual void start();
	virtual void stop();
	
	int getMotorCount();							// returns the number of available motors  
	Motor& getMotor(int motorId);					// returns the motor object with the specified motor id
	void setMotorAngle(int motorId, float angle);	// sets the angle of the motor in degress (0-180)
	float getMotorAngle(int motorId);				// gets the angle of the specified motor
	void panelOpen(int motorId);					// sets the panel attached to the specified motor to 90 degress
	void panelFront(int motorId);					// sets the panel attached to the specified motor to 0
	void panelBack(int motorId);					// sets the panel attached to the specified motor to 180
	ofxVec2f getMotorPos(int motorId);				// gets the floor position of the motor in relation the centre of the cylinder
	float getMotorPosAngle(int motorId);			// gets the positional angle of the specified motor in relation to motor 0
	bool userInFrontOfMotor(int motorId);			// returns true if there is a user in front of the motor
	
	static void add(Scene* scene);
};
