#pragma once
/*
 *  Lerp.h
 *  TestBed
 *
 *  Created by Mark Hauenstein on 13/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

template<class T, class R = T>
class Lerp{
public:
	T value;
	T target;
	R rate;
	
	inline void lerp(){
		value += (target - value)*rate;
	}
	
	inline void linear(){
		T delta = value - target;
		if(delta < rate || delta > -rate) value = target;
		else value += rate;
	}
	
	void setup(T value, T target){
		this->value = value;
		this->target = target;
	}
	void setup(T value, T target, R rate){
		this->value = value;
		this->target = target;
		this->rate = rate;
	}
};