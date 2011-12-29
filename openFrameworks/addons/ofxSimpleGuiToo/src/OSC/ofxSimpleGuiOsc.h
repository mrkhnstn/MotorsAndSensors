#pragma once
/*
 *  ofxSimpleGuiOsc.h
 *  atmosphere
 *
 *  Created by Developer on 19/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOsc.h"
#include "ofxSimpleGuiControl.h"
#include <map>

class ofxSimpleGuiOsc {

	struct SendAddress {
		string ip;
		int port;
		
		SendAddress(string ip_, int port_){
			ip = ip_;
			port = port_;
		}
	};
	
	struct ReceiveControl {
		string oscMessageName;
		ofxSimpleGuiControl* control;
		
		ReceiveControl(ofxSimpleGuiControl& control_, string oscMessageName_ = ""){
			control = &control_;
			if (oscMessageName_ != "") {
				oscMessageName = oscMessageName_;
			} else {
				oscMessageName = control_.key;
			}
		}
	};
	
public:
	
	ofxSimpleGuiOsc(){
		_setup = false;
	}
	
	~ofxSimpleGuiOsc(){
		if (_setup) {
			//removeListeners();
		}
	}
	
	void setup(int receivePort, string sendIp = "127.0.0.1", int sendPort = 8000){
		if (!_setup) { // make sure we only setup once
			_setup = true;
			receiver.setup(receivePort);
			sender.setup(sendIp,sendPort);
			addListeners();
		}
	}
	
	void addListeners(){
		ofAddListener(ofEvents.update, this, &ofxSimpleGuiOsc::update);
	}
	
	void removeListeners(){
		ofRemoveListener(ofEvents.update, this, &ofxSimpleGuiOsc::update);
	}
	
	void update(ofEventArgs &e){
		
		while (receiver.hasWaitingMessages()) {
			// get the next message
			ofxOscMessage m;
			receiver.getNextMessage( &m );
			
			map<string,ofxSimpleGuiControl*>::iterator it = receiveControls.find(m.getAddress());
			if (it != receiveControls.end()) {
				ofxSimpleGuiControl* control = it->second;
				control->receiveOscMessage(m);
			}
			else 
			{
				string msg_string;
				msg_string = m.getAddress();
				msg_string += ": ";
				for ( int i=0; i<m.getNumArgs(); i++ )
				{
					// get the argument type
					msg_string += m.getArgTypeName( i );
					msg_string += ":";
					// display the argument - make sure we get the right type
					if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
						msg_string += ofToString( m.getArgAsInt32( i ) );
					else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
						msg_string += ofToString( m.getArgAsFloat( i ) );
					else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
						msg_string += m.getArgAsString( i );
					else
						msg_string += "unknown";
				}
				ofLog(OF_LOG_VERBOSE, msg_string);
			}

		}
	}
	
	/**
	*/
	void addControl(ofxSimpleGuiControl& guiControl, string oscMessageName=""){
		string tempName = "/"+guiControl.name;
		if (oscMessageName.compare("") != 0) {
			tempName = oscMessageName;
		}
		receiveControls[tempName] = &guiControl;
	}

	void send(ofxOscMessage& message){
		for (vector<SendAddress*>::iterator it = sendAddress.begin(); it != sendAddress.end(); ++it) {
			SendAddress* sa = *it;
			message.setRemoteEndpoint(sa->ip, sa->port);
		}
		sender.sendMessage(message);
	}
	
	void addSendAddress(string sendIp, int sendPort){
		//TODO: implement
		sendAddress.push_back(new SendAddress(sendIp,sendPort));
	}
	
protected:
	
	bool _setup;
	ofxOscReceiver receiver;
	ofxOscSender sender;
	map<string,ofxSimpleGuiControl*> receiveControls;
	vector<SendAddress*> sendAddress;
};