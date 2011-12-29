#pragma once
/*
 *  ofxSimpleGuiEventTrigger.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 22/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "ofMain.h"

/**
 - container for standard listeners using ofEventArgs
 - used in conjunction with keyboard shortcuts
 */
class ofxSimpleGuiEventTrigger {

public:
	
	ofEvent<ofEventArgs> onTrigger;
	ofEventArgs e;
	
	template <class ListenerClass>
	void addListener(ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
		ofAddListener(onTrigger, listener, method);
	}	
	
	template <class ListenerClass>
	void removeListener(ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&)){
		ofRemoveListener(onTrigger, listener, method);
	}
	
	void trigger(){
		ofNotifyEvent(onTrigger, e, this);
	}
};
