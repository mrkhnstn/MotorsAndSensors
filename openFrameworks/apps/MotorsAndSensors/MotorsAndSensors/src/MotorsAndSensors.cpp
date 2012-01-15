/*
 *  MotorsAndSensors.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MotorsAndSensors.h"

void MotorsAndSensors::setup(){
	cylinderRotation = 0;
	
	camera.setOrigin(OF_ORIGIN_ZERO);
	camera.perspective();
	camera.position();
	camera.eye();
	camera.up();
	
	addObject(*Singleton<ofxOscManager>::instance());
	Singleton<ofxOscManager>::instance()->port = 3334;
	
	addObject(*Singleton<MotorCtrl>::instance());
	addObject(*Singleton<OscMotorCtrl>::instance());
	addObject(*Singleton<SensorCtrl>::instance());
	addObject(*Singleton<SceneCtrl>::instance());
	
	// setup 3d scene
	ofxMaterialSpecular(120, 120, 120); //how much specular light will be reflect by the surface
	ofxMaterialShininess(50); //how concentrated the reflexion will be (between 0 and 128
	
	globalLight.ambient(50,50, 50);
	directionalLight.directionalLight(128, 128, 128, 1, 1, 1);
	
	ofxBaseContainer::setup();
}

void MotorsAndSensors::setupGUI(){
	
	ofxBaseContainer::setupGUI();
	
	gui.page(1).addPageShortcut(gui.addPage("3D"));
	
	gui.addColorPicker("bg_color", &bgColor.r, false, true);
	gui.addSlider("cylinder_rotation",cylinderRotation,0,360);
	
	gui.addTitle("camera");
	gui.addSlider("camera_x",cameraPos.x,-1000,1000);
	gui.addSlider("camera_y",cameraPos.y,-1000,1000);
	gui.addSlider("camera_z",cameraPos.z,-1000,1000);
	
	gui.addSlider("eye_x",cameraEye.x,-1000,1000);
	gui.addSlider("eye_y",cameraEye.y,-1000,1000);
	gui.addSlider("eye_z",cameraEye.z,-1000,1000);
	

}

void MotorsAndSensors::postGUI(){
	ofxBaseContainer::postGUI();
}

// void MotorsAndSensors::udpate(); // see ofxBaseContainer update

void MotorsAndSensors::draw(){
	
	ofBackground(bgColor.r, bgColor.g, bgColor.b);
	
	// update camera
	camera.lerpPosition(cameraPos, 0.1);
	camera.lerpEye(cameraEye, 0.1);
	
	// enter 3D
	glEnable(GL_DEPTH_TEST); //lights look weird if depth test is not enabled
	camera.place();
	ofxLightsOn();
	
	ofPushMatrix();
	ofRotate(cylinderRotation, 0, 1, 0);
	ofxBaseContainer::draw();
	
	ofPopMatrix();
	
	// exit 3D
	ofxLightsOff();
	camera.remove();
	glDisable(GL_DEPTH_TEST);
	
}

void MotorsAndSensors::exit(){
	delete Singleton<MotorCtrl>::instance();
	delete Singleton<OscMotorCtrl>::instance();
	delete Singleton<SceneCtrl>::instance();
	delete Singleton<SensorCtrl>::instance();
	delete Singleton<ofxOscManager>::instance();
	//delete Singleton<SerialTest>::instance();
}