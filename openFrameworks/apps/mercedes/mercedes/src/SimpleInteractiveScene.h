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
 this scene opens all fins where a user proximity is detected
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
		for (int i=0; i<finCtrl->fins.size(); i++)
			if (finCtrl->fins[i]->userInProximity()) {
				finCtrl->fins[i]->setTgtAngleN(0.5);
			} else {
				finCtrl->fins[i]->setTgtAngleN(0);
			}
	}
	
	
};


