#pragma once

/*
 *  SensorCtrl.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "Sensor.h"
#include "ofxOscManager.h"



class SensorCtrl : public ofxBaseObject, ofxOscHandler {

public:
	
	Constants* constants;

	int rawValues[36*5];
	
	vector<Sensor*> sensors;
	
	void setup(){
		
		constants = Singleton<Constants>::instance();

		Singleton<ofxOscManager>::instance()->registerInterest(*this,"/adc");
		
		for(int i=0; i<36*5; i++)
		{
			rawValues[i] = 20;
		}
		
		int bank = 0;
		int ch = 0;
		for (int i=0; i<constants->numSensors; i++) {
			Sensor* sensor = new Sensor();
			sensor->index = i;
			sensor->setup();
			sensor->bank = bank;
			sensor->ch = ch;
			for(int j=0; j<5; j++)
				sensor->rawValues[j] = &rawValues[bank * 9 * 5 + ch * 5 + j];
				
			sensors.push_back(sensor);
			
			ch++;
			if(ch >= 9) {
				ch = 0;
				bank++;
			}
		}
	}
	
	void setupGUI(){
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->setupGUI();
	}
	
	void postGUI(){
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->postGUI();
	}
	
	void update(){
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->update();
	}
	
	void draw(){
		
		//ofPushMatrix();
		//ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0);
		
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->draw();
		
		//ofPopMatrix();
	}
	
	void handleOscMessage(ofxOscMessage& message){
		if (message.getAddress() == "/adc") {
			int bank = message.getArgAsInt32(0);
			
			int offset = bank * 9 * 5;
			int argId = 1;
			
			for (int led=0; led<5; led++) {
				for(int sensor=0; sensor<9; sensor++)
				{
					int index = offset + sensor * 5 + led;
					rawValues[index] = message.getArgAsInt32(argId);
					argId++;
				}
			}
			
		}
	}
};