#pragma once
/*
 *  ofxValueObserver.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 22/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

class ofxValueObserver{

public:
	
	ofEvent<ofEventArgs> onChanged;
	ofEventArgs e;
	
	void update(ofEventArgs& e){
		update();
	}
	
	void enableUpdate(){
		ofAddListener(ofEvents.update, this, &ofxValueObserver::update);
	}
	
	void disableUpdate(){
		ofRemoveListener(ofEvents.update,this, &ofxValueObserver::update);
	}
	
	virtual void update() = 0;
	
	void changed(){
		ofNotifyEvent(onChanged, e, this);
	}
	
};
