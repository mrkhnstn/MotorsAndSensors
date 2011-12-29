/*
 *  MathUtil2.cpp
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 06/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MathUtil2.h"

ofxVec2f getCircumCenter(ofxVec2f& a, ofxVec2f& b, ofxVec2f& c){
	ofxVec2f result;
	float d = 2*(a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y));
	float x = ((a.y*a.y+a.x*a.x)*(b.y-c.y)+(b.y*b.y+b.x*b.x)*(c.y-a.y)+(c.y*c.y+c.x*c.x)*(a.y-b.y))/d;
	float y = ((a.y*a.y+a.x*a.x)*(c.x-b.x)+(b.y*b.y+b.x*b.x)*(a.x-c.x)+(c.y*c.y+c.x*c.x)*(b.x-a.x))/d;
	result.set(x,y);
	return result;
}

ofxVec2f randomRadialPoint(ofxVec2f& center, float radius, float expo){
	float r = ofRandomuf();
	r = radius*powf(r, expo);
	float t = ofRandom(0, TWO_PI);
	return ofxVec2f(r*sin(t)+center.x, r*cos(t)+center.y);
}


float niceMod(float i, float m){
	return (i<0.0) ? fmod(m-fmod(-i, m),m) : fmod(i,m);
}

float linearAB(float a, float b, float p){
	return a + (b - a)*p;
}

float radialAB(float a, float b, float p){
	float difference = niceMod(a - b, TWO_PI);
	if(difference <= PI){
		//clockwise
		return a - p*difference;
	} else {
		//anticlockwise
		return a + p*(TWO_PI - difference);
	}
}

float wrappedAB(float a, float b, float p, float wrapping){
	float difference = niceMod(a - b, wrapping);
	if(difference <= wrapping*0.5){
		//clockwise
		return a - p*difference;
	} else {
		//anticlockwise
		return a + p*(wrapping - difference);
	}
}

ofxVec2f intersection(ofxVec2f& p1, ofxVec2f& p2, ofxVec2f& p3, ofxVec2f& p4, float snapping){
	float d = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
	float na = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
	float nb = (p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y)*(p1.x - p3.x);
	
	if(-snapping < d && d < snapping){
		//lines are parallel
		if(-snapping < na && na < snapping){
			//lines are coincident
			return ofxVec2f(p1.x, p1.y);
		} else {
			//there is no intersection!
			return ofxVec2f(NAN, NAN);
		}
	} else {
		float ua = na/d;
		float ub = nb/d;
		return ofxVec2f(p1.x + ua*(p2.x - p1.x), p1.y + ua*(p2.y - p1.y));
	}
}