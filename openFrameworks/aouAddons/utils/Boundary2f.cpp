#include "Boundary2f.h"

/*
 *  Boundary2f.cpp
 *  TestBed_Mark
 *
 *  Created by Mark Hauenstein on 25/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


Boundary2f::Boundary2f(float _minX, float _maxX, float _minY, float _maxY){
	set(_minX,_maxX, _minY, _maxY);
}

void Boundary2f::set(Boundary2f& _boundary){
	min.set(_boundary.min);
	max.set(_boundary.max);	
	
}

void Boundary2f::set (float _minX, float _maxX, float _minY, float _maxY){
	min.set(_minX,_minY);
	max.set(_maxX,_maxY);
}

void Boundary2f::set(ofxVec2f _min, ofxVec2f _max){
	min.set(_min);
	max.set(_max);
}

ofxVec2f& Boundary2f::getRandomPoint(ofxVec2f& p){
	p.set(ofRandom(min.x, max.x),ofRandom(min.y, max.y));
	return p;
}

ofxVec2f& Boundary2f::getNormalized(ofxVec2f& p){
	p.set(ofNormalize(p.x, min.x, max.x),ofNormalize(p.y, min.y, max.y));
	return p;
}

bool Boundary2f::contains(const ofxVec2f& p){
	return ofInRange(p.x, min.x, max.x) && ofInRange(p.y, min.y, max.y);
}

bool Boundary2f::contains(const float x_, const float y_){
	return ofInRange(x_, min.x, max.x) && ofInRange(y_, min.y, max.y);
}

bool Boundary2f::overlaps(Boundary2f& b_){
	return ((b_.min.x > min.x && b_.min.x < max.x) || (b_.max.x > min.x && b_.max.x < max.x))&&((b_.min.x > min.x && b_.min.x < max.x) || (b_.max.x > min.x && b_.max.x < max.x));
}

float Boundary2f::getWidth(){
	return fabsf(max.x - min.x);
}

float Boundary2f::getHeight(){
	return fabsf(max.y - min.y);
}

void Boundary2f::draw(){
	ofRect(min.x, min.y, max.x - min.x, max.y - min.y);
}

// for convenience reasons, shouldn't really be here
void Boundary2f::addToGUI(string _name, float _minX, float _maxX, float _minY, float _maxY){
	gui.addSlider(_name+"_x_min", min.x, _minX, _maxX);
	gui.addSlider(_name+"_x_max", max.x, _minX, _maxX);
	gui.addSlider(_name+"_y_min", min.y, _minY, _maxY);
	gui.addSlider(_name+"_y_max", max.y, _minY, _maxY);
}