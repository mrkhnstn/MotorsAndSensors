/*
 *  FboBlendEdge.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 24/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboBlendEdge.h"

FboBlendEdge::FboBlendEdge(){
	enabled = true;
	blackLevel = 0;
	blendPoint = 0.5;
	shape = 1;
	setAngle(0);
	setGamma(2.2, 2.2, 2.2);
	//gammaTable.setGamma(2.2);
}

void FboBlendEdge::setAngle(float theta){
	blendDirection.x = cos(theta);
	blendDirection.y = sin(theta);
	blendTangent.x = cos(theta+PI*0.5);
	blendTangent.y = sin(theta+PI*0.5);
}

void FboBlendEdge::setGamma(float r, float g, float b){
	gammaR.setGamma(r);
	gammaG.setGamma(g);
	gammaB.setGamma(b);
}

void FboBlendEdge::setupGUI(string name){
	gui.addTitle(name);
	gui.addToggle("Enable "+name, enabled);
	gui.addSlider("Width "+name, width, 0, 256);
	//gui.addSlider("Gamma "+name, gammaTable.g, 1, 3);
	gui.addSlider("Shape "+name, shape, 1, 17);
	gui.addSlider("BlendPoint "+name, blendPoint, 0, 1);
}

bool FboBlendEdge::operator == (FboBlendEdge& b){
	return
	width == b.width && enabled == b.enabled &&
	blackLevel == b.blackLevel && blendPoint == b.blendPoint && shape == b.shape &&
	position == b.position && blendDirection == b.blendDirection && blendTangent == b.blendTangent &&
	gammaR == b.gammaR && gammaG == b.gammaG && gammaB == b.gammaB;
}

void FboBlendEdge::copyFrom(FboBlendEdge& b){
	width = b.width;
	enabled = b.enabled;
	blackLevel = b.blackLevel;
	blendPoint = b.blendPoint;
	position = b.position;
	blendDirection = b.blendDirection;
	blendTangent = b.blendTangent;
	gammaR = b.gammaR;
	gammaG = b.gammaG;
	gammaB = b.gammaB;
	//gammaTable.setGamma(b.gammaTable.g);
}

float FboBlendEdge::blend(ofPoint p){
	ofPoint offset = p - position;
	float distance = offset.x*blendDirection.x + offset.y*blendDirection.y;
	return blendRelative(0 - distance/width);
}

float FboBlendEdge::blendRelative(float p){
	if(p < 0.0){
		return 0;
	} else if(p < blendPoint){
		return blendPoint*powf(p/blendPoint, shape);
	} else if(p < 1.0){
		return 1.0 - (1.0 - blendPoint)*powf((1.0 - p)/(1.0 - blendPoint), shape);
	} else {
		return 1;
	}
}

void FboBlendEdge::draw(){
	if(!enabled) return;
	
	ofPushStyle();
	
	ofPoint point;
	ofNoFill();
	ofSetLineWidth(2);
	
	//ofSetColor(255, 255, 0);
	//point = position;// + blendTangent*width;
	//ofCircle(point.x, point.y, 4);
	
	//point -= blendDirection*width;
	//ofCircle(point.x, point.y, 4);
	
	//draw blend start line
	ofPoint p1, p2;
	p1 = position - blendDirection*width;
	p2 = p1;
	p1 -= blendTangent* height * 0.5;
	p2 += blendTangent* height * 0.5;
	ofLine(p1.x, p1.y, p2.x, p2.y);
	
	
	/*
	point = position - blendTangent*width;
	ofCircle(point.x, point.y, 4);
	point -= blendDirection*width;
	ofCircle(point.x, point.y, 4);
	*/
	
	float angle = atan2(blendDirection.y, blendDirection.x);
	float flip = angle >=   0?-1:1;//flip the way we draw the curve so adjecent edges fit together nicely
	
	ofSetLineWidth(0);
	ofFill();
	float pointSize = 1;
	for(float i = 0; i >= -1; i -= 0.015625){
		
		/*
		ofSetColor(255, 255, 0);			
		point = position + blendDirection*width*i;
		ofCircle(point.x, point.y, pointSize);
		*/
		
		//ofSetColor(255, 0, 255);
		float p = blend(point);
		point = position + blendDirection*width*i + blendTangent*width*p*flip;// - blendTangent*width*0.5;
		ofCircle(point.x, point.y, pointSize);
		
		/*
		float g;
		//float b = blend(point);
		
		g = 1 - gammaR.igammaNormalized(p);
		ofSetColor(255, 0, 0);
		point = position + blendDirection*width*i - blendTangent*width*g*flip;
		ofCircle(point.x, point.y, pointSize);
		
		g = 1 - gammaG.igammaNormalized(p);
		ofSetColor(0, 255, 0);
		point = position + blendDirection*width*i - blendTangent*width*g*flip;
		ofCircle(point.x, point.y, pointSize);
		
		g = 1 - gammaB.igammaNormalized(p);
		ofSetColor(0, 0, 255);
		point = position + blendDirection*width*i - blendTangent*width*g*flip;
		ofCircle(point.x, point.y, pointSize);
		
		*/
	}
	
	ofSetColor(255, 255, 255);
	
	ofPopStyle();
}