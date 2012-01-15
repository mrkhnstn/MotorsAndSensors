/*
 *  ofxOscManager.cpp
 *  TestBed
 *
 *  Created by Mark Hauenstein on 18/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOscManager.h"

ofxOscManager::ofxOscManager(){
	name = "OSC";
	enabled = false;
	port = 3333;
	verbose = false;
}

void ofxOscManager::setupGUI(){
	gui.page(1).addPageShortcut(gui.addPage(name));
	gui.addDebug("port",port);
	gui.addToggle("verbose", verbose);
}

void ofxOscManager::setup(){
	setup(this->port);
}

void ofxOscManager::setup(int port, bool autoStart){
	oscReceiver.setup(port);
	this->port = port;
	ofAddListener(ofEvents.update, (ofxMSAInteractiveObject*)this, &ofxMSAInteractiveObject::_update);
	if(autoStart) start();
}

void ofxOscManager::update(){
	
	if(!enabled) return;
	
	if(oscReceiver.hasWaitingMessages()){
		while(oscReceiver.getNextMessage(&oscMessage)){
			handleOscMessage(oscMessage);
			oscMessage.clear();
		}
	}
	
	/*
	for (vector<ofxOscSendValueBase*>::iterator it = sendValues.begin(); it != sendValues.end(); ++it) {
		(*it)->update();
	}
	*/
}

void ofxOscManager::registerInterest(ofxOscHandler& handler, string interest){
	
	if(!oscHandlers.hasKey(interest)){
		vector<ofxOscHandler* > ihs;
		oscHandlers.add(interest, ihs);
	}
	vector<ofxOscHandler* > & interestHandlers = oscHandlers.at(interest);
	interestHandlers.push_back(&handler);
	
}

void ofxOscManager::handleOscMessage(ofxOscMessage& message){
	string address = message.getAddress();
	
	if (verbose) {
		ofLog(OF_LOG_VERBOSE, "OSC " + ofToString(port) + " : " + address);
	}
	
	int index;
	bool foundInterest = false;
	int max = oscHandlers.size();
	for(index = 0; index < max; ++index){
		string interest = oscHandlers.key(index);
		if(address.find(interest) == 0){
			//ie only if the first part of the address matches
			//e.g. interest:/a/b will match /a/b and /a/b/c but not /a or /c/a/b
			foundInterest = true;
			break;
		}
	}
	
	if(foundInterest){
		vector<ofxOscHandler* > interestHandlers = oscHandlers[index];
		for(vector<ofxOscHandler*>::iterator it = interestHandlers.begin(); it != interestHandlers.end(); ++it){
			ofxOscHandler& handler = **it;
			handler.handleOscMessage(message);
		}
	}
}