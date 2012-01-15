#pragma once
/*
 *  BlankScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 15/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this class can be used as a blue print to create new custom scenes
 just create a new C++ file, copy the code from here and replace BlankScene with the name of your class
*/
class BlankScene : public Scene {
	
public:
	
	void setup(){
		Scene::setup();
		name = "BlankScene";
	}
	
	void setupGUI(){
		Scene::setupGUI();
	}
	
	void postGUI(){
		Scene::postGUI();
	}
	
	void start(){
		Scene::start();
	}
	
	void update(){
		Scene::update();
	}
	
	void draw(){
		Scene::draw();
	}

	void stop(){
		Scene::stop();
	}
};
