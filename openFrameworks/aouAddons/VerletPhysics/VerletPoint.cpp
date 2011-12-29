/*
 *  VerletPoint.cpp
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 06/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "VerletPoint.h"

void VerletPoint::resetForce(){
	f.set(0,0);
}

void VerletPoint::setMass(float _mass){
	mass = _mass;
	if(mass != 0){
		imass = 1/mass;
	} else {
		imass = 0;//stationary!
	}
}

float VerletPoint::getMass(){
	return mass;
}
float VerletPoint::getDamping(){
	return damping;
}

void VerletPoint::setDamping(float _damping){
	damping = _damping;
	idamping = 1+damping;
}


void VerletPoint::force( float _x, float _y ) {
	f.x += _x;
	f.y += _y;
}
void VerletPoint::force( const ofxVec2f& pnt ) {
	f.x += pnt.x;
	f.y += pnt.y;
}
void VerletPoint::shift( float _x, float _y ) {
	p.x += _x;
	p.y += _y;
	op.x += _x;
	op.y += _y;
}
void VerletPoint::shift( const ofxVec2f& pnt ) {
	p += pnt;
	op += pnt;
}
void VerletPoint::jump( float _x, float _y ) {
	p.set(_x, _y);
	op.set(_x, _y);
}
void VerletPoint::jump( const ofxVec2f& pnt ) {
	p.set(pnt.x, pnt.y);
	op.set(pnt.x, pnt.y);
}

void VerletPoint::step() {
	float x, y;
	
	x = idamping*p.x - damping*op.x + f.x*imass;
	y = idamping*p.y - damping*op.y + f.y*imass;
	
	f.x = 0;
	f.y = 0;
	op.x = p.x;
	op.y = p.y;
	p.x = x;
	p.y = y;
}
