/*
 *  FboMask.cpp
 *  warp
 *
 *  Created by Robin Beitra (robin@beitra.net) on 23/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FboMask.h"

void FboMask::draw(bool editing, bool invert){
	int max = shapes.size();
	for(int i = 0; i < max; ++i){
		shapes[i].draw(editing,invert);
	}
}

void FboMask::drawSelection(FboMaskSelection& selection, bool active, bool mouseOver){
	if(isValidShape(selection)){
		shapes[selection.shapeID].drawSelection(selection, active, mouseOver);
	}
}

void FboMask::shiftSelection(FboMaskSelection& selection, float dx, float dy){
	if(isValidShape(selection)){
		shapes[selection.shapeID].shiftSelection(selection, dx,dy);
	}
}

FboMaskSelection FboMask::getNearestPoint(ofxVec2f& target){
	FboMaskSelection nearest;
	FboMaskSelection temp;
	float bestDistance = -1;
	int numShapes = shapes.size();
	
	for(int i = 0; i < numShapes; ++i){
		FboMaskShape& shape = shapes[i];
		temp = shape.getNearestPoint(target);
		temp.shapeID = i;
		float distance = temp.distance(target);
		if(!nearest.valid || distance < bestDistance){
			nearest = temp;
			bestDistance = distance;
		}
	}
	
	return nearest;
}

void FboMask::insertShape(FboMaskSelection& selection){
	FboMaskShape shape;
	float r = 64;
	float td = PI*2.0/3.0;
	
	for(int i = 0; i < 3; ++i){
		float t = td*i;
		ofxVec2f point;
		point.x = selection.point.x + r*sin(t);
		point.y = selection.point.y + r*cos(t);
		shape.points.push_back(point);
	}
	
	shapes.push_back(shape);
}

void FboMask::insertPoint(FboMaskSelection& selection){
	if(isValidShape(selection)){
		shapes[selection.shapeID].insertPoint(selection);
	}
}

void FboMask::replacePoint(FboMaskSelection& selection){
	if(isValidShape(selection)){
		shapes[selection.shapeID].replacePoint(selection);
	}
}

void FboMask::deletePoint(FboMaskSelection& selection){
	if(isValidShape(selection)){
		FboMaskShape& shape = shapes[selection.shapeID];
		if(shape.points.size() > 2){
			//we will delete the point
			shape.deletePoint(selection);
		} else {
			//it is too small! we will have to delete the shape!
			shapes.erase(shapes.begin()+selection.shapeID);
		}
	}
}

bool FboMask::isValidShape(FboMaskSelection& selection){
	return (selection.shapeID >= 0 && selection.shapeID < shapes.size());
}

void FboMask::save(string path){
	ofxXmlSettings xml;
	
	int max = shapes.size();
	for(int i = 0; i < max; ++i){
		FboMaskShape& shape = shapes[i];
		string shapeString = stringifyVec2fVec(shape.points);
		xml.addValue("shape", shapeString);
	}
	
	xml.saveFile(path);
}

void FboMask::load(string path){
	ofxXmlSettings xml;
	
	xml.loadFile(path);
	
	shapes.clear();
	
	int max = xml.getNumTags("shape");
	for(int i = 0; i < max; ++i){
		FboMaskShape shape;
		string shapeString = xml.getValue("shape", "", i);
		loadStringIntoVec2fVec(shapeString, shape.points);
		shapes.push_back(shape);
	}
}
