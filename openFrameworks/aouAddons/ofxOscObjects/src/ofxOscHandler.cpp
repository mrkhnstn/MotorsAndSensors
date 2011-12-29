/*
 *  OscHandler.cpp
 *  remoteactors
 *
 *  Created by Robin Beitra (robin@beitra.net) on 31/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOscHandler.h"

void ofxOscHandler::handleOscMessage(ofxOscMessage& message){
}

float ofxOscHandler::castOscNumberToFloat(ofxOscMessage& message, int index){
	float result = 0;
	ofxOscArgType t = message.getArgType(index);
	switch(t){
		case OFXOSC_TYPE_INT32:
		{
			int i = message.getArgAsInt32(index);
			result = i;
			break;
		}
		case OFXOSC_TYPE_FLOAT:
		{
			result = message.getArgAsFloat(index);
			break;
		}
	}
	return result;
}

int ofxOscHandler::castOscNumberToInt(ofxOscMessage& message, int index){
	int result = 0;
	ofxOscArgType t = message.getArgType(index);
	switch(t){
		case OFXOSC_TYPE_INT32:
		{
			result = message.getArgAsInt32(index);
			break;
		}
		case OFXOSC_TYPE_FLOAT:
		{
			float f = message.getArgAsFloat(index);
			result = f;
			break;
		}
		case OFXOSC_TYPE_STRING:
		{
			result = ofToInt(message.getArgAsString(index));
			break;
		}
	}
	return result;
}

bool ofxOscHandler::isOscArgNumeric(ofxOscMessage& message, int index){
	ofxOscArgType t = message.getArgType(index);
	return t == OFXOSC_TYPE_INT32 || t == OFXOSC_TYPE_FLOAT;
}

bool ofxOscHandler::isOscArgString(ofxOscMessage& message, int index){
	ofxOscArgType t = message.getArgType(index);
	return t == OFXOSC_TYPE_STRING;
}

void ofxOscHandler::splitOscAddress(ofxOscMessage& message, vector<string>& container){
	string delimiter = "/";
	string address = message.getAddress();
	tokenize(address, container, delimiter, false);
}
