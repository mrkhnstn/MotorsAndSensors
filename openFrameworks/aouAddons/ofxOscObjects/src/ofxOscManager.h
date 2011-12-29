#pragma once
/*
 *  ofxOscManager.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 18/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBaseObject.h"
#include "ofxOscHandler.h"
//#include "ofxOscSendValueBase.h"
//#include "ofxOscReceiveValueBase.h"
//class ofxOscReceiveValueBase;
//class ofxOscSendValueBase;


class ofxOscManager : public ofxBaseObject {

public:
	
	VectorMap<string, vector<ofxOscHandler* > > oscHandlers;
	//vector<ofxOscSendValueBase*> sendValues;
	//vector<ofxOscReceiveValueBase*> receiveValues;
	ofxOscReceiver oscReceiver;
	ofxOscMessage oscMessage;
	int port;
	bool verbose;
	
	ofxOscManager();
	
	void setupGUI();
	
	void setup(); //< same as setup(3333,true);	
	
	void setup(int port, bool autoStart = true);
	
	virtual void update();
	
	void registerInterest(ofxOscHandler& handler, string interest);
	
	void addFloatToSend(float &floatValue, string oscAddress, ofxOscSender& oscSender){
		//sendValues.push_back(new ofxOscSendValueFloat(floatValue, oscAddress, oscSender));
	}
	
	void addFloatToReceive(float &floatValue, string oscAddress){} //TODO: implement
	
	void addIntToSend(int &intValue, string oscAddress, ofxOscSender& oscSender){} //TODO: implement
	
	void addIntToReceive(int &intValue, string oscAddress){} //TODO: implement
	
	void addBoolToSend(bool &boolValue, string oscAddress, ofxOscSender& oscSender){} //TODO: implement
	
	void addBoolToReceive(bool &boolValue, string oscAddress){} //TODO: implement
	
protected:
	
	void handleOscMessage(ofxOscMessage& message);
	
};