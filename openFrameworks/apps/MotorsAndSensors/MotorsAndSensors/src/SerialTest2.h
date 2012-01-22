#pragma once
/*
 *  SerialTest2.h
 *  MotorsAndSensors
 *
 *  Created by Mark Hauenstein on 18/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Includes.h"

class SerialTest2 : public ofxBaseObject {
	
public:
	
	int sendValue;
	int _sendValue;
	int receiveValue;
	bool doRead;
	bool doSend;
	
	ofSerial serial;
	
	char		bytesRead[3];				// data from serial, we will be trying to read 3
	char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
	int			nBytesRead;					// how much did we read?
	int			nTimesRead;					// how many times did we read?
	float		readTime;					// when did we last read?	
	
	char readBuffer[2];
	int readIndex;
	
	void setup(){
		sendValue = 0;
		_sendValue = sendValue;
		receiveValue = 0;
		readIndex = 0;
		
		doRead = false;
		doSend = true;
		nTimesRead = 0;
		nBytesRead = 0;
		readTime = 0;
		memset(bytesReadString, 0, 4);
		
		serial.setup("/dev/tty.usbmodem411",115200);
	}
	
	void setupGUI(){
		gui.page(1).addPageShortcut(gui.addPage("SerialTest2"));
		gui.addSlider("sendValue",sendValue,0,1024);
		gui.addDebug("receiveValue", receiveValue);
		gui.addToggle("doSend", doSend);
		//gui.addToggle("doRead", doRead);
	}
	
	void update(){
		
		if(sendValue != _sendValue){
			unsigned char lowByte = sendValue & 255;
			unsigned char highByte = (sendValue >> 8) & 255;
			ofLog(OF_LOG_VERBOSE, "low byte : " + ofToString((int)lowByte));
			ofLog(OF_LOG_VERBOSE, "high byte : " + ofToString((int)highByte));
			serial.writeByte(lowByte);
			serial.writeByte(highByte);			
			
			_sendValue = sendValue;
		}
		
		int available = serial.available();
		int i=0;
		while (serial.available() > 0) {
			int val = serial.readByte();
			readBuffer[readIndex] = (char)val;
			if(readIndex == 1){
				receiveValue = (unsigned char)readBuffer[1] << 8 | (unsigned char)readBuffer[0];
			}
			readIndex++;
			if(readIndex == 2)
				readIndex = 0;
			ofLog(OF_LOG_VERBOSE, ofToString(i) + " : " + ofToString(val));
			i++;
		}
		
		/*
		if (doRead){

			
						
			nTimesRead = 0;
			nBytesRead = 0;
			int nRead  = 0;  // a temp variable to keep count per read
			
			unsigned char bytesReturned[2];
			
			memset(bytesReturned, 0, 2);
			
			while( (nRead = serial.readBytes( bytesReturned, 2)) > 0){
				nTimesRead++;	
				nBytesRead = nRead;
			};
			
			receiveValue = bytesReturned[1] * 255 + bytesReturned[0];
			ofLog(OF_LOG_VERBOSE, "received:"+ofToString(receiveValue));
			
			doRead = false;
			readTime = ofGetElapsedTimef();
			
		}
		*/
		
	}
	
	void doDefaultExample(){
		if (doRead){
			
			// (1) write the letter "a" to serial:
			serial.writeByte('a');
			
			// (2) read
			// now we try to read 3 bytes
			// since we might not get them all the time 3 - but sometimes 0, 6, or something else,
			// we will try to read three bytes, as much as we can
			// otherwise, we may have a "lag" if we don't read fast enough
			// or just read three every time. now, we will be sure to 
			// read as much as we can in groups of three...
			
			nTimesRead = 0;
			nBytesRead = 0;
			int nRead  = 0;  // a temp variable to keep count per read
			
			unsigned char bytesReturned[3];
			
			memset(bytesReadString, 0, 4);
			memset(bytesReturned, 0, 3);
			
			while( (nRead = serial.readBytes( bytesReturned, 3)) > 0){
				nTimesRead++;	
				nBytesRead = nRead;
			};
			
			memcpy(bytesReadString, bytesReturned, 3);
			
			ofLog(OF_LOG_VERBOSE, bytesReadString);
			
			doRead = false;
			readTime = ofGetElapsedTimef();
			
		}
		
	}
	
	
};
