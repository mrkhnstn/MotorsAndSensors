/*
 *  Scene.cpp
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

void Scene::setup(){
	name = "Scene";
	motorCtrl = Singleton<MotorCtrl>::instance();
	sensorCtrl = Singleton<SensorCtrl>::instance();
	constants = Singleton<Constants>::instance();
}

void Scene::setupGUI(){}

void Scene::postGUI(){}

void Scene::update(){}

void Scene::draw(){}