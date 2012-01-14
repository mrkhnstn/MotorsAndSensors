#pragma once
/*
 *  SimpleInteractiveScene.h
 *  mercedes
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
	
	void setupGUI(){
		gui.page("Scenes").addPageShortcut(gui.addPage(name));
	}
	
	void update(){
		for (int i=0; i<motorCtrl->motors.size(); i++)
			if (motorCtrl->motors[i]->userInProximity()) {
				motorCtrl->motors[i]->setTgtAngleN(0.5);
			} else {
				motorCtrl->motors[i]->setTgtAngleN(0);
			}
	}
	
	
};


