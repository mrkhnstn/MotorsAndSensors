/*
 *  OscMotorCtrl.cpp
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscMotorCtrl.h"
#include "MotorsAndSensors.h"

void OscMotorCtrl::setup(){
	oscSender.setup("127.0.0.1", 3333);
	motorCtrl = Singleton<MotorCtrl>::instance();
	sendFreqMs = 200;
	nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
	arduinoId = Singleton<MotorsAndSensors>::instance()->arduinoId;
}

void OscMotorCtrl::setupGUI(){
	gui.setPage("Motors");
	gui.addTitle("OSC");
	gui.addSlider("sendFreqMs",sendFreqMs,10,200);
}

void OscMotorCtrl::update(){
	
	if(ofGetElapsedTimeMillis() > nextSendTime){
		
			for(int bank=0; bank<4; bank++){
			ofxOscMessage m;
			m.setAddress("/servo");
			m.addIntArg(Singleton<MotorsAndSensors>::instance()->arduinoId[bank]);		// bank
			for (int i=bank*18; i<(bank+1)*18; i++) {
				Motor& motor = *motorCtrl->motors[i];
				//int servoVal = ofMap(motor.angleN, 0, 1, Motor::globalPulseMin, Motor::globalPulseMax, true);
				m.addIntArg(motor.pulse);
			}
			
			oscSender.sendMessage(m);
		}
		
		nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
	}
}