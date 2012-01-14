/*
 *  OscMotorCtrl.cpp
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscMotorCtrl.h"

void OscMotorCtrl::setup(){
	oscSender.setup("127.0.0.1", 3333);
	motorCtrl = Singleton<MotorCtrl>::instance();
	sendFreqMs = 200;
	nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
}

void OscMotorCtrl::setupGUI(){
	gui.page("MotorCtrl").addPageShortcut(gui.addPage("OscMotorCtrl"));
	gui.addSlider("sendFreqMs",sendFreqMs,10,200);
}

void OscMotorCtrl::update(){
	
	if(ofGetElapsedTimeMillis() > nextSendTime){
		
		int numSendChannels = 18;
		
		ofxOscMessage m;
		m.setAddress("/servo");
		m.addIntArg(0);		// bank
		
		for (int i=0; i<numSendChannels; i++) {
			
			Motor& motor = *motorCtrl->motors[i];
			int servoVal = ofMap(motor.angleN, 0, 1, motorCtrl->servoMin, motorCtrl->servoMax, true);
			m.addIntArg(servoVal);
			
		}
		
		oscSender.sendMessage(m);
		
		nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
	}
}