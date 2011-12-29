#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxMSAInteractiveObject.h"


/**
 Note: All ofxMSAInteractiveObject events are disabled by default.
 */
class ofxBaseObject : public ofxMSAInteractiveObject {
	
public:
	
	string name;
	
	////////////////////////////////////////////////////////////////////
	
	ofxBaseObject();
	
	/**
	 override this function and include all gui related code in here
	 */
	virtual void setupGUI(){}
	
	/**
	 function called after loading of gui xmls
	 */
	virtual void postGUI(){
	
	}
	
	/**
	 call this function to enable the object
	 */
	virtual void start(){
		enabled = true;
	}
	
	/**
	 call this function to disable the object
	 */
	virtual void stop(){
		enabled = false;
	}
	
	/**
	 use to add start() to an event
	 */
	void _start(ofEventArgs& e);
	
	/**
	 use to add stop() to an event
	 */
	void _stop(ofEventArgs& e);
	
	void addStartButton();	
	
	void addStopButton();
	
	////////////////////////////////////////////////////////////////////
	
	/**
	 outputs logText to console with name of object
	 */
	virtual void log(string logText);
	
protected:
	
	
};