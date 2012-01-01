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

#define SENSOR_BANKS_NUM	4
#define SENSORS_PER_BANK	9
#define SENSORS_NUM			SENSOR_BANKS_NUM * SENSORS_PER_BANK
#define RAYS_PER_SENSOR		5
#define TOTAL_RAYS			SENSOR_BANKS_NUM * SENSORS_PER_BANK * RAYS_PER_SENSOR

#define SENSOR_MAX_DISTANCE	300
#define SENSOR_OSC_TAG		"/adc" //TODO: change to /sensor

class SensorCtrl : public ofxBaseObject, ofxOscHandler {

public:
	
	Constants* constants;

	int rawValues[TOTAL_RAYS];			// raw 10-bit analog reading from sensors
	float distanceValues[TOTAL_RAYS];	// distance reading of each sensor in cm converted from rawValues
	float distanceLUT[1024];			// look up table used to convert rawValues into distanceValues;
	
	vector<Sensor*> sensors;
	
	void setup(){
		
		constants = Singleton<Constants>::instance();

		Singleton<ofxOscManager>::instance()->registerInterest(*this,"/adc");
		
		// calculate look up table
		// TODO: use graph from spec sheet
		for (int i=0; i<1024; i++) {
			distanceLUT[i] = ofMap(i, 60, 470, 300, 0, true);
		}
		
		for(int i=0; i<TOTAL_RAYS; i++)
		{
			rawValues[i] = 0;
			distanceValues[i] = SENSOR_MAX_DISTANCE;
		}
		
		int bank = 0;
		int ch = 0;
		for (int i=0; i<constants->numSensors; i++) {
			Sensor* sensor = new Sensor();
			sensor->index = i;
			sensor->setup();
			sensor->bank = bank;
			sensor->ch = ch;
			for(int j=0; j<RAYS_PER_SENSOR; j++){
				sensor->rawValues[j] = &rawValues[bank * SENSORS_PER_BANK * RAYS_PER_SENSOR + ch * RAYS_PER_SENSOR + j];
				sensor->distanceValues[j] = &distanceValues[bank * SENSORS_PER_BANK * RAYS_PER_SENSOR + ch * RAYS_PER_SENSOR + j];
			}
			
			sensors.push_back(sensor);
			
			ch++;
			if(ch >= SENSORS_PER_BANK) {
				ch = 0;
				bank++;
			}
		}
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("Sensors"));
		gui.addTitle("drawing");
		gui.addToggle("doDraw", Sensor::doDraw);
		gui.addToggle("doDrawRays", Sensor::doDrawRays);
		gui.addToggle("doDrawHitPoints", Sensor::doDrawHitPoints);
		gui.addToggle("doDrawLabels", Sensor::doDrawLabels);
		
		int min = 0;
		int max = 19;
		
		while (min < TOTAL_RAYS) {
			gui.page("Sensors").addPageShortcut(gui.addPage("SensorRays_"+ofToString(min)+"-"+ofToString(max-1)));
			gui.addTitle("raw");
			for(int i=min; i<max && i < TOTAL_RAYS; i++)
			{
				int sensor = (int)floor(i / RAYS_PER_SENSOR);
				gui.addSlider("raw_"+ofToString(sensor) + "-" + ofToString(i), rawValues[i], 0, 1023);
			}
			gui.addTitle("distance").setNewColumn(true);
			for(int i=min; i<max && i < TOTAL_RAYS; i++)
			{
				int sensor = (int)floor(i / RAYS_PER_SENSOR);
				gui.addSlider("distance_"+ofToString(sensor) + "-" + ofToString(i), distanceValues[i], 0, 300);
			}
			min += 20;
			max += 20;
		}
			
		
		
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->setupGUI();
	}
	
	void postGUI(){
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->postGUI();
	}
	
	void update(){
		updateDistanceValues();
		
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->update();
	}
	
	void updateDistanceValues(){
		for (int i=0; i<TOTAL_RAYS; i++) {
			int lookupValue = ofClamp(rawValues[i], 0, 1023);
			distanceValues[i] = distanceLUT[lookupValue];
		}
	}
	
	void draw(){
		for(int i=0; i<sensors.size(); i++)
			sensors[i]->draw();
	}
	
	void handleOscMessage(ofxOscMessage& message){
		if (message.getAddress() == SENSOR_OSC_TAG) {
			//TODO: check number of args
			
			int bank = message.getArgAsInt32(0);
			
			int offset = bank * SENSORS_PER_BANK * RAYS_PER_SENSOR;
			int argId = 1;
			
			for (int led=0; led<RAYS_PER_SENSOR; led++) {
				for(int sensor=0; sensor<SENSORS_PER_BANK; sensor++)
				{
					int index = offset + sensor * RAYS_PER_SENSOR + led;
					rawValues[index] = message.getArgAsInt32(argId);
					argId++;
				}
			}
			
		}
	}
	
	void getSensorsBetweenAngles(vector<Sensor*>& tempSensors, float startAngle, float endAngle){
	
		float range1Min = startAngle;
		float range1Max = startAngle > endAngle ? 360 : endAngle;
		
		float range2Min = startAngle > endAngle ? 0 : startAngle;
		float range2Max = startAngle > endAngle ? endAngle : startAngle;		
		
		for(int i=0; i<sensors.size(); i++)
		{
			Sensor& sensor = *sensors[i];
			if (sensor.posAngle >= range1Min && sensor.posAngle <= range1Max || sensor.posAngle >= range2Min && sensor.posAngle <= range2Max ) {
				tempSensors.push_back(&sensor);
			}
		}
	}
};