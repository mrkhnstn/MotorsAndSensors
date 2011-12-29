#pragma once
/*
 *  ofxValueObserverT.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 22/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxValueObserver.h"

template <class T> class ofxValueObserverT : public ofxValueObserver {

public:
	
	T* val;
	T oldVal;
	
	ofxValueObserverT(){
		val = NULL;
	}
	
	ofxValueObserverT(T& val){
		setup(val);
	}
	
	void setup(T& val){
		this->val = &val;
		oldVal = val;
	}
	
	template<class ListenerClass> void setup(T& val, ListenerClass  * listener, void (ListenerClass::*method)(ofEventArgs&), bool autoUpdate = true){
		setup(val);
		ofAddListener(onChanged, listener, method);
		if(autoUpdate)
			enableUpdate();
	}
	
	virtual void update(){
		if(oldVal != *val){
			oldVal = *val;
			changed();
		}
	}
};