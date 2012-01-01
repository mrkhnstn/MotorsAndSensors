#pragma once
/*
 *  StepScene.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 31/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this scene moves all fins in regular intervals by 90 degrees
 */
class StepScene : public Scene {
public:
	
	int		state;		// 0: side 1, 1: open, 2: side 2
	float	interval;	// time in seconds to change state
	float	changeTime; // if ofElapsedTimef() > changeTime then change state
	
	void setup(){
		Scene::setup();
		name = "StepScene";

		state = 0;
		interval = 2;
		changeTime = ofGetElapsedTimef() + interval;
	}
	
	void setupGUI(){
		gui.page("Scenes").addPageShortcut(gui.addPage(name));
		gui.addSlider("interval", interval, 0.5, 3);
		gui.addSlider("state", state, 0, 2);
	}
	
	void update(){
		
		if (ofGetElapsedTimef() > changeTime) {
			
			// change state
			state++;
			if (state > 2) {
				state = 0;
			}
			
			// set next change time
			changeTime = ofGetElapsedTimef() + interval;
			
		}
		
		switch (state) {
			case 0:
				for (int i=0; i<finCtrl->fins.size(); i++)
					finCtrl->fins[i]->setTgtAngleN(0);
				break;
			case 1:
				for (int i=0; i<finCtrl->fins.size(); i++)
					finCtrl->fins[i]->setTgtAngleN(0.5);
				break;
			case 2:
				for (int i=0; i<finCtrl->fins.size(); i++)
					finCtrl->fins[i]->setTgtAngleN(1);
				break;
			default:
				break;
		}
		

		
	}
	
	
};


