#pragma once
/*
 *  SimpleInteractiveScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 01/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this scene opens all motors where a user proximity is detected
 */
class SimpleInteractiveScene : public Scene {
public:
		
	void setup(){
		Scene::setup();
		name = "SimpleInteractiveScene";
	}
	
	void update(){
		for (int i=0; i<getMotorCount(); i++)
			if (userInFrontOfMotor(i)) {
				panelOpen(i);
			} else {
				panelFront(i);
			}
	}

};


