#pragma once
/*
 *  IdleScene.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 31/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 - this scene does nothing
 - activate this scene if you want to manually test each motor
 */

class IdleScene : public Scene {
public:
	
	void setup(){
		Scene::setup();
		name = "IdleScene";
	}
};