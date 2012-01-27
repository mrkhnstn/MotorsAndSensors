/*
 *  SensorCtrl.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 29/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SensorCtrl.h"
#include "MotorsAndSensors.h"

void SensorCtrl::setup(){
	
	Singleton<ofxOscManager>::instance()->registerInterest(*this,SENSOR_OSC_TAG);
	
	minDistance = 40;
	maxDistance = 300;
	distancePow = 3;
	//analogMin = 40;
	//analogMax = 470;
	analogMin = 0;
	analogMax = 1024;
	bgAdaptFactor = 0.1;
	_bgAdaptFactor = 1;
	adaptLerpStrength = 0.1;
	globalHitThreshold = 512;
	maxHitScore = 50;
	hitScoreIncFactor = 0.1;
	hitScoreDecFactor = 0.1;
	
	numOfRaysHit = 0;
	numOfRaysHitThresh = 3;
	numOfAdjacentRaysHit = 0;
	numOfAdjacentRaysHitThresh = 1;
	
	numOfRaysHitWindowSize = 30;
	numOfRaysHitWindow = new int[numOfRaysHitWindowSize];
	for(int i=0; i<numOfRaysHitWindowSize; i++)
		numOfRaysHitWindow[i] = 0;
	numOfRaysHitIndex = 0;
	numOfRaysHitInWindow = 0;
	numOfRaysHitInWindowThresh = 10;
	
	// calculate look up table
	updateLUT();
	
	
	for(int i=0; i<TOTAL_RAYS; i++)
	{
		rawValues[i] = 0;
		adaptedValues[i] = 0;
		distanceValues[i] = SENSOR_MAX_DISTANCE;
		averageValues[i].setSize(1);
		rayEnabled[i] = true;
		bgSubtract[i] = 0;
		hitThreshold[i] = globalHitThreshold;
		hitScore[i] = 0;
		hit[i] = false;
		rayPos[i] = RAY_OFFSET + i * RAY_GAP;
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

void SensorCtrl::setupGUI(){
	//TODO: clean up
	
	gui.page(1).addPageShortcut(gui.addPage("Sensors"));
	
	gui.addDebug("userInProximity", _userInProximity);
	gui.addSlider("numOfRaysHit",numOfRaysHit,0,128);
	gui.addSlider("numOfRaysHitThresh",numOfRaysHitThresh,0,128);
	gui.addSlider("numOfAdjacentRaysHit",numOfAdjacentRaysHit,0,128);
	gui.addSlider("numOfAdjacentRaysHitThresh",numOfAdjacentRaysHitThresh,0,128);
	gui.addSlider("numOfRaysHitInWindow",numOfRaysHitInWindow,0,256);
	gui.addSlider("numOfRaysHitInWindowThresh",numOfRaysHitInWindowThresh,0,256);
	gui.addSlider("userInProximityDistance", Sensor::userInProximityDistance, 40, 300);
	gui.addSlider("distanceToCentre", Sensor::distanceToCentre, 250, 350);
	gui.addSlider("angleBetweenRays", Sensor::angleBetweenRays, 5, 10);
	
	//gui.page("Sensors").addPageShortcut(gui.addPage("Sensors_Enabled"));
	gui.addButton("enable all rays", this, &SensorCtrl::enableAllSensors);
	gui.addButton("disable all rays", this, &SensorCtrl::disableAllSensors);
	
	gui.addSlider("analogMin", analogMin, 0, 1024);
	gui.addSlider("analogMax", analogMax, 0, 1024);
	
	gui.addSlider("minDistance", minDistance, 0, 400);
	gui.addSlider("maxDistance", maxDistance, 0, 400);
	
	gui.addSlider("adaptLerpStrength", adaptLerpStrength, 0.001, 0.5);
	gui.addSlider("adjacentLerpStrength", adjLerpStrength, 0.001, 0.5);
	gui.addSlider("bgAdaptFactor", bgAdaptFactor, 0, 0.1);
	gui.addSlider("hitThreshold", globalHitThreshold, 0, 1024);
	gui.addSlider("hitScoreThreshold", globalHitScoreThreshold, 1, 1024);
	gui.addSlider("maxHitScore", maxHitScore, 1, 1024);
	gui.addSlider("hitScoreIncFactor", hitScoreIncFactor,0.01,1);
	gui.addSlider("hitScoreDecFactor", hitScoreDecFactor,0.01,1);
	
	gui.addSlider("distancePow", distancePow, 1, 5);
	gui.addToggle("doLUT", doLUT);
	gui.addToggle("doAveraging", doAveraging);
	
	
	/*
	for(int i=0; i<sensors.size(); i++){
		gui.addToggle("sensor_"+ofToString(i), sensors[i]->enabled);
	}
	*/
	
	gui.addTitle("sensors");
	int min = 0;
	int max = 19;
	while (min < TOTAL_RAYS) {
		int minSensor = (int)floor(min / RAYS_PER_SENSOR);
		int maxSensor = (int)floor(max / RAYS_PER_SENSOR);
		
		gui.page("Sensors").addPageShortcut(gui.addPage("Sensors_"+ofToString(minSensor)+"-"+ofToString(maxSensor)));
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
		gui.addTitle("enabled").setNewColumn(true);
		for(int i=min; i<max && i < TOTAL_RAYS; i++)
		{
			int sensor = (int)floor(i / RAYS_PER_SENSOR);
			gui.addToggle("enabled_"+ofToString(sensor) + "-" + ofToString(i), rayEnabled[i]);
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
	gui.addToggle("drawOnlyInSensorZone", Sensor::drawOnlyInSensorZone);
	//gui.addToggle("doDrawSensorThreshold", Sensor::doDrawSensorThreshold);
}

void SensorCtrl::enableAllSensors(ofEventArgs& e){
	//for (int i=0; i<sensors.size(); i++) {
//		sensors[i]->enabled = true;
//	}
	for (int i=0; i<TOTAL_RAYS; i++) {
		rayEnabled[i] = true;
	}
}

void SensorCtrl::disableAllSensors(ofEventArgs& e){
	//for (int i=0; i<sensors.size(); i++) {
//		sensors[i]->enabled = false;
//	}
	for (int i=0; i<TOTAL_RAYS; i++) {
		rayEnabled[i] = false;
	}
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
	updateLUT();
	processSensorReadings();
	
	// check overall interactivity
	numOfRaysHit = 0;
	numOfAdjacentRaysHit = 0;
	for (int i=0; i<TOTAL_RAYS; i++) {
		int left = (i == 0) ? TOTAL_RAYS-1 : i - 1;
		//int right = (i == TOTAL_RAYS-1) ? 0 : i + 1;
		if (hit[i]) {
			numOfRaysHit++;
		}
		if (hit[left] && hit[i]) {
			numOfAdjacentRaysHit++;
		}
	}
	
	// calculate number of rays hit in time window
	numOfRaysHitWindow[numOfRaysHitIndex] = numOfRaysHit;
	numOfRaysHitIndex = (numOfRaysHitIndex + 1) % numOfRaysHitWindowSize;
	numOfRaysHitInWindow = 0;
	for(int i=0; i<numOfRaysHitWindowSize; i++)
		numOfRaysHitInWindow += numOfRaysHitWindow[i];
	
	// determine whether there is a  user in proximity
	_userInProximity = numOfRaysHit >= numOfRaysHitThresh || numOfAdjacentRaysHit >= numOfAdjacentRaysHitThresh || numOfRaysHitInWindow >= numOfRaysHitInWindowThresh;
	
	
	// update sensors
	for(int i=0; i<sensors.size(); i++)
		sensors[i]->update();
}

void SensorCtrl::updateLUT(){
	// TODO: visualize against spec sheet graph
	
	//float deltaDistance = maxDistance - minDistance;
	//float _pow = 3;
	if(doLUT){
		for (int i=0; i<1024; i++){
			//float analogMin = 40;
			//float analogMax = 470;
			float n = ofMap(i, analogMin, analogMax, 1, 0, true);
			float m = pow(n,distancePow);
			distanceLUT[i] = (int)ofMap(m, 0, 1, minDistance, maxDistance, true);
		}
	} else {
		for (int i=0; i<1024; i++) {
			distanceLUT[i] = (int)ofMap(i, 0, 1024, minDistance, maxDistance, true);
		}
	}
}

void SensorCtrl::processSensorReadings(){
	
	if (_bgAdaptFactor > bgAdaptFactor) {
		_bgAdaptFactor -= 0.01;
	} else {
		_bgAdaptFactor = bgAdaptFactor;
	}
	
	for (int i=0; i<TOTAL_RAYS; i++) {
		
		float rawValue = ofClamp(rawValues[i], 0, 1023);
		
		float bgSubValue = rawValue - bgSubtract[i];
		if(bgSubValue < 0) bgSubValue = 0;
		
		// lerp adapted value towards bgSubValue
		int left = (i == 0) ? TOTAL_RAYS-1 : i - 1;
		int right = (i == TOTAL_RAYS-1) ? 0 : i + 1;
		adaptedValues[i] = adaptedValues[i] + (bgSubValue-adaptedValues[i])*adaptLerpStrength + (distanceValues[left] - adaptedValues[i])*adjLerpStrength +  (distanceValues[right] - adaptedValues[i])*adjLerpStrength;
		
		
		// dynamically adapt bg values
		bgSubtract[i] = bgSubtract[i] * (1-_bgAdaptFactor) + rawValue * _bgAdaptFactor;
		
		//averageValues[i].addValue(bgSubValue);
		
		if (rayEnabled[i]) {
		
			distanceValues[i] = adaptedValues[i];
			
			hitThreshold[i] = globalHitThreshold;
			
			float thresholdDelta = distanceValues[i] - hitThreshold[i];
			
			// TODO: implement hit score calculation
			// depending on amount of thresholdDelta (individual positive, negative)
			if(thresholdDelta > 0){
				hitScore[i] = hitScore[i] + thresholdDelta * hitScoreIncFactor;
			} else {
				hitScore[i] = hitScore[i] + thresholdDelta * hitScoreDecFactor;
			}	
			hitScore[i] = ofClamp(hitScore[i], 0, maxHitScore);
			
			hit[i] = hitScore[i] > globalHitScoreThreshold;
				
			
			/*
			//float temp;
			if (doAveraging) {
				distanceValues[i] = distanceLUT[(int)averageValues[i].getAverage()];
			} else {
				distanceValues[i] = distanceLUT[rawValue];
			}
			*/
			
			//distanceValues[i] = temp - bgSubtract[i];
			//bgSubtract[i] = bgSubtract[i] * (1-bgAdaptFactor) + temp * bgAdaptFactor;
		} else {
			distanceValues[i] = 0; //TODO: exchange with variable
			hitScore[i] = 0;
			hit[i] = false;
		}
		
	}
}

void SensorCtrl::draw(){}

void SensorCtrl::draw3d(){
	
	/*
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
	*/
	
	for(int i=0; i<sensors.size(); i++)
		sensors[i]->draw();
}

void SensorCtrl::handleOscMessage(ofxOscMessage& message){
	if (message.getAddress() == SENSOR_OSC_TAG) {
		//TODO: check number of args
		
		
		int* arduinoId = Singleton<MotorsAndSensors>::instance()->arduinoId;
		int foundId = -1;
		int receivedId = message.getArgAsInt32(0);
		for(int i=0; i<4; i++)
		{
			if (arduinoId[i] == receivedId) {
				foundId = i;
				break;
			}
		}
		
		if(foundId != -1){
			int bank = foundId; //TODO: set LUT bank via GUI
		
			int offset = bank * RAYS_PER_BANK;
			int argId = 1;
		
			for(int i=0; i<RAYS_PER_BANK; i++){
				rawValues[offset+i] = message.getArgAsInt32(i+1);
			}
		}
		
		/*
		for (int led=0; led<RAYS_PER_SENSOR; led++) {
			for(int sensor=0; sensor<SENSORS_PER_BANK; sensor++)
			{
				int index = offset + sensor * RAYS_PER_SENSOR + led;
				rawValues[index] = message.getArgAsInt32(argId);
				argId++;
			}
		}
		*/
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

bool SensorCtrl::userInProximity(){
	return _userInProximity;
}