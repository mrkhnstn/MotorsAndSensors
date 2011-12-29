#pragma once

/*
 *  MathUtil2.h
 *  atmosphere
 *
 *  Created by Mark Hauenstein on 12/07/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxVec2f.h"
#include "ofMain.h"

/** Calculates the circumcenter of a triangle defined by three cartesian points
 Equation taken from http://en.wikipedia.org/wiki/Circumcenter
 */
ofxVec2f getCircumCenter(ofxVec2f& a, ofxVec2f& b, ofxVec2f& c);

ofxVec2f randomRadialPoint(ofxVec2f& center, float radius = 1, float expo = 0.5);

float niceMod(float i, float m);

float linearAB(float a, float b, float p);

float radialAB(float a, float b, float p);

float wrappedAB(float a, float b, float p, float wrapping);

ofxVec2f intersection(ofxVec2f& p1, ofxVec2f& p2, ofxVec2f& p3, ofxVec2f& p4, float snapping = 0.000001);