#pragma once
/*
 *  ThreeDTest.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 31/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "ofx3DUtils.h"

class ThreeDTest : public ofxBaseObject {

public:
	
	ofxCamera camera;
	ofxVec3f cameraPos;
	ofxVec3f cameraEye;
	
	ofxVec3f boxPos;
	
	float boxSize;
	float boxRotation;
	
	ofxLight globalLight;
	ofxLight directionalLight;
	
	void setup(){
		boxSize = 100;
		boxRotation = 0;
		//reflexions!!
		ofxMaterialSpecular(120, 120, 120); //how much specular light will be reflect by the surface
		ofxMaterialShininess(50); //how concentrated the reflexion will be (between 0 and 128
		
		globalLight.ambient(50,50, 50);
		directionalLight.directionalLight(128, 128, 128, 1, 1, 1);
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("ThreeDTest"));
		
		gui.addTitle("camera");
		gui.addSlider("camera_x",cameraPos.x,-1000,1000);
		gui.addSlider("camera_y",cameraPos.y,-1000,1000);
		gui.addSlider("camera_z",cameraPos.z,-1000,1000);
		
		gui.addSlider("eye_x",cameraEye.x,-1000,1000);
		gui.addSlider("eye_y",cameraEye.y,-1000,1000);
		gui.addSlider("eye_z",cameraEye.z,-1000,1000);
		
		gui.addTitle("box");
		gui.addSlider("box_x",boxPos.x,-1000,1000);
		gui.addSlider("box_y",boxPos.y,-1000,1000);
		gui.addSlider("box_z",boxPos.z,-1000,1000);
		gui.addSlider("box_size",boxSize,-1000,1000);
		gui.addSlider("box_rotation",boxRotation,0,360);
	}
	
	void postGUI(){
		cameraPos = camera.getPosition();
		cameraEye = camera.getEye();
	}
	
	void update(){}
	
	void draw(){
		glEnable(GL_DEPTH_TEST); //lights look weird if depth test is not enabled
	
		ofPushMatrix();
		
		camera.lerpPosition(cameraPos, 0.1);
		camera.lerpEye(cameraEye, 0.1);

		
		camera.place();
		
		ofxLightsOn();
		
		ofSetColor(255, 255, 0);
		
		ofPushMatrix();
		ofTranslate(boxPos.x, boxPos.y, boxPos.z);
		ofRotate(boxRotation, 0, 1, 0);
		ofxBox(0, 0, 0, boxSize);
		ofPopMatrix();
		
		ofRect(-500,-500,1000,1000);
		
		ofxLightsOff();
		
		glDisable(GL_DEPTH_TEST);
		
		ofPopMatrix();
		
		camera.remove();
		
		//ofSetupScreen();
	}
	
	
	
};