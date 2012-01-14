#pragma once
/*
 *  PalindromeScene.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 31/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

/*
 this scene moves all motors in a palindrome fashion (back and forth) using a global speed
 */
class PalindromeScene : public Scene {
public:
	
	float angleN;
	float angleNSpeed;
	float dir;
	
	void setup(){
		Scene::setup();
		name = "PalindromeScene";
		
		angleN = 0;
		angleNSpeed = 0;
		dir = 1;
	}
	
	void setupGUI(){
		gui.page("Scenes").addPageShortcut(gui.addPage(name));
		gui.addSlider("angleN", angleN, 0, 1);
		gui.addSlider("angleNSpeed", angleNSpeed, 0, 0.1);
	}
	
	void update(){
		
		angleN += dir * angleNSpeed;
		if (angleN > 1) {
			angleN = 1 - (angleN - 1);
			dir = -1;
		} else if (angleN < 0) {
			angleN *= -1;
			dir = 1;
		}
		
		for (int i=0; i<motorCtrl->motors.size(); i++) {
			motorCtrl->motors[i]->setTgtAngleN(angleN);
		}
	}
	
	
};
