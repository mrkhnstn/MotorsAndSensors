/*
 *  SensorCtrl.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SensorCtrl.h"

void SensorCtrl::setup(){
	
	Singleton<ofxOscManager>::instance()->registerInterest(*this,"/adc");
	
	// calculate look up table
	updateLUT();
	
	
	for(int i=0; i<TOTAL_RAYS; i++)
	{
		rawValues[i] = 0;
		distanceValues[i] = SENSOR_MAX_DISTANCE;
		averageValues[i].setSize(3);
	}
	
	int bank = 0;
	int ch = 0;
	for (int i=0; i<Sensor::numSensors; i++) {
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

void SensorCtrl::updateLUT(){
	// TODO: visualize against spec sheet graph		
	float minDistance = 40;
	float maxDistance = 300;
	float deltaDistance = maxDistance - minDistance;
	float _pow = 3;
	
	for (int i=0; i<1024; i++){
		float n = ofMap(i, 60, 470, 1, 0, true);
		float m = pow(n,_pow);
		distanceLUT[i] = (int)ofMap(m, 0, 1, minDistance, maxDistance, true);
	}
}

void SensorCtrl::setupGUI(){
	gui.page(1).addPageShortcut(gui.addPage("Sensors"));
	
	gui.addSlider("userInProximityDistance", Sensor::userInProximityDistance, 40, 300);

	gui.addTitle("sensors");
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
	
	gui.setPage("Sensors");
	gui.addTitle("drawing");
	gui.addToggle("doDraw", Sensor::doDraw);
	gui.addToggle("doDrawRays", Sensor::doDrawRays);
	gui.addToggle("doDrawHitPoints", Sensor::doDrawHitPoints);
	gui.addToggle("doDrawLabels", Sensor::doDrawLabels);
	gui.addToggle("doDrawSensorThreshold", Sensor::doDrawSensorThreshold);
}

void SensorCtrl::postGUI(){

	for(int i=0; i<TOTAL_RAYS; i++)
	{
		rawValues[i] = 0;
		distanceValues[i] = SENSOR_MAX_DISTANCE;
	}
	
	for(int i=0; i<sensors.size(); i++)
		sensors[i]->postGUI();
}

void SensorCtrl::update(){
	updateDistanceValues();
	
	for(int i=0; i<sensors.size(); i++)
		sensors[i]->update();
}

void SensorCtrl::updateDistanceValues(){
	for (int i=0; i<TOTAL_RAYS; i++) {
		
		int lookupValue = ofClamp(rawValues[i], 0, 1023);
		averageValues[i].addValue(distanceLUT[lookupValue]);
		distanceValues[i] = averageValues[i].getAverage();
	}
}

void SensorCtrl::draw(){
	
	if(Sensor::doDrawSensorThreshold){
		ofPushStyle();
		ofPushMatrix();
		ofSetColor(128,128,128);
		ofSetLineWidth(1);
		ofNoFill();
		ofRotate(90, 1, 0, 0);
		ofSetCircleResolution(72);
		ofCircle(0, 0, Sensor::distanceToCentre + Sensor::userInProximityDistance + 25);
		ofPopMatrix();
		ofPopStyle();
	}
	
	for(int i=0; i<sensors.size(); i++)
		sensors[i]->draw();
}

void SensorCtrl::handleOscMessage(ofxOscMessage& message){
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

void SensorCtrl::getSensorsBetweenAngles(vector<Sensor*>& tempSensors, float startAngle, float endAngle){
	
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