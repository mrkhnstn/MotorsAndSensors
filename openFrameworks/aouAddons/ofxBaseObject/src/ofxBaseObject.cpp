
#include "ofxBaseObject.h"

////////////////////////////////////////////////////////////////////

ofxBaseObject::ofxBaseObject(){
	name = "ofxBaseObject";
	disableAllEvents();
}

////////////////////////////////////////////////////////////////////

void ofxBaseObject::log(string logText){
	ofLog(OF_LOG_VERBOSE, name + ": " + logText);
}

////////////////////////////////////////////////////////////////////

void ofxBaseObject::_start(ofEventArgs& e){
	start();
}

////////////////////////////////////////////////////////////////////

void ofxBaseObject::_stop(ofEventArgs& e){
	stop();
}

void ofxBaseObject::addStartButton(){
	gui.addButton("start",this,&ofxBaseObject::_start);
}

void ofxBaseObject::addStopButton(){
	gui.addButton("stop",this,&ofxBaseObject::_stop);
}
