#pragma once
/*
 *  ofxValueT.h
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 28/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxValue.h"

template <class T> class ofxValueT : public ofxValue {

public:
	
	T val;
	
	void set(T& val){
		if (this->val != val) {
			this->val = val;
			changed();
		}
	}
	
	T& get(){
		return val;
	}
	
};