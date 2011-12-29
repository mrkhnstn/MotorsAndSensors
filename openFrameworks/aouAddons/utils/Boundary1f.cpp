#include "Boundary1f.h"

/*
 *  Boundary1f.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 25/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

Boundary1f::Boundary1f(float _min, float _max){
	min = _min;
	max = _max;
}

Boundary1f::Boundary1f(const Boundary1f& mom){
	min = mom.min;
	max = mom.max;
}

float Boundary1f::set(const Boundary1f& mom){
	set(mom.min,mom.max);
}

float Boundary1f::set(float _min, float _max){
	min = _min;
	max = _max;
}

float Boundary1f::getRandomValue(){
	return ofRandom(min, max);
}

float Boundary1f::getNormalized(float f){
	return ofNormalize(f, min, max);
}

float Boundary1f::getMapped(float f, float min_, float max_){
	return ofMap(f,min_,max_,min,max);
}

bool Boundary1f::contains(float f){
	return ofInRange(f, min, max);
}

// for convenience reasons, shouldn't really be here
void Boundary1f::addToGUI(string _name, float _min, float _max){
	gui.addSlider(_name+"_min", min, _min, _max);
	gui.addSlider(_name+"_max", max, _min, _max);
}