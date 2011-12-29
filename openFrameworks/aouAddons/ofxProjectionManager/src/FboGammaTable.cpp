/*
 *  FboGammaTable.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboGammaTable.h"

void FboGammaTable::setGamma(float gam){
	g = gam;
	//ig = 1.0/g;
}

float FboGammaTable::gammaNormalized(float i){
	return powf(i, g);
}

float FboGammaTable::igammaNormalized(float i){
	return powf(i, 1.0/g);
}

float FboGammaTable::gamma(float i){
	return 255.0*powf(i/255.0, g);
}

float FboGammaTable::igamma(float i){
	return 255.0*powf(i/255.0, 1.0/g);
}

float FboGammaTable::apply(float i, float a, float black){
	i = i + black;
	i = gamma(i);
	i = i*a;
	i = igamma(i);
	i = i - black;
	return i;
}

ofColor FboGammaTable::apply(ofColor c, float a, float black){
	ofColor result;
	result.r = apply(c.r, a, black);
	result.g = apply(c.g, a, black);
	result.b = apply(c.b, a, black);
	return result;
}

bool FboGammaTable::operator== (FboGammaTable& b){
	return
	g == b.g;// && ig == b.ig;
}