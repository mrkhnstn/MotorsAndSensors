/*
 *  Scene.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "Scene.h"
#include "SceneCtrl.h"
#include "Singleton.h"

void Scene::setup(){
	name = "Scene";
	motorCtrl = Singleton<MotorCtrl>::instance();
	sensorCtrl = Singleton<SensorCtrl>::instance();
	enabled = false;
}

void Scene::setupGUI(){
	gui.page("Scenes").addPageShortcut(gui.addPage(name));
	gui.addDebug("enabled", enabled);
}

void Scene::postGUI(){}

void Scene::update(){}

void Scene::draw(){}

void Scene::start(){
	enabled = true;
}

void Scene::stop(){
	enabled = false;
}

int Scene::getMotorCount(){
	return motorCtrl->motors.size();
}

Motor& Scene::getMotor(int motorId){
	return *(motorCtrl->motors[motorId]); // no bounds checking!!!
}

// sets the angle of the motor in degress (0-180)
void Scene::setMotorAngle(int motorId, float angle){
	getMotor(motorId).setAngle(angle);
}

// gets the angle of the specified motor
float Scene::getMotorAngle(int motorId){
	return getMotor(motorId).getAngle();
}	

// sets the panel attached to the specified motor to 90 degress
void Scene::panelOpen(int motorId){
	setMotorAngle(motorId, 90);
}				

// sets the panel attached to the specified motor to 0
void Scene::panelFront(int motorId){
	setMotorAngle(motorId, 0);
}			

// sets the panel attached to the specified motor to 180
void Scene::panelBack(int motorId){
	setMotorAngle(motorId, 180);
}				

// gets the floor position of the motor in relation the centre of the cylinder
ofxVec2f Scene::getMotorPos(int motorId){
	return getMotor(motorId).getPos();
}

// gets the positional angle of the specified motor in relation to motor 0
float Scene::getMotorPosAngle(int motorId){
	return getMotor(motorId).getPosAngle();
}

// returns true if there is a user in front of the motor
bool Scene::userInFrontOfMotor(int motorId){
	return getMotor(motorId).userInProximity();
}


void Scene::add(Scene* scene){
	Singleton<SceneCtrl>::instance()->scenes.push_back(scene);
}