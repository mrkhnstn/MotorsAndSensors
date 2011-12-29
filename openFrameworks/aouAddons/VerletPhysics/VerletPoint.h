#pragma once
/*
 *  VerletPoint.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 06/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxVec2f.h"

class VerletPoint {

public: 
	
	ofxVec2f p; // current position
	ofxVec2f op; // previous position
	ofxVec2f f; // current force

	VerletPoint(){
		setMass(1);
		setDamping(0.8);
	}
	
	~VerletPoint(){}
	
	void force( float _x=0.0f, float _y=0.0f );
	void force( const ofxVec2f& pnt );
	void shift( float _x=0.0f, float _y=0.0f );
	void shift( const ofxVec2f& pnt );
	void jump( float _x=0.0f, float _y=0.0f );
	void jump( const ofxVec2f& pnt );
	void resetForce();	
	void setMass(float _mass);
	float getMass();
	void setDamping(float _damping);
	float getDamping();
	
	void step();

protected:
	float mass;
	float imass;
	float damping;
	float idamping;
	
};