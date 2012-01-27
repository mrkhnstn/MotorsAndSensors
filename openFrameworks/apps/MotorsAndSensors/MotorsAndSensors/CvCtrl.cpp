/*
 *  CvCtrl.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 17/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "CvCtrl.h"
#include "SensorCtrl.h"
#include "MotorCtrl.h"
#include "Coordinates.h"

void CvCtrl::setup(){
	font.loadFont("fonts/Hlcb____.ttf",4,true,true,true);
	
	imgW = 512;
	imgH = 256;
	
	contourMinArea = 1;
	contourMaxArea = 100;
	maxContours = 10;
	blobsCount = 0;
	
	cvImgFront.allocate(imgW, imgH);
	cvImgFront.set(0);
	
	testX = 256;
	testY = 128;
	testRadius = 5;
	
	rayGap = RAY_GAP;
	motorGap = MOTOR_GAP;
	sensorOffset = SENSOR_OFFSET;
	sensorGap = SENSOR_GAP;
	rayOffset = RAY_OFFSET;
	
	doDraw = true;
	doDrawTest = false;
	drawScale = 2;
	drawX = 0;
	drawY = 0;
	
	doDrawRaw = true;
	doDrawBg = true;
	doDrawAdapted = true;
	rayDrawScale = 0.3;
}

void CvCtrl::setupGUI(){
	gui.page(1).addPageShortcut(gui.addPage("ComputerVision"));
	
	gui.addToggle("doDraw", doDraw);
	gui.addSlider("drawScale", drawScale, 0.5, 10);
	gui.addSlider("drawX",drawX,-512,1024);
	gui.addSlider("drawY",drawY,-512, 1024);
	
	//gui.addSlider("rayGap", rayGap, 1, 100);
	gui.addSlider("rayDrawScale", rayDrawScale, 0.1, 1);
	
	
	gui.addToggle("doDrawRaw", doDrawRaw);
	gui.addToggle("doDrawBg", doDrawBg);
	gui.addToggle("doDrawAdapter", doDrawAdapted);
	gui.addToggle("doDrawHitScore", doDrawHitScore);
	
	//gui.addContent("cvImgFront", cvImgFront);
	
	gui.addTitle("test");
	gui.addSlider("testX", testX, 0, imgW-1);
	gui.addSlider("testY", testY, 0, imgH-1);
	gui.addSlider("testRadius", testRadius, 1, 30);
	gui.addToggle("doDrawTest", doDrawTest);
	
	gui.addTitle("img_processing");
	gui.addSlider("smooth", smooth, 0, 100);
	gui.addSlider("threshold",threshold,0,255);
	
	gui.addTitle("contours");
	gui.addSlider("minArea",contourMinArea,0,10000);
	gui.addSlider("maxArea",contourMaxArea,0,10000);
	gui.addSlider("maxContours",maxContours,0,255);
	gui.addDebug("blobsCount",blobsCount);
	//gui.addContent("contourFinder",contourFinder);
	
	gui.addTitle("cv zone");
	gui.addSlider("polarMinRadius", Coordinates::polarMinRadius, 0, 600);
	gui.addSlider("polarMaxRadius", Coordinates::polarMaxRadius, 0, 600);
	
	gui.addSlider("cvMinX", Coordinates::cartMinX, 0, 512);
	gui.addSlider("cvMaxX", Coordinates::cartMaxX, 0, 512);
	gui.addSlider("cvMinY", Coordinates::cartMinY, 0, 256);
	gui.addSlider("cvMaxY", Coordinates::cartMaxY, 0, 256);
	
	gui.addTitle("sensing zone");
	gui.addSlider("minSensingZone", Coordinates::minSensingZone, 0, 1024);
	gui.addSlider("maxSensingZone", Coordinates::maxSensingZone, 0, 1024);
	
	gui.addTitle("motor proximity value");
	gui.addSlider("proximityMax", Motor::maxProximityValue, 10, 100);
	gui.addSlider("proximityThreshold", Motor::proximityOnThreshold, 0, 100);
	
	
}

void CvCtrl::postGUI(){}

void CvCtrl::update(){
	
	/*
	localMinSensingZone = ofMap(Coordinates::minSensingZone, Coordinates::polarMinRadius, Coordinates::polarMaxRadius, Coordinates::cartMinY, Coordinates::cartMaxY, false);
	localMaxSensingZone = ofMap(Coordinates::maxSensingZone, Coordinates::polarMinRadius, Coordinates::polarMaxRadius, Coordinates::cartMinY, Coordinates::cartMaxY, false);
	
	// clear img
	cvImgFront.set(0);
	
	// draw test circle
	if(doDrawTest)
		cvCircle(cvImgFront.getCvImage(),cvPoint(testX,testY),testRadius,cvScalar(255),-1,4);
	
	// draw sensor circles
	vector<Sensor*>& sensors = Singleton<SensorCtrl>::instance()->sensors;
	for(int i=0; i<sensors.size(); i++)
	{
		Sensor& sensor = *sensors[i];
		for (int j=0; j<5; j++) {
			ofxVec2f vec = sensor.cartRayPos[j];
			if(vec.x >= 0 && vec.x < imgW && vec.y >= 0 && vec.y < imgH) // if within img
			{
				cvCircle(cvImgFront.getCvImage(),cvPoint(vec.x,vec.y),testRadius,cvScalar(255),-1,4);
			}
		}
	}
	
	// img processing
	cvSmooth(cvImgFront.getCvImage(), cvImgFront.getCvImage(), CV_BLUR , int(ofMap(smooth, 0, 100, 0, 20))*2 + 1);
	cvThreshold(cvImgFront.getCvImage(), cvImgFront.getCvImage(), threshold, 255, CV_THRESH_BINARY);
	cvImgFront.flagImageChanged();
	
	vector<Motor*>& motors = Singleton<MotorCtrl>::instance()->motors;
	   motorGap = motors[2]->cartPos.x - motors[1]->cartPos.x;
	
	// reset all motor proximity udpated flag
	for(int i=0; i<motors.size(); i++){
		motors[i]->proximityUpdated = false;
	}
	
	// find contours
	contourFinder.findContours(cvImgFront, contourMinArea, contourMaxArea, maxContours, false);
	
	
	// iterate through blobs and register proximity with motors
	blobsCount = (int)contourFinder.blobs.size();
	for(int i=0; i<blobsCount; i++)
	{
		ofxCvBlob& blob = contourFinder.blobs[i];

		float bTop = blob.boundingRect.y;
		float bBottom = blob.boundingRect.y + blob.boundingRect.height;
		
		if(bTop > localMinSensingZone && bTop < localMaxSensingZone || bBottom > localMinSensingZone && bBottom < localMaxSensingZone){
			// blob within sensing zone
			int minMotor = (int)roundf(blob.boundingRect.x / motorGap);
			int maxMotor = (int)roundf((blob.boundingRect.x+blob.boundingRect.width) / motorGap);
			for(int i=minMotor; i<= maxMotor; i++){
				if(i >= 0 && i <motors.size()){
					motors[i]->incrementProximityValue();
					motors[i]->proximityUpdated = true;
				}
			}
		}
	}
	
	//
	for(int i=0; i<motors.size(); i++){
		if(!motors[i]->proximityUpdated)
			motors[i]->decrementProximityValue();
	}
	 
	*/
	
}

void CvCtrl::draw(){
	if(!doDraw) return;
	
	ofPushMatrix();
	ofTranslate(drawX, drawY, 0);
	ofScale(drawScale, 1, 1);
	ofSetColor(255, 255, 0);
	ofNoFill();
	//ofRect(0,0,imgW,imgH);
	
	//contourFinder.draw();
	//int testMotorId = (int)roundf(testX / motorGap);
	
	// draw motors
	ofPushStyle();	
	
	float proximityScale = 3;
	
	float crossSize = 1;
	
	vector<Motor*>& motors = Singleton<MotorCtrl>::instance()->motors;
	for(int i=0; i<motors.size(); i++)
	{
		Motor & motor = *motors[i];
		if(motor.userInProximity()){
			ofSetColor(255, 255, 0);
		} else {
			ofSetColor(255, 0, 0);
		}
		
		ofPushMatrix();
		
		ofTranslate(i*motorGap, 0, 0);
		ofLine(-crossSize, 0, crossSize, 0);
		ofLine(0,-crossSize, 0, crossSize);
		ofDrawBitmapString(ofToString(motor.index), 0, 0);	
				
		//ofLine(0, 0, 0, motor.proximityValue * proximityScale);
		ofPopMatrix();
	}
	ofPopStyle();
	
	/*
	// draw proximity thresholds
	ofPushStyle();
	
	ofSetColor(255, 255, 0);
	
	ofPushMatrix();
	ofTranslate(0, motors[0]->cartPos.y + Motor::proximityOnThreshold * proximityScale, 0);
	ofLine(0, 0, imgW, 0);
	ofDrawBitmapString("proximityOn", 0, 0);
	ofPopMatrix();
	
	ofPopStyle();
	*/
	
	
	// draw sensors
	ofPushStyle();	
	ofSetColor(0, 255, 0);

	ofPushMatrix();
	ofTranslate(0, 20, 0);
	vector<Sensor*>& sensors = Singleton<SensorCtrl>::instance()->sensors;
	SensorCtrl& sensorCtrl = *Singleton<SensorCtrl>::instance();
	for(int i=0; i<sensors.size(); i++)
	{
		Sensor& sensor = *sensors[i];
		//ofxVec2f cartPos = Coordinates::fromPolar(sensor.pos);
		ofPushMatrix();
		ofTranslate(sensorOffset + i * sensorGap, 0, 0);
		ofLine(-crossSize, 0, crossSize, 0);
		ofLine(0,-crossSize, 0, crossSize);
		//font.drawString(ofToString(sensor.index), -4, -4);
		ofDrawBitmapString(ofToString(sensor.index), -1, -2);
		
		//int rayOffset = i * 5;

		ofPopMatrix();
	}
	
	ofPopStyle();
	
	// draw rays
	
	
	ofScale(1, rayDrawScale, 1);
	
	if(doDrawRaw){
		ofSetColor(0, 255, 0);
		for(int j=0; j<TOTAL_RAYS; j++){
			float x = rayOffset + j * rayGap - 1;
			ofLine(x, 0, x, sensorCtrl.rawValues[j]);
		}
	}
	
	if(doDrawBg){
		ofSetColor(0, 255, 255);
		for(int j=0; j<TOTAL_RAYS; j++){
			float x = rayOffset + j * rayGap - 2;
			ofLine(x, 0, x, sensorCtrl.bgSubtract[j]);
		}
	}
	
	if(doDrawAdapted){
		
		for(int j=0; j<TOTAL_RAYS; j++){
			if(sensorCtrl.hit[j]){
				ofSetColor(255, 255, 0);
			} else {
				ofSetColor(255, 0, 255);
			}
			float x = rayOffset + j * rayGap + 0;
			ofLine(x, 0, x, sensorCtrl.adaptedValues[j]);
		}
		
		ofSetColor(255, 255, 0);
		ofPushMatrix();
		ofTranslate(0, sensorCtrl.globalHitThreshold, 0);
		ofLine(0, 0, imgW, 0);
		ofDrawBitmapString("hit threshold", 0, 0);
		ofPopMatrix();
	}
	
	if(doDrawHitScore){
		
		for(int j=0; j<TOTAL_RAYS; j++){
			ofSetColor(128, 128, 128);
			float x = rayOffset + j * rayGap + 1;
			ofLine(x, 0, x, sensorCtrl.hitScore[j]);
		}
		
		ofSetColor(128, 128, 128);
		ofPushMatrix();
		ofTranslate(0, sensorCtrl.globalHitScoreThreshold, 0);
		ofLine(0, 0, imgW, 0);
		ofDrawBitmapString("hit score threshold", 0, 0);
		ofPopMatrix();
		
		ofSetColor(128, 128, 128);
		ofPushMatrix();
		ofTranslate(0, sensorCtrl.maxHitScore, 0);
		ofLine(0, 0, imgW, 0);
		ofDrawBitmapString("max hit score", 0, 0);
		ofPopMatrix();
	}
	
	ofPopMatrix();
	/*
	// draw cv zone
	ofPushStyle();
	
	ofSetColor(128, 128, 128);
	
	ofPushMatrix();
	ofTranslate(0, 0, 0);
	ofLine(0, 0, imgW, 0);
	ofDrawBitmapString("cv min", 0, 0);
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(0, imgH, 0);
	ofLine(0, 0, imgW, 0);
	ofDrawBitmapString("cv max", 0, 0);
	ofPopMatrix();
	
	ofPopStyle();
	
	// draw sensing zone
	ofPushStyle();
	
	ofSetColor(255, 0, 255);
	
	ofPushMatrix();
	ofTranslate(0, localMinSensingZone, 0);
	ofLine(0, 0, imgW, 0);
	ofDrawBitmapString("sensing min", 0, 0);
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(0, localMaxSensingZone, 0);
	ofLine(0, 0, imgW, 0);
	ofDrawBitmapString("sensing max", 0, 0);
	ofPopMatrix();
	
	ofPopStyle();
	
	if(doDrawTest){
		// draw test point
		ofPushStyle();
		ofSetColor(255, 255, 0);
		ofCircle(testX, testY, 5);
		ofPopStyle();
	}
	 
	*/
	
	ofPopMatrix();
}

void CvCtrl::draw3d(){
	/*
	if(doDraw){
		ofPushStyle();
		ofPushMatrix();
		ofSetColor(128,128,128);
		ofSetLineWidth(1);
		ofNoFill();
		ofRotate(90, 1, 0, 0);
		
		// draw polar zone
		ofSetCircleResolution(72);
		ofCircle(0, 0, Coordinates::polarMinRadius);
		ofCircle(0, 0, Coordinates::polarMaxRadius);
		
		// draw sensing zone
		ofSetColor(255, 0, 255);
		ofCircle(0, 0, Coordinates::minSensingZone);
		ofCircle(0, 0, Coordinates::maxSensingZone);
		
		ofPopMatrix();
		ofPopStyle();
	}
	*/
}