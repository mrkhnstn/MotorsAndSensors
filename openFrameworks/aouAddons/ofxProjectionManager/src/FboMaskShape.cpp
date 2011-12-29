/*
 *  FboMaskShape.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 26/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboMaskShape.h"

FboMaskShape::FboMaskShape(){
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	clickDistance = FBOMASKCLICKDISTANCE;
}

void FboMaskShape::draw(bool editing, bool invert){
	int max = points.size();
	
	ofSetLineWidth(0);
	ofFill();
	//ofEnableAlphaBlending();
	if (invert) {
		ofSetColor(255, 255, 255, 255);
	} else {
		ofSetColor(0,0,0,255);
	}
	drawShape();
	//drawShapeCurved();
	
	if(editing){
		ofSetLineWidth(2);
		ofNoFill();
		ofSetColor(255, 0, 0, 255);
		//drawShapeCurved();
		drawShape();
		
		drawPoints();
	}
	
}

void FboMaskShape::drawPoints(){
	
	int max = points.size();
	for(int i = 0; i < max; ++i){
		ofxVec2f& point = points[i];
		drawPoint(point, false, false, false);
	}
	if(max > 1){
		for(int i = 0; i < max; ++i){
			ofxVec2f point = getMidPoint(i);
			drawPoint(point, false, false, true);
		}			
	}
}

void FboMaskShape::drawSelection(FboMaskSelection& selection, bool active, bool mouseOver){
	drawPoint(selection.point, active, mouseOver, selection.midPoint);
}

void FboMaskShape::shiftSelection(FboMaskSelection& selection, float dx, float dy){
	if (selection.pointID >= 0 && selection.pointID < points.size()) {
		points[selection.pointID] += ofxVec2f(dx,dy);
	}
}

void FboMaskShape::drawPoint(ofxVec2f& point, bool active, bool mouseover, bool midPoint){
	if(active){
		ofSetColor(255, 0, 0, 255);
	} else if(midPoint){
		ofSetColor(255, 127, 0, 255);		
	} else {
		ofSetColor(255, 255, 0, 255);
	}
	
	ofFill();
	ofSetLineWidth(0);
	ofCircle(point.x, point.y, 4);
	
	if(mouseover){
		ofSetLineWidth(4);
		ofNoFill();
		ofCircle(point.x, point.y, clickDistance);
	} else {
		//ofSetLineWidth(2);
		//ofNoFill();
		//ofCircle(point.x, point.y, clickDistance);			
	}
}

ofxVec2f FboMaskShape::getMidPoint(int i){
	return getLinearPoint(i+0.5);
	//return getCurvePoint(i+0.5);
}

ofxVec2f FboMaskShape::getLinearPoint(float p){
	int max = points.size();
	if(max > 1){
		int i = floor(p);
		float t = p-i;
		
		ofxVec2f& pointA = points[i];
		ofxVec2f& pointB = points[(i+1)%max];
		return pointA*(1.0-t) + pointB*t;
	} else {
		return noPoint();
	}
}

ofxVec2f FboMaskShape::getCurvePoint(float p){
	int max = points.size();
	if(max > 1){
		int i = floor(p);
		float t = p-i;
		
		ofxVec2f& p0 = points[(i+max - 1)%max];
		ofxVec2f& p1 = points[i];
		ofxVec2f& p2 = points[(i+1)%max];
		ofxVec2f& p3 = points[(i+2)%max];
		
		return catmulRom(p0, p1, p2, p3, t);
	} else {
		return noPoint();
	}
}

ofxVec2f FboMaskShape::noPoint(){
	ofxVec2f result;
	return result;
}

void FboMaskShape::drawShape(){
	ofBeginShape();
	int max = points.size();
	for(int i = 0; i < max; ++i){
		ofxVec2f& point = points[i];
		ofVertex(point.x, point.y);
	}
	
	ofEndShape(true);
}

void FboMaskShape::drawShapeCurved(){
	ofBeginShape();
	int max = points.size();
	
	for(int i = 0; i < max; ++i){
		ofxVec2f& p0 = points[(i+max - 1)%max];
		ofxVec2f& p1 = points[i];
		ofxVec2f& p2 = points[(i+1)%max];
		ofxVec2f& p3 = points[(i+2)%max];
		for(float t = 0; t < 1; t += 0.125){
			ofxVec2f point = catmulRom(p0, p1, p2, p3, t);
			ofVertex(point.x, point.y);
		}
		//ofVertex(point.x, point.y);
	}
	
	ofEndShape(true);
	
}

FboMaskSelection FboMaskShape::getNearestPoint(ofxVec2f& target){
	FboMaskSelection nearest;
	float bestDistance = -1;
	int max = points.size();
	
	//check points
	for(int i = 0; i < max; ++i){
		ofxVec2f& point = points[i];
		float distance = point.distance(target);
		if(distance < clickDistance){
			if(!nearest.valid || distance < bestDistance){
				nearest.setPoint(point, i);
				bestDistance = distance;
			}
		}
	}
	
	//check midpoints
	if(max > 1){
		for(int i = 0; i < max; ++i){
			ofxVec2f point = getMidPoint(i);
			float distance = point.distance(target);
			if(distance < clickDistance){
				if(!nearest.valid || distance < bestDistance){
					nearest.setPoint(point, i);
					nearest.midPoint = true;
					bestDistance = distance;
				}
			}
		}
	}
	
	return nearest;
}

void FboMaskShape::insertPoint(FboMaskSelection& selection){
	if(selection.valid && selection.midPoint){
		int max = points.size();
		if(max > 1 && isValidPoint(selection)){
			
			points.insert(points.begin()+selection.pointID+1, selection.point);
			//ofxVec2f& pointA = points[i];
			//ofxVec2f& pointB = points[(i+1)%max];
		}
	}
}

void FboMaskShape::replacePoint(FboMaskSelection& selection){
	if(selection.valid && !selection.midPoint){
		int max = points.size();
		if(isValidPoint(selection)){
			points[selection.pointID] = selection.point;
		}
	}
}

void FboMaskShape::deletePoint(FboMaskSelection& selection){
	if(selection.valid && !selection.midPoint){
		int max = points.size();
		if(isValidPoint(selection)){
			points.erase(points.begin()+selection.pointID);
		}
	}
}

bool FboMaskShape::isValidPoint(FboMaskSelection& selection){
	return selection.pointID >= 0 && selection.pointID < points.size();
}