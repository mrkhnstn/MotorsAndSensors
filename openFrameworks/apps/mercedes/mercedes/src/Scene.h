#pragma once
/*
 *  Scene.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "FinCtrl.h"

class Scene : public ofxBaseObject {
	
public:
	
	FinCtrl* finCtrl;
	Constants* constants;
	
	virtual void setup(){
		name = "Scene";
		finCtrl = Singleton<FinCtrl>::instance();
		constants = Singleton<Constants>::instance();
	}
	
	virtual void setupGUI(){}
	
	virtual void postGUI(){}
	
	virtual void update(){}
	
	virtual void draw(){}
	
};
