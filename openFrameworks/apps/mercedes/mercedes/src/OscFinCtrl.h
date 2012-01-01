#pragma once
/*
 *  OscFinCtrl.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "FinCtrl.h"
#include "ofxOsc.h"

class OscFinCtrl : public ofxBaseObject {
	
public: 
	
	ofxOscSender oscSender;
	FinCtrl* finCtrl;
	
	int sendFreqMs;
	int nextSendTime; // if ofGetElapsedTimeMillis() > nextSendTime then send fin values to SD84
	
	void setup(){
		oscSender.setup("127.0.0.1", 3333);
		finCtrl = Singleton<FinCtrl>::instance();
		sendFreqMs = 200;
		nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
	}
	
	void setupGUI(){
		gui.page("FinCtrl").addPageShortcut(gui.addPage("OscFinCtrl"));
		gui.addSlider("sendFreqMs",sendFreqMs,10,200);
	}
	
	void update(){
		
		if(ofGetElapsedTimeMillis() > nextSendTime){
			
			int numSendChannels = 18;
			
			ofxOscMessage m;
			m.setAddress("/servo");
			m.addIntArg(0);		// bank
			
			for (int i=0; i<numSendChannels; i++) {
				
				Fin& fin = *finCtrl->fins[i];
				int servoVal = ofMap(fin.angleN, 0, 1, finCtrl->servoMin, finCtrl->servoMax, true);
				m.addIntArg(servoVal);
				
			}
			
			oscSender.sendMessage(m);
			
			nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
		}
	}
	
};
