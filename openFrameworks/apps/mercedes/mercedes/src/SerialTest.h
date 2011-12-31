#pragma once
/*
 *  SerialTest.h
 *  mercedes
 *
 *  Created by Mark Hauenstein on 24/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"
#include "FinCtrl.h"
#include "ofxOsc.h"

class SerialTest : public ofxBaseObject {
	
public: 
	
	ofxOscSender oscSender;
	FinCtrl* finCtrl;
	
	ofSerial serial;
	
	int sendFreqMs;
	int nextSendTime; // if ofGetElapsedTimeMillis() > nextSendTime then send fin values to SD84
	
	void setup(){
		//oscSender.setup("127.0.0.1", 3333);
		
		serial.enumerateDevices();
		serial.setup(135,115200);
		
		finCtrl = Singleton<FinCtrl>::instance();
		
		sendFreqMs = 50;
		nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
		
		//setupSD84Modes();
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("OscFinCtrl"));
		gui.addSlider("sendFreqMs",sendFreqMs,10,200);
	}
	
	void update(){
		
		if(ofGetElapsedTimeMillis() > nextSendTime){
			
			Fin& fin = *finCtrl->fins[0];
			int servoVal = ofMap(fin.angleN, 0, 1, finCtrl->servoMin, finCtrl->servoMax, true);
			
			unsigned char lowByte = servoVal & 255;
			unsigned char highByte = (servoVal >> 8) & 255;
			//ofLog(OF_LOG_VERBOSE, ofToString(lowByte) + "," + ofToString(highByte));
			unsigned char msg[] = {170,160,85,1,1,2,lowByte,highByte};
			serial.writeBytes(msg, 8);
			
			/*
			serial.writeByte(170);
			serial.writeByte(160);
			serial.writeByte(85);
			serial.writeByte(1);
			serial.writeByte(1);
			serial.writeByte(2);
			serial.writeByte(lowByte);
			serial.writeByte(highByte);
			*/
			
			//serial.flush(true, true);
			
			/*
			while(serial.available() > 0){
				ofLog(OF_LOG_VERBOSE,ofToString(serial.readByte()));
			}
			*/
			
			/*
			int numSendChannels = 18;
			
			ofxOscMessage m;
			m.setAddress("/sendRaw");
			m.addIntArg(0);		// bank
			
			m.addIntArg(170);	// sync 1
			m.addIntArg(160);	// sync 2
			m.addIntArg(85);	// sync 3
			
			m.addIntArg(1);		// 0x01 SET_SERVO
			m.addIntArg(1);		// from channel
			m.addIntArg(numSendChannels*2); //num of bytes (2 bytes per channel)
			
			for (int i=0; i<numSendChannels; i++) {
				
				Fin& fin = *finCtrl->fins[i];
				int servoVal = ofMap(fin.angleN, 0, 1, finCtrl->servoMin, finCtrl->servoMax, true);
				
				int lowByte = servoVal & 255;
				int highByte = (servoVal >> 8) & 255;
				
				m.addIntArg(lowByte);
				m.addIntArg(highByte);
			}
			
			oscSender.sendMessage(m);
			*/
			
			nextSendTime = ofGetElapsedTimeMillis() + sendFreqMs;
		}
	}
	
	// set all channels on all servo related SD84s to SERVO_MODE
	void setupSD84Modes(){
		
		for (int i=0; i<4; i++) {
			
			ofxOscMessage m;
			m.setAddress("/sendRaw");
			m.addIntArg(i); // bank
			
			m.addIntArg(170); // sync 1
			m.addIntArg(160);
			m.addIntArg(85);
			
			m.addIntArg(4); // 0x04 SET_MODE
			m.addIntArg(1); // from channel 1
			m.addIntArg(84); // set all to servo
			
			for(int i=0; i<84; i++)
			{
				m.addIntArg(25); // SERVO_MODE (25)
			}
			
			oscSender.sendMessage(m);
			
		}
	}
	
	
	
};
