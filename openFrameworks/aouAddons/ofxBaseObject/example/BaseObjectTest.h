#pragma once

/*
 *  BaseObjectTest.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 12/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "MathUtil2.h"
#include "Singleton.h"
#include "Lerp.h"

/**
 a simple example explaining how to use base object
 */

class BaseObjectTest : public ofxBaseObject {

public:
	
	float x;
	float y;
	float velX;
	float velY;
	float radius;
	string text;
	
	BaseObjectTest(){
		name = "BaseObjectTest";
		x = 0;
		y = 0;
		radius = 0;
		log("constructed");
		text = "";
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage(name)); //< this creates a page and puts a page shortcut onto the first page
		gui.addButton("start", (ofxBaseObject*)this, &ofxBaseObject::_start); 
		gui.addButton("stop", (ofxBaseObject*)this, &ofxBaseObject::_stop);
		gui.addToggle("enabled", enabled);
		gui.addDebug("x", x); //< addDebug only displays a value, works with integers, strings and floats
		gui.addDebug("y", y);
		gui.addSlider("velocity x", velX, 0, 10);
		gui.addSlider("velocity y", velY, 0, 10);
		gui.addSlider("radius", radius, 0, 100);
		gui.addTextInput("text", text);
	}
	
	void update(){
		if(!enabled) return;
		x = niceMod(x + velX, ofGetWidth());
		y = niceMod(y + velY, ofGetHeight());
	}
	
	void draw(){
		if(!enabled) return;
		ofSetColor(255, 255, 255);
		ofFill();
		ofCircle(x, y, radius);
	}
	
};